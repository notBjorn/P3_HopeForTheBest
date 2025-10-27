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

    //while (pq.size() > 1) {
        /*
        the logic here is pretty simple I think
        we assign the smallest value to the leftsubtree and the next smallest to right subtree
        ,we recieve this value by using extractMin on the priority queue. For the parent node we
        combine the frequences of the leftNide and the right node. And the keyword that may later be used
        for tie breaking will be the smaller word of the two. do this until we reach the root and we now
        have an almost empty prioQueue and a huffman tree that we will get the code from.
         */
  //  }

    HuffmanTree tree;

    return tree;
}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string> > &out) const {
    //create a function that goes through the tree recursively and it only inserts when it reaches a leaf node.
    //

}
