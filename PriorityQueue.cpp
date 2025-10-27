//
// Created by Bhalu Da PC on 10/17/2025.
//

#include "PriorityQueue.hpp"

PriorityQueue::PriorityQueue(std::vector<TreeNode*> nodes) {
    if (nodes.empty()) {
        return;
    }
    // Use insert() for each node - implements insertion sort algorithm
    for (TreeNode* node : nodes) {
        insert(node);
    }
}


std::size_t PriorityQueue::size() const noexcept{
    return items_.size();
}


bool PriorityQueue::empty() const noexcept {
    return items_.empty();
}


TreeNode* PriorityQueue::findMin() const noexcept {
    if (empty()) return nullptr;
    return items_.back();
}



TreeNode* PriorityQueue::extractMin() noexcept {
    if (empty()) return nullptr;
    TreeNode* min = items_.back();
    items_.pop_back();
    return min;
}


void PriorityQueue::deleteMin() noexcept {
    if (!items_.empty()) {
        items_.pop_back();
    }
}

void PriorityQueue::insert(TreeNode* node) {
    // Make space for the node we want to add
    items_.push_back(nullptr);
    int idx = static_cast<int>(items_.size()) - 2;


    while (idx >= 0 && higherPriority(node, items_.at(idx))) {
        items_.at(idx + 1) = items_.at(idx);
        idx--;
    }
    // Insert node
    items_.at(idx + 1) = node;

}



bool PriorityQueue::higherPriority(const TreeNode* a, const TreeNode* b) noexcept {
    // check frequenct
    if (a->howMany() != b->howMany()) {
        return a->howMany() > b->howMany();
    }
    // when frequecy is the same check word priority
    return a->whatWord() < b->whatWord();
}

bool PriorityQueue::isSorted() const {
    for (std::size_t i = 1; i < items_.size(); ++i) {
        if (higherPriority(items_.at(i), items_.at(i-1))) {
            return false;
        }
    }
    return true;
}

void PriorityQueue::print(std::ostream& os) const {
    os << "PriorityQueue:\n";
    for (const auto* node : items_) {
        os << "  " << node->whatWord() << ": " << node->howMany() << "\n";
    }
}

void PriorityQueue::writeFreqFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::out | std::ios::trunc);

    if (!out.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << "\n";
        return;
    }

    // Write each (word, count) pair: one per line as "word count"
    for (const auto* node : items_) {
        out << node->whatWord() << " " << node->howMany() << '\n';
        if (!out) {
            std::cerr << "Error: Failed while writing to " << filename << "\n";
            return;
        }
    }
}
