// ----------------------------------------------------------------------
// <copyright file="Tree.h" company="CodeValue">
//     Copyright (c) 2011 by CodeValue Ltd. All rights reserved
// </copyright>
//
// http://codevalue.com
// Licensed under the Educational Community License version 1.0 (http://www.opensource.org/licenses/ecl1)
// This example was written as a demonstration of principles only
//
// ------------------------------------------------------------------------

#pragma once
#include <memory>
#include <future>

using namespace std;

class Tree;

class TreeNode
{
	friend class Tree;
private:
	typedef shared_ptr<TreeNode> tree_node_ptr;
	int _value;
	tree_node_ptr _pLeft;
	tree_node_ptr _pRight;

public:
	TreeNode(int value) : _value(value) {}
	void SetLeft(tree_node_ptr pLeft) { _pLeft = pLeft; }
	tree_node_ptr GetLeft() const { return _pLeft; }
	void SetRight(tree_node_ptr pRight) { _pRight = pRight; }
	tree_node_ptr GetRight() const { return _pRight; }
	int GetValue() const { return _value; }
};

class Tree
{
private:
	TreeNode::tree_node_ptr _pRoot;
	future<int> SubTreeSum(TreeNode::tree_node_ptr subTreeRoot);

public:
	Tree(int depth);
	future<int> Sum();
	~Tree(void);
};

