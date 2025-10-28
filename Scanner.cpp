//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include "utils.hpp"

Scanner::Scanner(std::filesystem::path inputPath) {
    inputPath_ = std::move(inputPath);
}


error_type Scanner::tokenize(std::vector<std::string>& words) {
    std::ifstream inputFile(inputPath_);

    if (!inputFile.is_open()) return UNABLE_TO_OPEN_FILE;

    words.clear();//clear words to make sure if multiple calls are made only a single set of data is kept.

    std::string token;
    while (!inputFile.eof()) {
        token = readWord(inputFile);
        if (token.empty()) break;
        words.push_back(token);
    }

    return NO_ERROR;
}



//Copied over from utils not sure what is there to change here
error_type Scanner::tokenize(std::vector<std::string> &words, const std::filesystem::path &outputFile) {
    std::ofstream out(outputFile, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }

    tokenize(words);

    for (const auto& item : words) {
        out << item << '\n';
        if (!out) {
            std::cerr << "Error: failed while writing to " << outputFile << "\n";
            return FAILED_TO_WRITE_FILE;
        }
    }

    if (out.fail()) {
        return ERR_TYPE_NOT_FOUND;
    }

    return NO_ERROR;
}



std::string Scanner::readWord(std::istream& in) {
    std::string word;
    char c;

    while (in.get(c)) {
        if (std::isalpha(static_cast<unsigned char>(c)))
            word.push_back(std::tolower(static_cast<unsigned char>(c))); //forgot to lower the char here
        else if (c == '\'' && !word.empty() && std::isalpha(in.peek())) {
            word.push_back(c);
        } else {
            if (!word.empty()) break; //there is something in the word and c that means we have reached the end
        }
    }
    return word;
}