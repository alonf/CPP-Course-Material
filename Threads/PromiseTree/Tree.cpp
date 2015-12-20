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
#include <thread>
#include <future>
#include "Tree.h"

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
			auto leftLeave = make_shared<TreeNode>(TreeNode(1));
			auto rightLeave = make_shared<TreeNode>(TreeNode(1));

			newLeaves.push_back(leftLeave);
			newLeaves.push_back(rightLeave);
			node->SetLeft(leftLeave);
			node->SetRight(rightLeave);
		});
		leaves = newLeaves;
	}
}

int Tree::Sum()
{
	return SubTreeSum(_pRoot);	
}

//should be: void Tree::AddLeftSubTree(promise<int> &&prms, const TreeNode::tree_node_ptr &subTreeRoot)
//but VS 2012/2013 has a bug: http://connect.microsoft.com/VisualStudio/feedback/details/729886
//so we use l-value ref instead

void Tree::AddLeftSubTree(promise<int> &&prms, const TreeNode::tree_node_ptr &subTreeRoot)
{	
	auto leftSubTree = subTreeRoot->GetLeft();
	prms.set_value(leftSubTree ? SubTreeSum(leftSubTree) : 0);
};

void Tree::AddRightSubTree(promise<int> &&prms, const TreeNode::tree_node_ptr &subTreeRoot)
{	
	auto rightSubTree = subTreeRoot->GetRight();
	prms.set_value(rightSubTree ? SubTreeSum(rightSubTree) : 0);
};


int Tree::SubTreeSum(const TreeNode::tree_node_ptr &subTreeRoot)
{
	promise<int> leftPromise;
	promise<int> rightPromise;

	auto leftFuture = leftPromise.get_future();
	auto rightFuture = rightPromise.get_future();
	

	thread leftThread(&Tree::AddLeftSubTree, move(leftPromise), cref(subTreeRoot));
		
	thread rightThread(&Tree::AddRightSubTree, move(rightPromise), cref(subTreeRoot));

	leftThread.join();
	rightThread.join();

	int result = subTreeRoot->GetValue() + leftFuture.get() + rightFuture.get();
	return result;
}

Tree::~Tree(void)
{
}
