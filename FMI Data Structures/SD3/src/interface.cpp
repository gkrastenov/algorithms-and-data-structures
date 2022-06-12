#include "interface.h"

void WordsMultiset::add(const std::string& word, size_t times)
{
    if (tree.find_and_increase(word, times)) 
        return;

    for (size_t i = 0; i < times; i++)
    {
        tree.insert(word);
    }
}

bool WordsMultiset::contains(const std::string& word) const
{
    return tree.search(word);
}

size_t WordsMultiset::countOf(const std::string& word) const
{
    return tree.getCountOfNode(word);
}

size_t WordsMultiset::countOfUniqueWords() const
{
    return tree.getSize();
}

std::multiset<std::string> WordsMultiset::words() const
{
    return tree.print();
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
    ComparisonReport report;

    std::string word;
    while (a >> word)
    {
        report.uniqueWords[0].add(word, 1);
    }
    while (b >> word)
    {
        if (report.uniqueWords[0].tree.search(word))
        {
            report.commonWords.tree.insert(word);
            report.uniqueWords[0].tree.delete_key(word);
        }
        else {
            report.uniqueWords[1].add(word, 1);
        }
    }
    
    return report;
}

int AVLTree::height(node* tree) const
{
    if (tree == nullptr)
        return 0;
    return tree->height;
}

AVLTree::node* AVLTree::right_rotate(node* current)
{
    node* left_child = current->left;
    node* right_subtree = left_child->right;

    left_child->right = current;
    current->left = right_subtree;

    current->height = 1 + std::max(height(current->left), height(current->right));
    left_child->height = 1 + std::max(height(left_child->left), height(left_child->right));

    return left_child;
}

AVLTree::node* AVLTree::left_rotate(node* current)
{
    node* right_child = current->right;
    node* left_subtree = right_child->left;

    right_child->left = current;
    current->right = left_subtree;

    current->height = 1 + std::max(height(current->left), height(current->right));
    right_child->height = 1 + std::max(height(right_child->left), height(right_child->right));

    return right_child;
}

void AVLTree::clear(node* treeRoot)
{
    if (treeRoot) {
        clear(treeRoot->left);
        clear(treeRoot->right);
        delete treeRoot;
    }
}

AVLTree::node* AVLTree::copy(node* treeRoot)
{
    return treeRoot ?
        new node(treeRoot->key, copy(treeRoot->left), copy(treeRoot->right), treeRoot->height, treeRoot->count) :
        treeRoot;
}

int AVLTree::getBalance(node* N) const
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

AVLTree::node* AVLTree::find_minimal_value(node* n)
{
    node* curr = n;
    while (curr && curr->left)
        curr = curr->left;
    return curr;
}

size_t AVLTree::getCountOfNode(const std::string& key) const
{
    if (search_util(root, key))
        return search_util(root, key)->count;
    return 0;
}

void AVLTree::insert(const std::string& key)
{
    root = insert_util(root, key);
}

bool AVLTree::search(const std::string& key) const
{
    return search_util(root, key) != nullptr;
}

bool AVLTree::find_and_increase(const std::string& key, const size_t times)
{
    node* founded = search_util(root, key);
    if (founded){
        founded->count += times;
        return true;
    }
    return false;
}

AVLTree::node* AVLTree::delete_util(node* curr, const std::string& key)
{
    if (!curr)
        return nullptr;
    if (curr->key > key)
        curr->left = delete_util(curr->left, key);
    else if(curr->key < key)
        curr->right = delete_util(curr->right, key);
    else {
        if (!curr->left && !curr->right)
            return nullptr;
        else if (!curr->left) {
            node* temp = curr->right;
            delete curr;
            return temp;
        }
        else if (!curr->right) {
            node* temp = curr->left;
            delete curr;
            return temp;
        }
        else {
            node* helper = find_minimal_value(curr->right);
            curr->key = helper->key;
            curr->right = delete_util(curr->right, helper->key);
        }
    }

    curr->height = 1 + std::max(height(curr->left), height(curr->right));

    int balance = getBalance(curr);
    if (balance > 1) {
        int leftBalance = getBalance(curr->left);
        if (leftBalance >= 0)
            return right_rotate(curr);
        else {
            curr->left = left_rotate(curr->left);
            return right_rotate(curr);
        }
    }
    else if (balance < -1) {
        int rightBalance = getBalance(curr->right);
        if (rightBalance > 0)
        {
            curr->right = right_rotate(curr->right);
            return left_rotate(curr->right);
        }
        else {
            return left_rotate(curr);
        }   
    }
    return curr;
}

AVLTree::node* AVLTree::search_util(node* head, const std::string& key) const
{
    if (head == nullptr)
        return nullptr;

    if (head->key < key) 
        return search_util(head->right, key);
    else if (head->key > key)
        return search_util(head->left, key);

    return head;
}

AVLTree::node* AVLTree::insert_util(node* head, const std::string& key)
{
    if (head == nullptr) {
        node* temp = new node(key);
        ++size;
        return temp;
    }

    if (key == head->key) {
        head->count++;
        return head;
    }

    if (head->key < key)    
        head->right = insert_util(head->right, key);   
    else if (head->key > key)
        head->left = insert_util(head->left, key);
    else  return head;

    head->height = 1 + std::max(height(head->left), height(head->right));

    int balance = getBalance(head);
    if (balance > 1)
    {
        int balance_left = getBalance(head->left);
        if (balance_left > 0)
        {
            return right_rotate(head);
        }
        else
        {
            head->left = left_rotate(head->left);
            return right_rotate(head);
        }
    }
    else if (balance < -1)
    {
        int balance_right = getBalance(head->right);
        if (balance_right > 0)
        {
            head->right = right_rotate(head->right);
            return left_rotate(head);
        }
        else
        {
            return left_rotate(head);
        }
    }
    return head;
}

std::multiset<std::string> AVLTree::print() const
{
    std::multiset<std::string> multiset; 
    if (!root)
        return multiset;

    std::queue<node*> queue;
    queue.push(root);
    while (!queue.empty())
    {
        node* curr = queue.front();
        queue.pop();
        if (curr){
            for (size_t i = 0; i < curr->count; i++)
            {
                multiset.insert(curr->key);
            }
        }

        if (curr->left)
            queue.push(curr->left);

        if (curr->right)
            queue.push(curr->right);
    }
    return multiset;
}

void AVLTree::delete_key(const std::string& key)
{
    node* founded = search_util(root, key);
    if (founded->count == 1) {
        root = delete_util(root, key);
        --size;
    }
    else {
        founded->count--;
    }
    
}
