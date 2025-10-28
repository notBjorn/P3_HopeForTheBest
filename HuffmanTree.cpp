//
// Created by Bhalu Da PC on 10/26/2025.
//

#include "HuffmanTree.hpp"

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string, int> > &counts) {
    //bassically what I did in main for part 2 is now being done inside this function
    //we are basically using the vector that we received from the Binary search tree we created
    //and then we are going to create a new tree with the words and counts but before we do that
    //we need a vector of leaves that will create a priorityQueue which will behave as the leaves for
    //the huffman tree
    std::vector<TreeNode *> leaves;
    leaves.reserve(counts.size()); // create space suggested by clion


    // I did this a bit differently earlier but this a more smart way of doing what I had done
    // bascially we are using the info from the vector of pairs and creating a new node using that info
    // and then we are pushing that node into the vector of leaves
    // this vector of leaves will be passed to our priorityQueue class to do create a priority queue
    for (const auto &[word, count]: counts) {
        leaves.push_back(new TreeNode(word, count));
    }

    // create a priority queue using the vector of leaves we created earlier
    PriorityQueue pq(leaves);

    while (pq.size() > 1) {
        /*
        the logic here is pretty simple I think
        we assign the smallest value to the leftsubtree and the next smallest to right subtree
        ,we recieve this value by using extractMin on the priority queue. For the parent node we
        combine the frequences of the leftNide and the right node. And the keyword that may later be used
        for tie breaking will be the smaller word of the two. do this until we reach the root and we now
        have an almost empty prioQueue and a huffman tree that we will get the code from.
         */
        TreeNode *left = pq.extractMin();
        TreeNode *right = pq.extractMin();

        int newFreq = left->howMany() + right->howMany(); //combine the frequencies of the two nodes
        std::string newKWord = std::min(left->whatWord(), right->whatWord());
        // find the keyword that will be used to break ties

        auto *parent = new TreeNode(newKWord, newFreq);
        parent->leftSubtree(left);
        parent->rightSubtree(right);
        pq.insert(parent);
    }

    HuffmanTree tree;
    tree.root_ = pq.extractMin();
    return tree;
}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string> > &out) const {
    //create a function that goes through the tree recursively and it only inserts when it reaches a leaf node.
    out.clear(); // we don't want duplicate data. Learned this the hard way in tokenize.
    if (root_ == nullptr) return;
    std::string prefix;
    assignCodesDFS(root_, prefix, out);
}

void HuffmanTree::assignCodesDFS(const TreeNode *n,
                                 std::string &prefix,
                                 std::vector<std::pair<std::string, std::string> > &out) {
    if (n == nullptr) return;
    if (n->leftSubtree() == nullptr && n->rightSubtree() == nullptr) {
        out.emplace_back(n->whatWord(), prefix);
        return;
    }
    if (n->leftSubtree() != nullptr) {
        prefix.push_back('0'); // add a 0 to the prefix when we turn left
        assignCodesDFS(n->leftSubtree(), prefix, out); // move left
        prefix.pop_back(); // we remove 0 as we go back
    }

    if (n->rightSubtree() != nullptr) {
        prefix.push_back('1'); // add a 1 to the prefix when we turn right
        assignCodesDFS(n->rightSubtree(), prefix, out); // move right
        prefix.pop_back(); // we remove 1 as we go back
    }
}


error_type HuffmanTree::writeHeader(std::ostream &os) const {
    if (!os)
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    if (root_ == nullptr)
        return NO_ERROR;

    std::string prefix;
    writeHeaderPreorder(root_, os, prefix);
    os << "\n";

    if (!os)
        return FAILED_TO_WRITE_FILE;

    return NO_ERROR;
}


// very similar to what we did in assignCode
void HuffmanTree::writeHeaderPreorder(const TreeNode *n, std::ostream &os,
                                      std::string &prefix) {
    if (n == nullptr) return;

    if (n->leftSubtree() == nullptr && n->rightSubtree() == nullptr) {
        os << n->whatWord() << " " << prefix << "\n";
        return;
    }
    
    if (n->leftSubtree() != nullptr) {
        prefix.push_back('0');
        writeHeaderPreorder(n->leftSubtree(), os, prefix);
        prefix.pop_back();
    }

    if (n->rightSubtree() != nullptr) {
        prefix.push_back('1');
        writeHeaderPreorder(n->rightSubtree(), os, prefix);
        prefix.pop_back();
    }




}

//The Desconstructa
HuffmanTree::~HuffmanTree() {
    destroy(root_);
}


//Compiler was screaming about the destructer so I had to make one quick lets see if it works
void HuffmanTree::destroy(TreeNode *n) noexcept {
    if (n == nullptr) return;
    destroy(n->leftSubtree());
    destroy(n->rightSubtree());
    delete n;
}
