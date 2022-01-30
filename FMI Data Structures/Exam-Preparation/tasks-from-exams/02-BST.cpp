#include <string>
#include <queue>
#include <fstream>
#include <iostream>
using namespace std;

struct Node {
public:
    int data{};
    Node* left{};
    Node* right{};

    Node() = default;
    Node(const int data, Node* left = nullptr, Node* right = nullptr)
        : data(data),
        left(nullptr),
        right(nullptr) {};

    ~Node() = default;
};

class Tree {
public:
    Node* root{};
    Tree()
    {
            
    }
    void insert(const int data)
    {
        root = insert_util(root, data);
    }
    int countSubTree(const int N)
    {
       int count = 0;
       countSubTree_util(root, count, N, 0);
       return count;
    }
    Node* findNode(const int K)
    {
       return findNode_util(root, K);
    }
private:
    Node* insert_util(Node* nod, const int data)
    {
        if (nod == nullptr)
        {
            nod = new Node(data);
            return nod;
        }

        if (nod->data == data)
            return nod;

        if (nod->data > data)
            nod->left = insert_util(nod->left, data);
        else nod->right= insert_util(nod->right, data);

        return nod;
    }

    int countSubTree_util(Node* nod, int& count, const int N, int sum = 0)
    {
        if (nod == nullptr)
            return 0;
       
        if (nod->left == nullptr && nod->right == nullptr)
            return 1;
        sum += countSubTree_util(nod->left, count, N, 0);
        sum += countSubTree_util(nod->right, count, N, 0);

        if (N == sum)
            ++count;

        sum += 1;
        return sum;
    }

    Node* findNode_util(Node* nod, const int K)
    {
        if (nod->data == K)      
            return nod;
        
        if (nod->data > K)
            nod->left = findNode_util(nod->left, K);
        else nod->right = findNode_util(nod->right, K);

        return nullptr;
    }
};
void makeTree(Tree& tree, std::istream& stream)
{
    string intFromFile;
    while (stream >> intFromFile)
    {
        // TODO: add exeption safety
        int element = stoi(intFromFile);
        tree.insert(element);
    }
}
int countSubTreeByGivenValue(Tree& tree, const int N)
{
    return tree.countSubTree(N);
}
void deleteNodeByK(Tree& tree, const int K)
{
    auto founded = tree.findNode(K);
    if (founded == nullptr)
        return;

    queue<Node*> que;

    vector<int> toBeDeleted;
    toBeDeleted.push_back(founded->data);

    que.push(founded->left);
    que.push(founded->right);
    que.push(NULL);

    int level = 1;
    while (!que.empty())
    {
        auto curr = que.front();
        que.pop();
        if (curr == NULL)
        {
            level++;
            if (que.size() == 0)
                continue;
            que.push(NULL);
        }
        else {
            if (level % 2 == 0)
                toBeDeleted.push_back(curr->data);

            if(curr->left != nullptr)
                que.push(curr->left);

            if (curr->right != nullptr)
                que.push(curr->right);
        }         
    }
        
    for (size_t i = 0; i < toBeDeleted.size(); i++)
    {
        tree.root = deleteNode(tree.root, toBeDeleted[i]);
    }
}
Node* deleteNode(Node* root, int key)
{
    if (root)
        if (key < root->data) 
            root->left = deleteNode(root->left, key);     //We frecursively call the function until we find the target node
        else if (key > root->data) root->right = deleteNode(root->right, key);
        else {
            if (!root->left && !root->right) return NULL;          //No child condition
            if (!root->left || !root->right)
                return root->left ? root->left : root->right;    //One child contion -> replace the node with it's child
                                                                //Two child condition   
            auto temp = root->left;                        //(or) TreeNode *temp = root->right;
            while (temp->right != NULL) temp = temp->right;     //      while(temp->left != NULL) temp = temp->left;
            root->data = temp->data;                            //       root->val = temp->val;
            root->left = deleteNode(root->left, temp->data);  //        root->right = deleteNode(root->right, temp);		
        }
    return root;
}
int main()
{
    Tree tree;

    string fileName;
    std::getline(std::cin, fileName);

    std::ifstream file(fileName);

    if (file.is_open()) {
        makeTree(tree, file);
    }
    file.close();

    int N;
    cin >> N;
    int numberOfSubTree = countSubTreeByGivenValue(tree, N);

    int K;
    cin >> K;
    deleteNodeByK(tree, K);
}