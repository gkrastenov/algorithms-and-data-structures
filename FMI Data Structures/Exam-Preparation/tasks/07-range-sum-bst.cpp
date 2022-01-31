
  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
  };

class Solution {
public:
     void rangeSumBST_helper(TreeNode* root, int low, int high, int& count) {
        if (root == nullptr)
            return;
        if (root->val >= low && root->val <= high)
            count += root->val;

        rangeSumBST_helper(root->left, low, high, count);
        rangeSumBST_helper(root->right, low, high, count);
    return ;

    }
    int rangeSumBST(TreeNode* root, int low, int high) {
        int count = 0;
        rangeSumBST_helper(root, low, high, count);
        return count;
    }
};