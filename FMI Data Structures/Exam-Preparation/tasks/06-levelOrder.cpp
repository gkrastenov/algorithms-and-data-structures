#include <iostream>
#include<queue>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;
    Node(int d) {
        data = d;
        left = NULL;
        right = NULL;
    }
};


void levelOrder(Node * root) {
    queue<Node*> que;
    que.push(root);
    while (!que.empty())
    {
        auto curr = que.front();
        que.pop();

        cout << curr->data;

        if (!curr->left)
            que.push(curr->left);

        if (!curr->right)
            que.push(curr->right);
    }

}