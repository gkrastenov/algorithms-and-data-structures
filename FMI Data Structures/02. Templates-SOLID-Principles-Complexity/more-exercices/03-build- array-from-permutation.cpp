// ////////////////////////////////////////////////////////
// # Title
// Build Array from Permutation
//
// # URL
// https://leetcode.com/problems/build-array-from-permutation/
//
// # Problem
// Given a zero-based permutation nums (0-indexed), build an array ans of the same length 
// where ans[i] = nums[nums[i]] for each 0 <= i < nums.length and return it.
// A zero - based permutation nums is an array of distinct integers from 0 to nums.length - 1 (inclusive).

#include <iostream>
#include <vector>

vector<int> buildArray(const vector<int>& nums)
{
    vector<int> vec(nums.size());
    for (int i = 0; i < nums.size(); ++i)
        vec[i] = nums[nums[i]];
    return vec;
}

int main()
{
    return 0;
}