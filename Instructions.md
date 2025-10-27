
### Project 3 -- Huffman Trees and Coding

### Introduction

Humans have always needed ways to encode messages—mapping symbols (letters, words) to signals that can be stored or transmitted and later recovered. A good code is unambiguous, compact, and fast to process.

Morse code is a classic example. Each letter is represented by a sequence of short and long signals (dots and dashes). Common letters like e use very short codes, while rare letters are longer. Morse is a variable‑length, prefix‑free code: no letter’s code is the prefix of another, so a receiver can decode a stream of dots and dashes with a greedy algorithm, one symbol at a time.

Modern computing uses many coding schemes, from fixed‑length encodings like ASCII and Unicode to variable‑length statistical codes that exploit frequency information. Examples include Shannon–Fano coding, Huffman coding, arithmetic coding, and dictionary‑based methods (the LZ family) used in formats like ZIP and PNG. These systems balance two goals: represent data compactly and keep encoding/decoding efficient.

Huffman coding, the focus of this project, builds an optimal prefix‑free binary code when symbol frequencies are known. More frequent symbols receive shorter bit strings; rarer symbols receive longer ones. The codebook can be viewed as a binary tree: leaves store symbols, and a left/right path from the root spells out the symbol’s bit code.

We study Huffman coding because it ties together core ideas you are learning this term: binary search trees for frequency counting, sorting as a stand‑in for a priority queue (we'll cover priority queues later), and tree construction/traversal for encoding and decoding. It is also widely used in practice: as a building block inside compressors and file formats, in networking stacks, and in embedded systems where memory is tight.

In this project you will tokenize text, count word frequencies, build a Huffman tree from those counts, generate a codebook, and use it to encode and decode text. In the second phase, you will work through several text files drawn from Jack London and Theodore Dreiser—starting with chapter‑length segments (for example, chapters from The Call of the Wild and stories from Lost Face), then moving up to complete books—rebuilding the tree after each addition and observing how code lengths, tree shape, and decode times change. Along the way you will log statistics and create simple plots that make the learning visual and more effective.

By the end, you will have a working understanding of how variable‑length prefix codes are constructed and why they yield real compression, as well as hands‑on practice with the data structures and algorithms that make them possible.

### Building Blocks of the Project

You will implement and test an end-to-end encoder/decoder pipeline for word-level Huffman coding.

1. Read a text file and tokenize it into words.

2. Insert the words into a binary search tree to count how many times each word appears.

3. From the counts, build a descending frequency-sorted list of (word, count). This list stands in place of a heap-ordered priority queue that we will introduce in a later phase.

4. Build a Huffman tree using the words and their frequencies.

5. Generate a codebook (word -> bitstring) and write two outputs:

- a header file that stores the codebook

- an encoded file that stores the compressed bitstream

```
   Phase note: the decoder will be implemented in Phase 2 (later in the assignment sequence). For Phase 1 you only need to produce the codebook and the encoded file.

  
```

Note on granularity: the source texts are normal prose, but this project encodes and decodes at the word level. The decoder reconstructs the sequence of normalized words produced by your tokenizer, not the original punctuation or casing.

### Goals (Phase 1)

- Build a frequency counter over tokens using a binary search tree.

- Replace a priority queue with a frequency-sorted list to drive Huffman construction.

- Build the Huffman tree.

- Traverse the tree to generate a codebook (word -> bitstring).

- Produce intermediate output files to promote building and testing software in phases.

### Phase 1 outputs and files (using base-name.txt for the input-file name)

- base-name.tokens

- base-name.freq — word, count sorted by count descending then word ascending

- base-name.hdr — one mapping per line: word code (single space)

- base-name.code — the encoded bitstream

## Tokenization

The inputs for this project are normal English prose—chapters and short stories from Jack London and Theodore Dreiser—with capitalization, punctuation, quotation marks, dashes/hyphens, numbers, and contractions. Our focus is to identify and encode words only. Tokenization converts each text into a normalized, ASCII‑only sequence of word tokens that form the basis of the frequency counting and Huffman coding. This normalization makes results unambiguous and portable: the same source text must always yield the same token sequence across machines, and (in Phase 2) decoding will reproduce the token sequence rather than the original punctuation or casing.

Rules

- Lowercase the entire input (ASCII only).

- A token is letters a–z with optional internal apostrophes.

    - Allowed patterns: [a–z]+ or [a–z]+’[a–z]+ (apostrophes only between letters).

    - Drop leading/trailing apostrophes.

- Treat the following as separators (never part of tokens):

    - digits and numbers,

    - whitespace,

    - punctuation and symbols (including quotes and hyphens/dashes such as -, –, —),

    - any non-ASCII byte.

- Resulting tokens contain only a–z and apostrophes; they never include spaces, tabs, or newlines.


Examples

```

Input: Wolf-dog races – fast!

Tokens: wolf, dog, races, fast

Input: 'Tis the camp's fire.

Tokens: tis, the, camp's, fire

Input: rock-n-roll and O'Keefe

Tokens: rock, n, roll, and, o'keefe

Input: Room 101... really?

Tokens: room, really

Input: café on the corner

Tokens: caf, on, the, corner

Input: 'em—again

Tokens: em, again

```

#### Determinism

Use the exact same tokenization wherever tokens are needed. The token stream produced by these rules forms the basis for counting, encoding, and (later) decoding tests.

#### Implementation hint

The scanner for this type of tokenization is straightforward: when you see a letter, start a token; continue consuming letters; if you see an apostrophe followed by a letter, keep it and continue; otherwise, end the token. Everything else (digits, punctuation, hyphens/dashes, non‑ASCII) is a separator.

Edge cases

- Empty input produces zero tokens.

- A file with one distinct token is valid; its Huffman code should be "0".

- Extremely long tokens are allowed.

- Tabs/newlines/spaces never appear inside tokens; this makes "word code" lines in codes.hdr well defined when you parse it to decode a message. You will learn about this in the upcoming sections.

### Implementation (Scanner)

Use a small Scanner class to encapsulate tokenization. The “write to file” overload should call the in‑memory version so the scan logic lives in one place.

```
#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <istream>

enum error_type { // Added here for information only. Will have to be shared with other classes.
    NO_ERROR,
    FILE_NOT_FOUND,
    DIR_NOT_FOUND,
    UNABLE_TO_OPEN_FILE,
    ERR_TYPE_NOT_FOUND,
    UNABLE_TO_OPEN_FILE_FOR_WRITING,
    FAILED_TO_WRITE_FILE,
};

class Scanner {
public:
    explicit Scanner(std::filesystem::path inputPath);

    // Tokenize into memory (according to the Rules in this section).
    error_type tokenize(std::vector<std::string>& words);

    // Tokenize and also write one token per line to 'outputFile' (e.g., <base>.tokens).
    // This overload should internally call the in‑memory tokenize() to avoid duplicate logic.
    error_type tokenize(std::vector<std::string>& words,
                        const std::filesystem::path& outputFile);

    ~Scanner() = default;

private:
    // Read the next token from 'in'. Returns empty string when no more tokens.
    // Follows the project’s tokenization rules: letters a–z with optional internal apostrophes;
    // digits, punctuation, hyphens/dashes, whitespace, and non‑ASCII are separators.
    static std::string readWord(std::istream& in);

    std::filesystem::path inputPath_;
};
```

Behavior & error returns

- On success, return NO_ERROR.

- If the input directory does not exist, return DIR_NOT_FOUND; if the file is missing, FILE_NOT_FOUND; if opening fails, UNABLE_TO_OPEN_FILE.

- For the file‑writing overload: if the output file cannot be opened, return UNABLE_TO_OPEN_FILE_FOR_WRITING; if a write fails, return FAILED_TO_WRITE_FILE.

- When writing the `.tokens` file: write exactly one token per line and end the file with a single trailing newline.


### std::filesystem

As you have noticed, one of the Scanner's tokenize functions takes an argument of type: std::filesystem::path. We use std::filesystem because it provides a portable way to split and join paths and to derive base names/extensions without brittle string slicing. It abstracts path separators across platforms and keeps file/directory logic separate from your tokenization and encoding code. You can always call .string() when you need a plain std::string for file I/O.

Alternative

You could do all of this with raw std::string operations (find_last_of('/'), substr, concatenation) or platform-specific APIs. That works, but it’s easier to make mistakes and harder to read. For this project we prefer std::filesystem for clarity and portability.

Common std::filesystem::path members

- parent_path() — directory of the path (e.g., input_output)

- filename() — last component (e.g., call_of_the_wild_ch01.txt)

- stem() — base name without extension (e.g., call_of_the_wild_ch01)

- extension() — extension (e.g., .txt)

- string() — convert a path to std::string


Also handy

- operator/ — join paths (dir / "file.ext")

- replace_extension(".code") — change extension in place

- has_parent_path(), has_extension() — quick checks


Example

```
namespace fs = std::filesystem;

fs::path in = "input_output/call_of_the_wild_ch01.txt";

fs::path dir  = in.parent_path();                 // "input_output"
std::string base = in.stem().string();            // "call_of_the_wild_ch01"

fs::path tokens = dir / (base + ".tokens");      // input_output/call_of_the_wild_ch01.tokens
fs::path freq   = dir / (base + ".freq");        // input_output/call_of_the_wild_ch01.freq
fs::path hdr    = dir / (base + ".hdr");         // input_output/call_of_the_wild_ch01.hdr
fs::path code   = dir / (base + ".code");        // input_output/call_of_the_wild_ch01.code
```

### Counting word frequencies (BST)

From the Tokenization step you now have a stream of normalized word tokens. In this step you turn that stream into a frequency table—how many times each word appears—using a binary search tree that you implement. A BST is a viable choice for this problem and, importantly, it’s the structure you’re learning now: inserts are simple, the lexicographic comparison matches our token order, and an inorder traversal yields a clean, deterministic list we can pass to the next stage (the frequency‑sorted list that stands in for a priority queue).

#### Overview of other data-structure options

- Array/vector + full sort: collect all tokens, sort them, then run-length count. Simple, cache-friendly, and deterministic; good for one-shot batches. Not ideal if you need interleaved queries or incremental updates (online operations.)

- Hash table (std::unordered_map): average O(1) insert/find and scales well to large vocabularies. Iteration order isn’t defined, so producing sorted output requires an extra step; memory overhead is higher.

- Balanced map (std::map): tree-ordered map with O(log V) inserts/lookups and in-order iteration by word. Handy when you want lexicographic output without a separate sort; slower constants than an unordered_map.

- Trie/radix tree: stores characters along edges; supports prefix queries and lexicographic walks. Higher memory use and more per-character work; overkill for this assignment’s word-level counting.

- Skip list: probabilistic list structure with expected O(log V) operations. Simpler to implement than a balanced tree, but still more machinery than we need right now; covered later in the course.

- Hand-built binary search tree (BST): straightforward pointers and recursion; exposes tree-shape effects and traversals. Can skew on ordered inputs unless you randomize insertion; great for practicing core concepts.


### Skewed Trees

- The shape of a BST is how nodes are arranged. The height is the length of the longest root‑to‑leaf path.

- A tree is skewed when it looks like a long chain (each node has only one child). In the extreme, operations degrade toward linear time because you traverse many nodes per insert/find.


#### Why it happens

- In a plain BST, inserts follow comparisons only—no rotations. If words arrive already ordered (e.g., tokens in lexicographic order) or nearly ordered, each new insert tends to go to the same side, creating a long path.


#### How to mitigate (deterministic randomization)

- Shuffle the token sequence once before building the BST. Random arrival order tends to interleave keys so the tree’s expected height is about O(log V) for V distinct words.

- Keep it deterministic so runs are reproducible:

    1. Collect tokens into a vector.

    2. Choose a fixed seed (e.g., a course‑provided constant, or a stable hash of the input file’s base name).

    3. Use std::shuffle(tokens.begin(), tokens.end(), std::mt19937(seed)).

    4. Insert in that shuffled order.


#### Pseudocode (one time per file)

```
seed ← 0xC0FFEE  #
rng  ← mt19937(seed)
shuffle(tokens.begin(), tokens.end(), rng)
for w in tokens:
    bst_insert_or_increment(root, w)
```

#### When it’s okay not to shuffle

- For small, chapter‑length inputs, even a skewed tree may be fast enough. Skipping the shuffle keeps the pipeline purely online.


#### Measure and report (recommended)

- Implement bst_height(root) and, after the build, record:

    - V (vocabulary size),

    - tree height,

    - total tokens.  
      This helps you see the effect of shuffling vs. not shuffling.


### Input

- tokens: the list of normalized words produced by the Tokenization step for one text file


### Requirements

- Implement your own BST with nodes that store: word, count, left, right.

- Compare keys lexicographically on the lowercase word string.

- On insertion: if the word already exists, increment its count; otherwise create a new node with count = 1.

- After processing all tokens, traverse the BST **inorder** (left, node, right) to produce a flat vector of (word, count) pairs ordered by word ascending. This order is deterministic and independent of the order in which tokens were inserted (even if you shuffled).


#### Complexity notes

- With randomized insertion, expected insertion time per token is O(log V), total O(T log V) where T is token count and V is the number of distinct words.

- In the worst case without shuffling, the BST may degenerate and approach O(T·V); acceptable for chapter‑length inputs but not ideal.


### BST API sketch (raw pointers)

```
class BinSearchTree {
public:
    BinSearchTree() = default;
    ~BinSearchTree(); // calls destroy(root_)

    // Insert 'word'; if present, increment its count.
    void insert(const std::string& word);

    // Convenience: loop over insert(word) for each token.
    void bulkInsert(const std::vector<std::string>& words);

    // Queries
    [[nodiscard]] bool contains(std::string_view word) const noexcept;
    [[nodiscard]] std::optional<int> countOf(std::string_view word) const noexcept;

    // In-order traversal (word-lex order) -> flat list for next stage
    void inorderCollect(std::vector<std::pair<std::string,int>>& out) const;

    // Metrics
    [[nodiscard]] std::size_t size() const noexcept;  // distinct words
    [[nodiscard]] unsigned height() const noexcept;   // empty tree = 0

private:
    // TreeNode is defined elsewhere in the project
    TreeNode* root_ = nullptr;

    // Helpers
    static void destroy(TreeNode* node) noexcept;
    static TreeNode* insertHelper(TreeNode* node, const std::string& word);
    static const TreeNode* findNode(const TreeNode* node, std::string_view word) noexcept;
    static void inorderHelper(const TreeNode* node,
                              std::vector<std::pair<std::string,int>>& out);
    static std::size_t sizeHelper(const TreeNode* node) noexcept;
    static unsigned heightHelper(const TreeNode* node) noexcept;
};
```

# Building the frequency-sorted list (Phase 1 “priority queue”)

After the BST step you have a lexicographically ordered vector of (word, count) pairs. Huffman construction, however, repeatedly needs the two smallest frequencies. In Phase 1, we simulate a priority queue with a single vector kept sorted by frequency (largest → smallest), with a deterministic tie-break. This keeps the two smallest items at the back of the vector so they can be removed in O(1). When we create a combined node, we re-insert it at the correct position to restore the order (O(N) due to shifting).

## Ordering rule (the comparator)

To keep behavior deterministic and grading consistent, we use one global ordering rule:

- Primary key: frequency descending (higher frequency comes earlier).

- Secondary key (tie-break): key_word ascending (lexicographic; the representative word stored in each node).


In short: items are kept sorted by (frequency descending, key_word ascending). This places high-frequency words near the front and low-frequency words near the back. Because the two lowest-frequency items are at the end, removing them is a constant-time pop from the back; inserting the merged parent restores the invariant with an O(N) sorted insert. Here is a comparator for this purpose.

inline bool PriorityQueue::HigherPriority(const TreeNode* a, const TreeNode* b) noexcept {

if (a->freq != b->freq)

return a->freq > b->freq; // higher freq first

return a->key_word < b->key_word; // tie: smaller key_word first

}

## Alternatives Data Structures 

- **Sort once + binary insertion.** Sort the initial leaves (O(V log V)), then use binary search to find each insert position (O(log V) search + O(V) shift). Simple and perfectly adequate here.

- **Balanced-tree multiset.** Maintain a multiset ordered by (frequency ascending, key_word ascending); erase two smallest and insert the parent. Clean interface; logarithmic find/insert/erase.

- **Heap-based min-priority queue.** The classic approach: extract-min twice, then push the parent. We may revisit a heap-based implementation in a later phase (not using `std::priority_queue`).

- **Buckets by frequency.** Only practical if frequencies lie in a very small range (not typical for prose).


See the Huffman Tree section for how this list is structure is used during construction of the Huffman tree. The important point here is the ADT boundary: to support “find-min, delete-min, and insert” efficiently and deterministically, we maintain a single internal ordering rule and preserve it after every operation. In the absence of a Priority Queue, as stated before, we will use a sorted list. Insertion sort is a viable and in this case, fitting algorithm for creating the initial queue, and for adding new nodes to the queue.

#### **Insertion sort algorithm** 

```
Algorithm BuildFrequencySortedList

Input:  freq_lex = list of (word, count) from BST (lexicographic by word)

Output: leaves = list of Leaf(word, key_word=word, freq) sorted by (freq desc, key_word asc)


1. Convert each (word, count) to a Leaf(word, key_word=word, freq=count) and place into array A.

2. For i from 1 to length(A) − 1:

    2.1 item ← A[i]

    2.2 SortedInsertDesc(A, item, i - 1)

3. Return A as leaves
```

```
Predicate HigherPriority(x, y):

  returns true if (x.freq > y.freq) OR (x.freq = y.freq AND x.key_word < y.key_word). 
  Otherwise, return false
```

A reusable sorted insert (for later Huffman steps)

```
Procedure SortedInsertDesc(V, item, idx)

# V is maintained sorted by (freq desc, key_word asc)

1. while idx ≥ 0 and HigherPriority(item, V[idx]) do

     V[idx + 1] ← V[idx]

     idx ← idx − 1

   end while

2. V[idx + 1] ← item
```

**Notes**

- Because the list is kept in descending frequency, the two least-frequent items are at the back; that makes removal of these elements a constant-time operation.

- Insertion sort, in the worst case, runs in O(N^2). Priority queues would provide more efficient implementation for this particular need.

- Keep the comparator in a single, shared definition to avoid subtle “almost-the-same” ordering bugs.


#### **Implementation**

We use a simple, vector-backed, non-owning priority queue. It maintains the ordering invariant (frequency descending, key_word ascending), which places the minimum at the back. The Huffman builder creates all nodes (leaves and internal) and passes raw pointers into and out of the queue during construction; the queue never deletes them. When the build loop finishes (see the Huffman Tree section), the queue will contain one node, the root of the Huffman tree. The Huffman tree class will hold that pointer and owns the entire tree.

PriorityQueue API sketch

```
class PriorityQueue {
public:
    // Non‑owning: does NOT delete the TreeNode* it stores.
    // The constructor takes an initial set of leaves and sorts them internally.
    explicit PriorityQueue(std::vector<TreeNode*> nodes);
    ~PriorityQueue() = default;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    // Min accessors (MIN = items_.back() under our ordering)
    [[nodiscard]] TreeNode* findMin() const noexcept;   // nullptr if empty
    TreeNode* extractMin() noexcept;                    // remove+return min, or nullptr
    void deleteMin() noexcept;                          // remove min if present

    // Insert while maintaining the invariant (O(N) due to shifting)
    // Stores the pointer without taking ownership.
    void insert(TreeNode* node);

    // Debug printing
    void print(std::ostream& os = std::cout) const;

private:
    // Invariant: items_ is kept sorted by HigherPriority(a,b)
    // i.e., (freq desc, key_word asc). Therefore the MIN is items_.back().
    // Ownership: items_ does NOT own the pointers.
    std::vector<TreeNode*> items_;

    static bool higherPriority(const TreeNode* a, const TreeNode* b) noexcept; // a before b?
    bool isSorted() const; // for assertions/tests only
};
```

## Huffman Tree Construction

This stage transforms aggregated word counts into a usable, prefix‑free code. It receives the lexicographically ordered vector of (word, count) pairs produced by the BST stage and produces:

- a Huffman tree whose leaves are words and whose root‑to‑leaf paths define bit codes (left = 0, right = 1), and

- a header file derived from that Huffman tree -- words and their corresponding Huffman codes (codebook)


The Huffman module is also the place that, on request, encodes a stream of tokens using the same Huffman tree and codebook.

#### **Leaves vs. internal nodes, revisited**

- Each leaf stores a word and its count, as produced by the BST. Ultimately, only leaves receive codes and appear in the header.

- Internal nodes are created by merging two children (initially two leaves; later, possibly internal nodes). They do not represent a word. Their purpose is to combine subtrees and carry just enough information for ordering:

    - `freq = left.freq + right.freq` — the sum of all counts in the subtree.

    - `key_word = min(left.key_word, right.key_word)` — a representative word used only by the comparator to break ties deterministically when frequencies are equal.


    Internal nodes are never printed in the header and never receive a code.


#### **Algorithm**

- Start from the BST’s inorder output: a vector of `(word, count)` pairs.

- Create a vector, store one leaf per pair, and use it to initialize the internal, non-owning PriorityQueue.

- Loop while the queue has at least two items:

    - `a ← extractMin()` and `b ← extractMin()` (the two items with the smallest `(freq, key_word)`).

    - Create `parent` with `freq = a.freq + b.freq` and `key_word = min(a.key_word, b.key_word)`.

    - Attach `a` as the left child and `b` as the right child for determinism.

    - Insert `parent` back into the queue (this restores the order in the queue).

- When one item remains, that pointer is the root of the Huffman tree.


During or even after the construction, no code strings are stored in nodes. The Huffman tree implicitly defines the code: every left edge stands for `0` and every right edge stands for `1`. A word’s code is the sequence of edge labels on the path from the root to that word’s leaf. You only materialize a code when you traverse the finished Huffman tree while carrying a running `prefix` of `0`/`1.`

#### Complexity

- findMin/deleteMin/extractMin: O(1)
- insert back into the queue: O(N) (shifts to keep the vector sorted)
- the above two operations are executed N - 1 time, (N is the number of words
- Therefore, the construction of the tree runs in O(N^2)

# Code generation (assigning bitstrings and writing the header)

**Why we build the Huffman tree.**  
It’s a compact recipe for an optimal variable-length code tailored to the input file’s token frequencies. **Leaves** are the actual words; **internal nodes** don’t represent words—they combine subtrees and carry (a) a summed frequency and (b) a `key_word` used only for deterministic tie-breaking. Frequent words end up nearer the root (shorter paths → shorter codes); rarer words end up deeper (longer paths → longer codes). Among all prefix-free binary codes determined solely by these frequencies, the Huffman tree minimizes the expected bits per word (much like Morse code gives shorter patterns to common symbols).

**How the tree becomes a code**  
Implicitly, label every left edge `0` and every right edge `1`. A word’s code is the sequence of these labels on the path from the root to its leaf. During construction, no code strings are stored; you materialize codes only when you traverse the finished tree while carrying a running prefix of `0/1`. At that point, build a codebook (e.g., `std::map<std::string,std::string>` or `std::unordered_map<std::string,std::string>`) of `(word → code)` for later lookups.

**Why prefix-free matters.**  
Because no code is a prefix of another, a decoder can read the bitstream greedily—start at the root, follow `0/1` until a leaf is reached, emit that word, reset to the root, and continue. No separators are needed between codes.

**Determinism for consistency.**

- Visit left before right when assigning codes.

- If there is only one distinct word, assign it the code `"0"`.


**Emitting the header.**  
Perform a pre-order traversal of the Huffman tree and, when you reach a leaf, write a line `word<space>code` (single ASCII space) to the header. Do not sort after traversal.

**Mental model.**  
The tree _is_ the code; the codebook is the tree written down; the bitstream is the original token sequence translated through that code.

#### **Generic Traversal of the Huffman tree**

#### // Visit all Huffman leaves in pre-order, carrying the implicit code as `prefix`.

#### //  `emit` is either a vector into which the (word, code) gets stored or

#### // it is an output stream for writing the code (header uses this choice.)

####   
Procedure VisitLeavesWithCodes(node, prefix, emit):  
  if node is a leaf:  
    code ← (prefix is empty) ? "0" : prefix   # single-word edge case  
    emit(node.word, code)  
  else:  
    VisitLeavesWithCodes(node.left,  prefix + "0", emit)   # left = 0  
    VisitLeavesWithCodes(node.right, prefix + "1", emit)   # right = 1

#### **Example output when emit is output stream (.code file uses this format):**

```
about 010
camp's 1110
fire 00
wolf 1111
...
```

#### **Huffman Implementation suggestion** 

```
class HuffmanTree {
public:
    // Build from BST output (lexicographic vector of (word, count)).
    static HuffmanTree buildFromCounts(const std::vector<std::pair<std::string,int>>& counts);

    HuffmanTree() = default;
    ~HuffmanTree();                         // deletes the entire Huffman tree

    // Build a vector of (word, code) pairs by traversing the Huffman tree
    // (left=0, right=1; visit left before right). 
    void assignCodes(std::vector<std::pair<std::string,std::string>>& out) const;

    // Header writer (pre-order over leaves; "word<space>code"; newline at end).
    error_type writeHeader(std::ostream& os) const;

    // Encode a sequence of tokens using the codebook derived from this tree.
    // Writes ASCII '0'/'1' and wraps lines to wrap_cols (80 by default).
    error_type encode(const std::vector<std::string>& tokens,
                      std::ostream& os_bits,
                      int wrap_cols = 80) const;

private:
    TreeNode* root_ = nullptr; // owns the full Huffman tree

    // helpers (decl only; defs in .cpp)
    static void destroy(TreeNode* n) noexcept;
    static void assignCodesDFS(const TreeNode* n,
                               std::string& prefix,
                               std::vector<std::pair<std::string,std::string>>& out);
    static void writeHeaderPreorder(const TreeNode* n, std::ostream& os,
                                    std::string& prefix);
};
```

**Notes on this implementation**

1. **`assignCodes` (public API).**  
   Traverses the Huffman tree (left = `0`, right = `1`, visit left before right) and builds a vector of `(word, code)` pairs. Codes are not stored in nodes; they’re materialized during traversal. Please see assignCode algorithm above. 
    
2. **`writeHeader(std::ostream&)`.**  
                                                                                                                                                                                                                                             Emits the header by visiting leaves in pre-order and writing lines as _`word<space>code`_, each terminated with a newline. It may internally call `assignCodes` and then stream the pairs, or write lines directly during traversal. If the function Returns `error_type`.

3. **`encode(const std::vector<std::string>&, std::ostream&, int wrap_cols = 80)`.**  
   Given a sequence of tokens, looks up each token’s code (e.g., from the vector/map produced by VisitLeavesWithCodes) and writes an ASCII `'0'`/`'1'` bitstream to the output stream, wrapping at exactly 80 characters per line (except the last) and ending with a final newline. Returns `error_type`; if any token has no code, report failure.


# Encoding words (message)

When the encode function is called, first build a codebook by traversing the Huffman tree once. Create an std::map<std::string, std::string> named codes and call VisitLeavesWithCodes to insert each (word, code) pair into the map. This converts the implicit codes into an explicit structure you can use for efficient lookups.

Next, iterate through each word (token), look it up in codes, and write its code to the output stream. Wrap the output to exactly 80 characters per line (except possibly the last). Always end the file with a final newline, including when the last line is partial.

#### **Example .code file format**

ASCII text containing only the characters 0 and 1 and new-line characters.

```
0010110010...    (80 chars)
0010111110010... (80 chars)
...              (80 chars)
...0101          (last line may be < 80)
```

**Ownership and lifetimes**

- The Huffman module creates all nodes and owns the final Huffman tree (e.g., its destructor deletes the entire structure). The PriorityQueue is non‑owning, used only to order pointers during construction, and is empty when the build finishes.


**Common pitfalls (and how to avoid them)**

- Missing codes during encoding: every token you encode must exist in the codebook. If you find a token without a code, that indicates a mismatch between tokenization and codebook construction—stop and report an error.

- Tabs or extra spaces in .hdr: use exactly one ASCII space between word and code, no leading/trailing spaces, and end the file with a newline.

- Forgetting to reset at 80 columns: keep a running column counter; always write a final newline even if the last line is short.

- Sorting the header after traversal. Just add them to the vector as you reach them


**Error handling**

- The header and encoder functions return error_type. When writing to an std::ostream:

    - if the stream is in a bad state before writing, return UNABLE_TO_OPEN_FILE_FOR_WRITING if it originated from a file stream, or FAILED_TO_WRITE_FILE otherwise;

    - after writing, if os.fail() is true, return FAILED_TO_WRITE_FILE; otherwise return NO_ERROR.


# **Input/Output**

- Create a directory named input_output at your project root.

- Place your input .txt files inside input_output.

- Run your Part 1 encoder with exactly one command-line argument: the path to the input file. If the argument is missing or extra arguments are present, print a short usage message to stderr and exit with a nonzero status.

- Write all derived outputs back into the same input_output directory using the base-name convention below.

- If the directory or file does not exist, print a clear error message and exit with a nonzero status.


### **Project layout example**

project/  
  input_output/  
    call_of_the_wild_ch01.txt

### **Example run**

./huffman_part1 input_output/call_of_the_wild_ch01.txt

### **Output files created**

input_output/  
  call_of_the_wild_ch01.tokens  
  call_of_the_wild_ch01.freq  
  call_of_the_wild_ch01.hdr  
  call_of_the_wild_ch01.code  
  (optional) call_of_the_wild_ch01.bits.count

## **I/O rules**

- All files are ASCII text and end with a newline.

- Print error messages to **stderr** and exit with a **nonzero** status on failure.

- The .code file must be wrapped at exactly 80 characters per line (except the last line).


### **File naming conventions**

#### **Input**

- The input is the name of the text file to be encoded. It ends with .txt and is placed in input_output directory.

- The base name is the file name with the final .txt removed (do not strip other dots).


#### **Derived outputs (same directory)**

Using call_of_the_wild_ch01.txt as an example.

**call_of_the_wild_ch01.tokens** will contain the words as they are identified in the input file.

wolf  
camp's  
fire

**call_of_the_wild_ch01.freq** will contain pairs of (freq word), sorted by (freq desc, key_word asc):

**42 wolf  
         7 camp's  
         3 fire**

- notes: use iomanip’s std::setw(10) to write the right-justified frequency, followed by a single space, then the word, then a newline.


**call_of_the_wild_ch01.hdr**: emit leaves in pre-order (left before right), each line as word<space>code. 

fire 00  
camp's 01  
wolf 1

**call_of_the_wild_ch01.code**: ASCII text containing only '0' and '1'; each line is exactly 80 characters long (except possibly the last), followed by a final newline.

0010110010...    (80 chars)  
0010111110010... (80 chars)  
...              (80 chars)  
...0101          (last line may be < 80)

#### **Optional facts written to standard output**

- **Total tokens (words)** identified in the input

- The number of distinct words

- The height of the binary-search tree

- The height of the Huffman tree

- The sum of the number of letters in the input words

- The total number of bits in the encoded words

# **Part 1 — Scanner**

[The project statement](https://canvas.sonoma.edu/courses/47115/assignments/556736 "Project 3 -- Huffman Trees and Coding") is the source of truth: it fully specifies tokenization rules, file naming, CLI usage, and output formats. We’re dividing the work into parts to manage complexity and to enable unit testing as you go. Part 1 focuses only on building the scanner and producing the .tokens file; later parts will add the BST, the frequency-sorted list, and the Huffman tree.

## **Goal**

Implement the scanner that reads one .txt input and writes the token list <base>.tokens (see _Tokenization_ and _Input/Output_ for exact behavior and format).

## **What to do**

- Follow the Tokenization section precisely to extract words.

- Follow Input/Output for CLI usage, file naming, and newline conventions.

- Your program should:

    - take exactly one input path to a .txt file,

    - write one token per line to input_output/<base>.tokens,

    - handle errors as specified (print to stderr, exit nonzero).



# **Starter CLion project & utilities**

To keep everyone on the same page, [here is a complete CLion project](https://canvas.sonoma.edu/courses/47115/files/4924744?wrap=1 "project3_part1_student_files.zip")[](https://canvas.sonoma.edu/courses/47115/files/4924744/download?download_frd=1)

[Download here is a complete CLion project](https://canvas.sonoma.edu/courses/47115/files/4924744/download?download_frd=1) that already:

- builds and runs when you give it a filename argument,

- checks that the input_output directory exists,

- checks that the input file exists,

- checks that the corresponding output file can be created.


**What’s included**

- utils.hpp — defines the error codes used in this project (see syllabus/spec) and declares helper functions for file/directory checks and error reporting.

- utils.cpp — implements those helpers, including a function that, given an error code, prints an appropriate message and exits with a non-zero status.

- a minimal main.cpp wired for Part 1’s CLI contract (exactly one argument) and stub calls.

- The definition of class Scanner (Scanner.hpp and Scanner.cpp). Scanner.cpp has enough to make the compiler happy and compile the code. 

## **How you’ll test**

- We’ll provide sample inputs and expected outputs on **blue**.

- Use the _Testing on blue_ scripts (next section) to compile/run and compare. You can do this manually by using the diff command like so:


diff -u input_output/<base>.tokens see-path-in-the-next-section/<base>.tokens

- - When files are identical, diff doesn't print anything. Otherwise, it attempts to tell you where the two files diff. If you are not familiar with diff, please look at the manual pages on blue by running this command at the shell prompt.


man diff

## **What to submit**

Upload a zip of your entire project directory that includes:

- All source files needed to build and run Part 1,

- Your input_output/ directory already created. It is okay if it is empty.

- A README at the project root that follows the course syllabus instructions.


We will build your implementation with a command like: g++ -std=c++20 -Wall *.cpp -o huffman_part1 in your project directory and run it using:

./huffman_part1 input_file_name.text

and compare your output to ours using the Linux/Unix _diff_ command. 

# **Testing on blue (Part 1 — Scanner)**

Follow these steps on blue to compile, run, and compare your Part 1 (.tokens) output against the reference.

#### **One-time setup in your project directory**

1. Copy the scripts into your project directory:


cp /home/faculty/kooshesh/cs315_f2025_p3_part1/compile_and_test.bash .  
cp /home/faculty/kooshesh/cs315_f2025_p3_part1/copy_files.bash .

2. **Copy the sample input files**


bash copy_files.bash

This places the sample input files in input_output directory of the directory in which you run this script.

## **Compile, run, and compare**

3. Run the test script on one filename that exists in your input_output/ directory:


bash compile_and_test.bash name_of_one_of_the_files_in_input_output_directory.txt

Example:

bash compile_and_test.bash call_of_the_wild.txt

**What the script does:**

- **Compiles** your code (with g++ -std=c++20 -Wall …).

- **Runs** your program on input_output/<file>.txt.

- **Compares** your generated input_output/<base>.tokens to the reference in:


/home/faculty/kooshesh/cs315_f2025_p3_part1/part1_tokens_files/

Prints any differences (if files don’t match). If your output is not correct, this could generate a lot of output depending on the size of the input file.

## Huffman Tree — Part 2

Part 2 continues the main Huffman project. You will extend your Part 1 program so that, given an input text file, it:

1. tokenizes the file with your Scanner (Part 1),

2. builds a binary-search tree (BST) over words to compute frequencies, and

3. constructs a simulated priority queue using a vector that you sort by frequency.


This phase does not perform encoding. Its goal is to produce clean, verifiable artifacts and on-screen measurements that will be used as the basis for the next and final phase. 

### Goals

1. Reinforce module boundaries among Scanner, BST, and PriorityQueue

2. Build a frequency BST from tokens for O(log n) average updates

3. Implement the “priority queue” using a vector and sorting (no heap yet)

4. Produce deterministic, reproducible outputs for automated testing

5. Report BST measures (height, counts) to standard output

6. Implement all functions in the public interface of each provided class

### What you will build

Your main program pulls together three modules in this order:

1. Scanner: read input file, produce a vector of tokens (words)

2. BST: consume the vector of tokens, count frequencies, and return a vector of (word, count) pairs

3. PriorityQueue (vector+sort): consume that vector and sort it by descending count with a deterministic tie-breaker


Implementation requirement

You must use the class definitions provided for this assignment. Implement all functions listed in the public interface of each class, even if a few are not directly used by your main program. This ensures your code can be compiled and linked with an instructor-provided test main that may call additional functions (such as findMin, extractMin, etc.). You should not remove or rename any functions from the provided interfaces.

### I/O and file conventions

The instructions for the naming of the output files and where you should store them has been provided to you in the statement of the project. 

Running your program like this: 

```
./huff_phase2 input_output/someFile.txt
```

Should generate the following two files. You also have been asked to write a few statistics to the standard output. See below. 

• tokens: input base + .tokens  
• frequencies: input base + .freq

If an output file already exists, overwrite it.

### Module responsibilities and required outputs

Please refer to the statement of the project for details. That is your ultimate source of information for this project. 

#### **Scanner (re-use from Part 1)**

• Tokenizes the input and provides a vector of the words that it identifies.

#### **BST (new for Part 2)**

• Input: vector tokens  
• Returns to the caller a vector<pair<string, size_t>> of (word, count) pairs.   
• Output files: none (BST does not write a file in Part 2).  
• Required stdout measures (print to standard output, not a file). Print exactly these lines and labels in this order:

```
BST height: H
BST unique words: U
Total tokens: T
Min frequency: MIN
Max frequency: MAX
```

Here, H is the height of the BST; U is the number of distinct words, T is the size of the original token vector, and MIN/MAX are taken over the frequencies. If the tree is empty, define height as 0 and set U, T, MIN, MAX to 0 as well.

#### **PriorityQueue (vector+sort)**

• Input: vector<pair<string, size_t>> that the BST provides

• Internal behavior: store entries as (word, count). When “building the queue,” sort the vector as described in the statement of the project. 

       • Output (file): .freq. See the statement of the project for file format

#### **Program flow**

```
int main(int argc, char* argv[]) {
  // 1) parse/validate args; open input file; print clear error message and exit non-zero if needed
  // 2) Scanner: read file -> vector<string> tokens; also write .tokens
  // 3) BST: insert tokens -> build tree; obtain vector<pair<string,size_t>> frequencies (lexicographically by word)
  // 4) Print BST measures to stdout (exact labels/lines above)
  // 5) PriorityQueue: build from frequency vector; sort (count desc, word asc); write .freq
  // 6) return 0 on success; non-zero on failure
}
```

Error handling

• Test to make sure that the input file can be opened (it is expected to be in input_output) and the output files can be written in input_ouput.  
• Empty input files are allowed: produce empty .tokens and .freq, and print the five BST measures as follows:

```
BST height: 0
BST unique words: 0
Total tokens: 0
Min frequency: 0
Max frequency: 0
```

Hints and expectations

• Keep modules independent. Scanner knows nothing about BST; BST knows nothing about the queue’s sort; PriorityQueue only knows its comparator.

• Unit-test the comparator with hand-crafted cases to validate the tie-breaker.

Example (tiny)

Input

```
to be or not to be
```

BST measures (the words have not been randomized)

```
BST height: 4
BST unique words: 4
Total tokens: 6
Min frequency: 1
Max frequency: 2
```

Submission

Submit your complete CLion project (or prescribed structure) that builds and runs with:

```
./huff_phase2 path/to/input.txt
```

Your submission must generate two artifacts:

• .tokens  
• .freq

and print the five BST measures to standard output exactly as specified.

If the main project statement mandates different filenames or an empty-tree height of −1, adjust the implementation to match and update the comment in code accordingly.

## **What to submit**

Upload a zip of your entire project directory that includes:

- All source files needed to build and run Part 2,

- Your input_output/ directory already created. It is okay if it is empty.

- A README at the project root that follows the course syllabus instructions.


We will build your implementation with a command like: g++ -std=c++20 -Wall *.cpp -o huffman_part1 in your project directory and run it using:

./huffman_part1 input_file_name.text

and compare your output to ours using the Linux/Unix _diff_ command. 

I will provide a script for testing your solution this Monday.




# Project 3 -- Full-project scripts

Below are the instructions for copying and testing your final implementation of Project 3. These steps will help you verify that your solution compiles and runs correctly on blue before submission. All commands should be executed on the blue server and from your project directory, where your `.cpp` files and `input_output` directory are located.

### Step 1: Copy the scripts to your project directory 

cp /home/faculty/kooshesh/cs315_fall2025/project3/final_version/copy_files.bash .  
cp /home/faculty/kooshesh/cs315_fall2025/project3/final_version/compile_and_test_project3_final_version.bash .

### Step 2: Run the copy script

This will copy the input files from my directory into your `input_output` directory.

bash ./copy_files.bash

### Step 3: Compile and test your program

Run the test script to compile and execute your program using the input files now located in your `input_output` directory.

bash ./compile_and_test_project3_final_version.bash

### Important Notes

- You should copy these scripts into your _project directory_, where your `.cpp` files and the `input_output` directory are located, and run them from there.

- Your program should expect a _single argument_ — the name of the input file.

- The input file name should _not_ include its path. Your `main` function should prepend `input_output/` to it.


**Example:**  
In your project directory, the script would run:

g++ -std=c++20 *.cpp -Wall -o p3_complete.x  
./p3_complete TheBells.txt

`TheBells.txt` should be in the `input_output` directory (which it would be if you have run the copy script), and all output files should also be written to that directory.

After running your executable with `TheBells.txt` as an argument, the script will compare the following files in your `input_output` directory with the reference versions:

- `TheBells.tokens`

- `TheBells.freq`

- `TheBells.hdr`

- `TheBells.code`


It will display any differences found.

---

### Phase 2 Reminder

For Part 2 of this project, please follow the testing instructions specific to that phase. I will run the Part 2 test script in your project directory to grade that portion. If you have submitted a solution for that phase and your output matches the expected results, you will receive full credit for that phase.