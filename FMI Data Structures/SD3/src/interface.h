#include <iostream>
#include <set>
#include <string>
#include <queue>
class AVLTree
{
private:
    struct node
    {
        std::string key;
        node* left, * right;
        size_t count;
        int height;

        node(const std::string& key, node* left = nullptr, node* right = nullptr,
            const int height = 0, const size_t count = 1)
            : key(key)
            , left(left)
            , right(right)
            , height(height)
            , count(count)

        {};
    } *root;
    size_t size;

public:
    AVLTree() : root(nullptr), size(0) {};
    ~AVLTree() { clear(root); }

    AVLTree(const AVLTree& tree)
        : root(copy(tree.root))
        , size(tree.size) {};

    AVLTree& operator=(const AVLTree& tree)
    {
        if (&tree != this) {
            clear(root);
            root = copy(tree.root);
            size = tree.size;
        }
        return *this;
    }

    AVLTree& operator=(AVLTree&& rhs) noexcept
    {
        if (this != &rhs) {
            std::swap(root, rhs.root);
            std::swap(size, rhs.size);
        }
        return *this;
    }

    size_t getSize() const { return size; }
    size_t getCountOfNode(const std::string& key) const;
   
    void insert(const std::string& key);
    bool search(const std::string& key) const;
    bool find_and_increase(const std::string& key, const size_t times);
    std::multiset<std::string> print() const;
    void delete_key(const std::string& key);
private:
    // Recursive function to delete a node 
    // with given key from subtree with 
    // given root. It returns root of the 
    // modified subtree. 
    node* delete_util(node* treeRoot, const std::string& key);

    node* search_util(node* head, const std::string& key) const;

    node* insert_util(node* head, const std::string& key);

    int height(node* tree) const;

    node* right_rotate(node* current);

    node* left_rotate(node* current);

    void clear(node* treeRoot);

    node* copy(node* treeRoot);

    // Get Balance factor of node N 
    int getBalance(node* N) const;

    node* find_minimal_value(node* n);
};


///
/// Represents a multiset of words
///
/// If you need to see how it is intended to be used,
/// check out the corresponding unit tests
///
/// Implement all methods of this class
/// 
class WordsMultiset {
public:
	AVLTree tree{};

public:
    WordsMultiset() {}
    ~WordsMultiset() {};

	/// Adds times occurences of word to the container
	///
	/// For example, add("abc") adds the word "abc" once,
	/// while add("abc", 4) adds 4 occurrances.
	void add(const std::string& word, size_t times = 1);

	/// Checks whether word is contained in the container
	bool contains(const std::string& word) const;

	/// Number of occurrances of word 
	size_t countOf(const std::string& word) const;

	/// Number of unique words in the container
	size_t countOfUniqueWords() const;

	/// Returns a multiset of all words in the container
	std::multiset<std::string> words() const;

	// You can add additional members if you need to
};

///
/// Results of the comparison of two streams of words
/// DO NOT modify this class
/// If you need to see how it is intended to be used,
/// check out the corresponding unit tests
///
class ComparisonReport {
public:
	/// A multiset of all words that exist in both streams
	WordsMultiset commonWords;

	/// Multisets of words unique to the two streams
	/// The first element of the array contains the words that are unique
	/// to the first stream (a) and the second one -- to the second stream (b)
	WordsMultiset uniqueWords[2];
};

/// 
/// Can be used to compare two streams of words
///
class Comparator {
public:
	ComparisonReport compare(std::istream& a, std::istream& b);

	// You can add additional members if you need to
};
