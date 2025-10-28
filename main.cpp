#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>


#include "Scanner.hpp"
#include "utils.hpp"
#include "BinSearchTree.hpp"
#include "HuffmanTree.hpp"
#include <algorithm>


// function that I wiil use compare frequencies before writing the freq file
bool compareFrequency(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
    if (a.second != b.second) {
        return a.second > b.second;
    }
    return a.first < b.first;
}


//function that will write the freq file with the correct format
void writeFreq(std::string filename, std::vector<std::pair<std::string, int> > freqList) {
    std::ofstream out(filename, std::ios::out | std::ios::trunc);

    if (!out.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << "\n";
        return;
    }

    // Write each (word, count) pair: one per line as "word count"
    for (auto items: freqList) {
        out << std::setw(10) << items.second << ' ' << items.first << '\n';
        if (!out) {
            std::cerr << "Error: Failed while writing to " << filename << "\n";
            return;
        }
    }
    out.close();
}


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

    fs::path inputFile(inputFileName); // files system for the input file
    fs::path input = dirPath / inputFile; // path to the input file
    fs::path TokensFile(inputFileBaseName + ".tokens"); // create a file system path using the output file.
    fs::path Tokens = dirPath / TokensFile;
    fs::path freqFile(inputFileBaseName + ".freq");
    fs::path freq = dirPath / freqFile;
    fs::path hdrFile(inputFileBaseName + ".hdr");
    fs::path hdr = dirPath / hdrFile;
    fs::path codeFile(inputFileBaseName + ".code");
    fs::path code = dirPath / codeFile;


    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
    if (error_type status; (status = regularFileExistsAndIsAvailable(input.string())) != NO_ERROR)
        exitOnError(status, input.string());

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
        exitOnError(status, TokensFile.string());
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


    // --- Binary Search Tree End


    // -- Printing The Frequency File
    // Our sorting function
    // Could have copied the way I did it in Priority Queue but that would have been complex for no reason
    auto freqSorted = frequencyList; // don't want to change the OG freq file cause that needs to go to Huffman
    std::sort(freqSorted.begin(), freqSorted.end(), compareFrequency); //our sorting function

    if (error_type status; (status = canOpenForWriting(freq.string())) != NO_ERROR)
        exitOnError(status, freqFile.string());

    writeFreq(freq.string(), freqSorted);

    // -- Frequency File Has been made

    // -- Building the Huffman Tree
    HuffmanTree HFtree = HuffmanTree::buildFromCounts(frequencyList);
    // static member so call it a bit differently I had forgotten
    if (error_type status; (status = canOpenForWriting(hdr.string())) != NO_ERROR)
        exitOnError(status, hdrFile.string());


    std::ofstream writeHdr(hdr);
    if (error_type status; (status = HFtree.writeHeader(writeHdr)) != NO_ERROR)
        exitOnError(status, hdr.string());
    writeHdr.close();

    writeHdr.close();

    // --hdr file is now written


    if (error_type status; (status = canOpenForWriting(code.string())) != NO_ERROR)
        exitOnError(status, codeFile.string());

    std::ofstream writeCode(code);

    if (error_type status; (status = HFtree.encode(words, writeCode, 80)) != NO_ERROR)
        exitOnError(status, code.string());

    writeCode.close();
    return 0;
}
