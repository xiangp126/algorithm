## Path Sum III
### Illustrate
<https://leetcode.com/problems/path-sum-iii>

You are given a binary tree in which each node contains an integer value.

Find the number of paths that sum to a given value.

The path does not need to start or end at the root or a leaf, but it must go downwards (traveling only from parent nodes to child nodes).

The tree has no more than 1,000 nodes and the values are in the range -1,000,000 to 1,000,000.

### Example

```c
root = [10,5,-3,3,2,null,11,3,-2,null,1], sum = 8

      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \   \
3  -2   1

Return 3. The paths that sum to 8 are:

1.  5 -> 3
2.  5 -> 2 -> 1
3. -3 -> 11
```

### Code

* The first step is to travel through all the nodes of the tree.
* The second step is to find all the paths starting with the current node.

```c
// correct me
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
    int sumNumbers(TreeNode* root) {
        int ret = 0;
        int subSum = 0;
        dfs(ret, subSum, root);
        return ret;
    }
    void dfs(int &ret, int &subSum, TreeNode *root) {
        if (!root) return;
        int tmp = subSum;
        subSum = subSum* 10 + root->val;
        if (!root->left && !root->right) {
            ret += subSum;
        }
        dfs(ret, subSum, root->left);
        dfs(ret, subSum, root->right);
        subSum = tmp;
    }
};
```