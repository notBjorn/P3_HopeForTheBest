//
// Created by Bhalu Da PC on 10/11/2025.
//

#include "BinSearchTree.hpp"

BinSearchTree::~BinSearchTree() {
    destroy(root_);
}


void BinSearchTree::insert(const std::string &word) {
    root_ = insertHelper(root_, word);
}



// Should Work
TreeNode *BinSearchTree::insertHelper(TreeNode *root, const std::string &word) {
    if (root == nullptr)
        return new TreeNode(word, 1);

    if (root->whatWord() == word) {
        root->increaseCount();
        return root;
    } else if (root->whatWord() > word) {
        root->leftSubtree(insertHelper(root->leftSubtree(), word));
    } else if (root->whatWord() < word) {
        root->rightSubtree(insertHelper(root->rightSubtree(), word));
    }
    return root;
}



// Should Work
void BinSearchTree::bulkInsert(const std::vector<std::string> &words) {
    for (const std::string &word: words) {
        root_ = insertHelper(root_, word);
    }
}



//Should Work
bool BinSearchTree::contains(std::string_view word) const noexcept {
    return findNode(root_, word) != nullptr;
}



//Should Work
const TreeNode *BinSearchTree::findNode(const TreeNode *root, std::string_view word) noexcept {
    if (root == nullptr) {
        return nullptr; // if we are returning nullptr this means we could not find the node
    }
    if (word == root->whatWord()) return root;
    if (word < root->whatWord()) return findNode(root->leftSubtree(), word);
    return findNode(root->rightSubtree(), word);
}



std::optional<int> BinSearchTree::countOf(std::string_view word) const noexcept {
    const TreeNode *node = findNode(root_, word);
    if (node != nullptr)
        return node->howMany();
    return std::nullopt; //
}



void BinSearchTree::inorderCollect(std::vector<std::pair<std::string, int> > &out) const {
    inorderHelper(root_, out);
}



void BinSearchTree::inorderHelper(const TreeNode *node, std::vector<std::pair<std::string, int> > &out) {
    if (node == nullptr) return;
    inorderHelper(node->leftSubtree(), out);
    out.emplace_back(node->whatWord(), node->howMany());
    inorderHelper(node->rightSubtree(), out);
}



std::size_t BinSearchTree::size() const noexcept {
    return sizeHelper(root_);
}



std::size_t BinSearchTree::sizeHelper(const TreeNode* node) noexcept {
    if (node == nullptr) return 0;
    return 1 + sizeHelper(node->leftSubtree()) + sizeHelper(node->rightSubtree());
}



unsigned BinSearchTree::height() const noexcept {
    return heightHelper(root_);
}



unsigned BinSearchTree::heightHelper(const TreeNode* node) noexcept {
    if (node == nullptr) return 0;
    return 1 + std::max(heightHelper(node->leftSubtree()), heightHelper(node->rightSubtree()));
}



void BinSearchTree::destroy(TreeNode *node) noexcept {
    if (node == nullptr) return;
    destroy(node->leftSubtree());
    destroy(node->rightSubtree());
    delete node;
}
