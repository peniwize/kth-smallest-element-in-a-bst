/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/kth-smallest-element-in-a-bst/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <cassert>
#include <chrono>
#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <iterator>
#include <memory>
#include <queue>
#include <vector>
#include <set>
#include <span>

using namespace std;

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    virtual ~TreeNode() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
        }
    }
};

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    /*
        Time = O(2n) => O(n); O(log2(n)) for balanced tree.
        Space = O(log2(n))
    */
    int kthSmallestInorderSearch(TreeNode* root, int& k) {
        if (root && 0 < k) {
            auto result = kthSmallestInorderSearch(root->left, k);
            if (0 < k) {
                result = root->val;
                --k;
                if (0 < k) {
                    result = kthSmallestInorderSearch(root->right, k);
                }
            }
            return result;
        } else {
            return 0;
        }
    }

    int kthSmallest(TreeNode* root, int k) {
        return kthSmallestInorderSearch(root, k);
    }
};

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct elapsed_time_t
{
    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point end{};
    
    elapsed_time_t(
        std::chrono::steady_clock::time_point start
        , std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now()
    ) : start{std::move(start)}, end{std::move(end)} {}
};

std::ostream&
operator<<(std::ostream& os, elapsed_time_t const& et)
{
    auto const elapsed_time = et.end - et.start;
    os << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count()
       << '.' << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() % 1000)
       << " ms";
    return os;
}

TreeNode* createBst(std::vector<int> values) {
    TreeNode* root = nullptr;

    for (auto value : values) {
        // Search for parent node, if any.
        TreeNode* parent{};
        for (TreeNode* iter = root; iter; ) {
            parent = iter;
            iter = value < iter->val ? iter->left : iter->right;
        }
        
        // Find parent pointer.
        TreeNode** parentPtr{};
        if (parent) {
            if (value < parent->val) {
                parentPtr = &parent->left;
            } else {
                parentPtr = &parent->right;
            }
        } else {
            parentPtr = &root;
        }
        
        // Allocate new node.
        if (parentPtr) {
            *parentPtr = new TreeNode{std::move(value)};

            if (!root) {
                root = *parentPtr;
            }
        }
    }

    return root;
}

std::vector<int> toVector(TreeNode const* root) {
    std::vector<int> result{};

    // In order depth first search.
    if (root) {
        auto const leftResult = toVector(root->left);
        std::copy(leftResult.begin(), leftResult.end(), std::back_inserter(result));

        result.push_back(root->val);

        auto const rightResult = toVector(root->right);
        std::copy(rightResult.begin(), rightResult.end(), std::back_inserter(result));
    }

    return result;
}

TEST_CASE("Case 1")
{
    cerr << "Case 1" << '\n';
    std::vector<int> const values{3,1,4,2};
    auto root = std::unique_ptr<TreeNode>{createBst(values)};
    auto const k = 1;
    auto const expected = [&]{
        auto result{values};
        std::sort(result.begin(), result.end());
        assert(0 < k && result.size() >= k);
        return result[k - 1];
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = Solution{}.kthSmallest(root.get(), k);
        CHECK(expected == result);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    std::vector<int> const values{5,3,6,2,4,1};
    auto root = std::unique_ptr<TreeNode>{createBst(values)};
    auto const k = 3;
    auto const expected = [&]{
        auto result{values};
        std::sort(result.begin(), result.end());
        assert(0 < k && result.size() >= k);
        return result[k - 1];
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = Solution{}.kthSmallest(root.get(), k);
        CHECK(expected == result);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 10")
{
    cerr << "Case 10" << '\n';
    std::vector<int> const values{0};
    auto root = std::unique_ptr<TreeNode>{createBst(values)};
    auto const k = 1;
    auto const expected = [&]{
        auto result{values};
        std::sort(result.begin(), result.end());
        assert(0 < k && result.size() >= k);
        return result[k - 1];
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = Solution{}.kthSmallest(root.get(), k);
        CHECK(expected == result);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
