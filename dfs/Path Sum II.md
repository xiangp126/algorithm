## Path Sum II
### Illustrate
<https://leetcode.com/problems/path-sum-ii>

Given a binary tree and a sum, find all **root-to-leaf** paths where each path's sum equals the given sum.

Note: A leaf is a node with no children.

### Example

```c
Given the below binary tree and sum = 22,

      5
     / \
    4   8
   /   / \
  11  13  4
 /  \    / \
7    2  5   1
Return:

[
   [5,4,11,2],
   [5,8,4,5]
]
```

### Code

_standard Template for `DFS`_

```c
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int> > ret;
        vector<int> path;
        dfs(root, path, ret, sum);
        return ret;
    }

    /*
     * dfs: depth-first search to solve problem
     * @root: denotes the tree
     * @gap: sum gap
     * @path: template path vector
     * @ret: result vector, push path into
     * @return void
     */
    void dfs(TreeNode *root, vector<int> &path, vector<vector<int> > &ret, int gap) {
        // On termination
        if (!root) return;

        path.push_back(root->val);

        // On Matching
        if (root->left == NULL && root->right == NULL && root->val == gap) {
            ret.push_back(path);
        }

        dfs(root->left,  path, ret, gap - root->val);
        dfs(root->right, path, ret, gap - root->val);

        // Backtrack and remove the last element
        path.pop_back();
    }
};
```
