#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
// Importing the required libraries.

using std::ios;
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::fstream;
using std::unordered_map;
using std::priority_queue;
// Using the required objects from the standard namespace.

// Tree type data-structure for storing the Huffman Tree
struct TreeNode {
    char character;
    // Character which is reffered on the node.
    int frequency;
    // Frequency of the specified character.
    TreeNode *leftNode, *rightNode;
    // Pointers of type TreeNode to refer to the left and the right node.
};

// Function that will create a new node of TreeNode type and will return a pointer
// that can be used as a refrence for the node.
// Function also behaves like a constructor which accepts the values of the data points
// and set them so that we need not to initialize them later. 
TreeNode* getNewTreeNode(char character, int frequency, TreeNode* leftNode, TreeNode* rightNode) {
    TreeNode* newTreeNode = new TreeNode();
    // Creates a new tree node.

    newTreeNode->character = character;
    // Sets the character for which the tree node is created.
    newTreeNode->frequency = frequency;
    // Sets the frequency of the character.

    newTreeNode->leftNode = leftNode;
    newTreeNode->rightNode = rightNode;
    // Initializes the left and right pointer for the tree.

    return newTreeNode;
    // Returns the refrence for the new created node.
}

// Comparator for the priority queue to find which node had the highest priority.
struct comparator 
{
    // Overloading the () operator to compare the different values of two tree nodes.
    // It will return the node with the highest priority.
    bool operator()(TreeNode* leftNode, TreeNode* rightNode)
    {
        return leftNode->frequency > rightNode->frequency;
        // Comparing the frequency of both the nodes.
        // Here, the highest priority item has lowest frequency.
    }
};

// Function that will calculate the huffman encoding for each of the character and 
// will store it in the unordered map's reference so that the table for the 
// reference can be used later as required.
void huffmanEncoding(TreeNode* rootNode, string strEncode, unordered_map<char, string> &huffmanCodeMap)
{
    if (rootNode == nullptr)
    {
        return;
        // If the rootNode is empty, thn it will return and the function will exit.
    }

    if(!rootNode->leftNode && !rootNode->rightNode)
    {
        huffmanCodeMap[rootNode->character] = strEncode;
        // If the node is the leaf node, then it will store the encoded string for 
        // the future reference in the unordered map.
    }

    huffmanEncoding(rootNode->leftNode, strEncode + "0", huffmanCodeMap);
    // Add a 0 in the encoding for the recursive left node.
    huffmanEncoding(rootNode->rightNode, strEncode + "1", huffmanCodeMap);
    // Add a 1 in the encoding for the recursive right node.
}

// Function will build huffman tree for the plain text. Assign the code and store
// them in the unordered map which will further be used to build the encoded message
// The function will return the encoded message.
string buildHuffmanTree(string plainText)
{
    unordered_map<char, int> frequencyMap;
    // Unordered Map is used to store the frequency of each character.
    for(char ch: plainText) 
    {
        frequencyMap[ch]++;
    }
    // Range-based for loop is used to iterate through each character in the plain
    // text and further to count it's frequency and store it in the map.

    priority_queue<TreeNode*, vector<TreeNode*>, comparator> minFrequencyPriorityQueue;
    // Priority Queue is used for storing the leaf nodes of Huffman Tree.

    for(auto characterFrequencyPair: frequencyMap)
    {
        minFrequencyPriorityQueue.push(getNewTreeNode(characterFrequencyPair.first, characterFrequencyPair.second, nullptr, nullptr));
        // Here, we are using the range based for loop for creating the leaf nodes 
        // for each character and adding it to the priority queue.
        // characterFrequencyPair.first -> Stores the character
        // characterFrequencyPair.second -> Stores the frequency
    } 

    while(minFrequencyPriorityQueue.size() != 1) 
    {
        TreeNode *rightNode = minFrequencyPriorityQueue.top();
        minFrequencyPriorityQueue.pop();
        // Fetches the highest priority (min frequency) node from the priority queue
        // and treat it as the right node.
        // Remove it to avoid repetition.

        TreeNode *leftNode = minFrequencyPriorityQueue.top();
        minFrequencyPriorityQueue.pop();
        // Fetches the highest priority (min frequency) node from the priority queue
        // and treat it as the left node.
        // Remove it to avoid repetition.

        int sumOfFrequency = leftNode->frequency + rightNode->frequency;
        // Calculate the frequency of new node which has these two selected node as 
        // it's child nodes.
        minFrequencyPriorityQueue.push(getNewTreeNode('\0', sumOfFrequency, leftNode, rightNode));
        // Create a new node with the sumOfFrequencies and add it to the priority queue.
    }
    // The above process will be repeated till there exists a single node in the 
    // priority queue.

    TreeNode* rootNode = minFrequencyPriorityQueue.top();
    // The last remaining node from the priority queue will be treated as the root
    // node for the Huffman Encoding.

    unordered_map<char, string> huffmanCodeMap;
    // Unordered Map will be used to store the character and it's associated 
    // Huffman code.

    huffmanEncoding(rootNode, "", huffmanCodeMap);
    // Use the huffman encoding function for generating the encoding and storing it
    // in the unordered map.

    cout<<"Huffman codes are : \n";
    for(auto characterCodePair: huffmanCodeMap) 
    {
        cout<<characterCodePair.first<<" - "<<characterCodePair.second<<"\n";
        // Using the range based for loop for printing the codes generated for each
        // character in the huffman encoding.
    }

    string encodedMessage = "";
    // Using an empty string for storing the encoded message.
    for(char character: plainText)
    {
        encodedMessage += huffmanCodeMap[character];
        // Use the unordered map to access the encoded message for the character
        // and adding the encoded message to the string.
    }

    return encodedMessage;
    // Returns the encoded message.
}

// Function to read the content of the file and return them as a single string.
string readPlainTextFile(string locationOfFile) 
{
    string fileText = "";
    // String to store the content of whole file.
    string fileLine;
    // String to store the line read.

    fstream fileToBeEncoded;
    // Created a file stream object.
    fileToBeEncoded.open(locationOfFile, ios::in);
    // Open the file in read mode.
    if(fileToBeEncoded.is_open()) 
    {
        while(getline(fileToBeEncoded, fileLine))
        {
            if(fileText != "") 
            {
                fileText += "_";
            }
            fileText += fileLine;
        }
        // Read the lines and store them in a temporary string.
        // Further add the string to the previous string so that a single string can
        // contain the whole text.
        fileToBeEncoded.close();
        // Close the file once it is read.
    }
    cout<<"\n\n"<<"Original message is : "<<"\n";
    cout<<fileText<<"\n\n";
    // Print the plain text in the file.

    return fileText;
    // Return the string containing the file text.
}

// Function to write the encoded message to the file.
void writeToCompressedFile(string locationOfFile, string encodedMessage)
{
    cout<<"\n"<<"Encoded message is : "<<"\n";
    cout<<encodedMessage<<"\n\n";
    // Print the encoded message.

    fstream encodedFile;
    // Created a file stream object.
    encodedFile.open(locationOfFile+".compressed", ios::binary | ios::out);
    // Open the file in write mode.
    if(encodedFile.is_open())
    {
        encodedFile<<encodedMessage;
        encodedFile.close();
        // Write the encoded message to the file and close the file.
    }

    return;
    // Return from the function.
}

// Function to calculate the compression ratio and dump it on the screen.
void calculateCompressionRatio(string textString, string encodedString)
{
    double compressionRatio = 0.0;
    // Initialize compression ratio to ZERO.

    double originalSize = textString.length() * 8.0;
    // Calculate the original size of the file.
    double encodedSize = encodedString.length();
    // Calculate the size of the file after Huffman Encoding.

    compressionRatio = 100.0 - (encodedSize/originalSize*100);
    // Calculate the compression ratio.

    cout<<"Compression ratio for the current encoding is : "<<compressionRatio<<"%"<<"\n\n";
    // Dump the compression ratio on the screen.

    return;
    // Return from the function.
}

int main() 
{
    cout<<"\n"<<"HUFFMAN ENCODING";
    // Print message on the screen.
    //string fileLocation;
    //cout<<"enter the file name:";
    //cin>> fileLocation;
    string fileLocation = "test.txt";

    string textString = readPlainTextFile(fileLocation);
    // String to be input from file.

    string encodedString = buildHuffmanTree(textString);
    // Encode the string using huffman encoding.

    writeToCompressedFile(fileLocation, encodedString);
    // Store the encoded message to the file specified.

    calculateCompressionRatio(textString, encodedString);
    // Calculate the compression ratio of the Huffman Encoding.

    return 0;
    // Return 0 after successful execution of the program.
}