//
// Created by Bhalu Da PC on 10/11/2025.
// The code in this file was copied from one of code provided to us earlier by prof. Ali Kooshesh
// This header file is so small that it doesn't need a seperate .cpp file
//
#include <string>

#ifndef P3_PART1_NODE_H
#define P3_PART1_NODE_H

class TreeNode {
    public:
    //constructor
    TreeNode(std::string c, int n) : left(nullptr), right(nullptr), word(c), count(n) {
    };

    //Destructor
    ~TreeNode() = default;



    // following two functions will modify the left and right nodes
    void leftSubtree(TreeNode* newLeft) {left = newLeft;}
    void rightSubtree(TreeNode* newRight) {right = newRight;}
    void increaseCount() { count++; }

    // following two functions will return us the left and right node
    TreeNode *leftSubtree() { return left; }
    [[nodiscard]] const TreeNode* leftSubtree() const { return left; } //some functions call for const some don't

    TreeNode *rightSubtree() {return right; }
    [[nodiscard]] const TreeNode* rightSubtree() const { return right; }

    // following function will return us the data in the node
    std::string& whatWord() { return word; }
    [[nodiscard]] const std::string& whatWord() const { return word; }

    int& howMany() { return count; }
    [[nodiscard]] const int& howMany() const { return count; }

private:
    TreeNode* root;
    TreeNode* left, *right;
    std::string word;
    int count;
};


#endif //P3_PART1_NODE_H