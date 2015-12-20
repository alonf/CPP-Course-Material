// ----------------------------------------------------------------------
// <copyright file="Tree.cpp" company="CodeValue">
//     Copyright (c) 2011 by CodeValue Ltd. All rights reserved
// </copyright>
//
// http://codevalue.com
// Licensed under the Educational Community License version 1.0 (http://www.opensource.org/licenses/ecl1)
// This example was written as a demonstration of principles only
//
// ------------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include "Tree.h"
#include <future>
#include <experimental/resumable>

using namespace std;

Tree::Tree(int depth)
{
	vector<TreeNode::tree_node_ptr> leaves;
	
	_pRoot = make_shared<TreeNode>(TreeNode(1));
	leaves.push_back(_pRoot);

	for (int i = 0; i < depth - 1; ++i)
	{
		vector<shared_ptr<TreeNode>> newLeaves;
		for_each(begin(leaves), end(leaves), [&](TreeNode::tree_node_ptr node)
		{
			auto leftLeave = make_shared<TreeNode>(TreeNode(depth));
			auto rightLeave = make_shared<TreeNode>(TreeNode(depth));

			newLeaves.push_back(leftLeave);
			newLeaves.push_back(rightLeave);
			node->SetLeft(leftLeave);
			node->SetRight(rightLeave);
		});
		leaves = newLeaves;
	}
}

future<int> Tree::Sum()
{
	return __await SubTreeSum(_pRoot);	
}

future<int> Tree::SubTreeSum(TreeNode::tree_node_ptr subTreeRoot) 
{
	auto leftSubTree = subTreeRoot->GetLeft();
	int leftSum = leftSubTree ? __await SubTreeSum(leftSubTree) : 0;

	auto rightSubTree = subTreeRoot->GetRight();
	int rightSum = rightSubTree ? __await SubTreeSum(rightSubTree) : 0;

	return subTreeRoot->GetValue() + leftSum + rightSum;
}

Tree::~Tree(void)
{
}
