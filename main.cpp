#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>


#include "Scanner.hpp"
#include "utils.hpp"
#include "BinSearchTree.hpp"
#include "PriorityQueue.hpp"


int main(int argc, char *argv[]) {
    // std::cout << "the program ran\n";
    // std::cout << "Current working directory: " << std::filesystem::current_path() << "\n";

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    namespace fs = std::filesystem;

    const std::string dirName = std::string("input_output");
    fs::path dirPath(dirName); // directory as a path type

    const std::string inputFileName = std::string(argv[1]);
    const std::string inputFileBaseName = baseNameWithoutTxt(inputFileName);
    // base name for every file type but as a string
    //fs::path Base(inputFileBaseName); // base name for every file type, i don't think i need this

    fs::path input(inputFileName); // files system for the input file
    fs::path TokensFile(inputFileBaseName + ".tokens"); // create a file system path using the output file.
    fs::path Tokens = dirPath / TokensFile;
    fs::path freqFile(inputFileBaseName + ".freq");
    fs::path freq = dirPath / freqFile;
    fs::path hdrFile(inputFileBaseName + ".hdr");
    fs::path hdr = dirPath / hdrFile;
    fs::path codeFile(inputFileBaseName + ".code");
    fs::path code = dirPath / codeFile;

// just testing if the files are even being created
    std::ofstream(freq.string()).close();
    std::ofstream(hdr.string()).close();
    std::ofstream(code.string()).close();


    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
    if (error_type status; (status = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR)
        exitOnError(status, inputFileName);

    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR)
        exitOnError(status, dirName);

    //-- Scanner START
    if (error_type status; (status = canOpenForWriting(Tokens.string())) != NO_ERROR)
        exitOnError(status, Tokens.string());

    std::vector<std::string> words;

    auto fileToWords = Scanner(input); //create the scanner object that will use the file path that file path
    // will later be used to tokensize the contents of that file
    if (error_type status; (status = fileToWords.tokenize(words)) != NO_ERROR)
        exitOnError(status, inputFileName);

    if (error_type status; (status = fileToWords.tokenize(words, Tokens.string())) != NO_ERROR)
        exitOnError(status, Tokens.string());
    //-- Scanner END

    // --- Binary Search Tree Begin

    // Building the binary search tree
    BinSearchTree bst;
    bst.bulkInsert(words); //words is a vector of strings that tokenize generated

    // vector of pairs with strings and ints that will be passed to BST and later used as the input for the Huffman Tree
    std::vector<std::pair<std::string, int> > frequencyList;
    bst.inorderCollect(frequencyList);

    int height = bst.height();
    int uniqueWords = bst.size();
    int totalTokens = words.size();
    int minFreq = 0;
    int maxFreq = 0;

    if (!frequencyList.empty()) {
        minFreq = frequencyList[0].second;
        maxFreq = frequencyList[0].second;

        for (const auto &[word, count]: frequencyList) {
            if (count < minFreq) minFreq = count;
            if (count > maxFreq) maxFreq = count;
        }
    }

    std::cout << "BST height: " << height << "\n";
    std::cout << "BST unique words: " << uniqueWords << "\n";
    std::cout << "Total tokens: " << totalTokens << "\n";
    std::cout << "Min frequency: " << minFreq << "\n";
    std::cout << "Max frequency: " << maxFreq << "\n";
    /*
        // Convert frequencyList to vector<TreeNode*> for PriorityQueue
        std::vector<TreeNode*> leaves;
        leaves.reserve(frequencyList.size());

        for (const auto& [word, count] : frequencyList) {
            leaves.push_back(new TreeNode(word, count));
        }

        const std::string freqFileName = dirName + "/" + inputFileBaseName + ".freq";
        if (error_type status; (status = canOpenForWriting(freqFileName)) != NO_ERROR)
            exitOnError(status, freqFileName);

        PriorityQueue pq(leaves);

        pq.print();
        pq.writeFreqFile(freqFileName);
        */
    return 0;
}
