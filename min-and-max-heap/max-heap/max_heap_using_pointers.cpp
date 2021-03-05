//implementation of max heap
#include<iostream>

#include "node.h"
#include "list.h"
#include "max_heap.h"

list heap;

void init_heap()
{
	heap.head = nullptr;
	heap.last_node = nullptr;
}

void insert(int value)
{
	node *new_node;
	new_node = create_node(value);
	if (heap.head == nullptr) //if heap is empty
		heap.head = new_node;
	else
	{
		heap.last_node->next = new_node; //append node at end of heap
		resolve_relationships(new_node); //resolve parent-child relationships of new node
		sift_up(new_node);				 //checking for heap property
	}
	heap.last_node = new_node;
	std::cout << value << " has been inserted\n";
}

void resolve_relationships(node *new_node)
{
	node *incomplete_node = find_incomplete_node(); //find the first incomplete node in the heap
	if (incomplete_node->left_child == nullptr)
		incomplete_node->left_child = new_node;
	else
		incomplete_node->right_child = new_node;
	new_node->parent = incomplete_node;
}

node *find_incomplete_node()
{
	node *itr;
	for (itr = heap.head; itr != nullptr; itr = itr->next) //search for first incomplete node
	{
		if ((itr->left_child == nullptr) || (itr->right_child == nullptr))
			return itr;
	}
	return nullptr;
}

void sift_up(node *child)
{
	node *parent = child->parent;
	if (parent == nullptr) //reached root
		return;
	if (child->key > parent->key)
	{
		swap(child, parent);
		sift_up(parent);
	}
}

void delete_max()
{
	if (heap.head == nullptr)
	{
		std::cout << "\nheap is empty\n";
		return;
	}
	swap(heap.head, heap.last_node); //swap min element with the last node in heap
	std::cout << heap.last_node->key << " is deleted\n";
	delete_last_node();	   //delete last node in the heap
	if (heap.head != nullptr) //if the heap is not empty
		sift_down(heap.head);
}

void sift_down(node *parent)
{
	node *largest_child = find_largest_child(parent); //find largest child of node
	if (largest_child == nullptr)						  //leaf node
		return;
	if (largest_child->key > parent->key) //if child violates heap property
	{
		swap(largest_child, parent);
		sift_down(largest_child);
	}
}

node *find_largest_child(node *parent)
{
	if (parent->left_child && parent->right_child)															  //if both children exist
		return parent->left_child->key > parent->right_child->key ? parent->left_child : parent->right_child; //return largest child
	else if (parent->left_child)																			  //if only left child exists
		return parent->left_child;
	else if (parent->right_child) //if only right child exist
		return parent->right_child;
	else
		return nullptr; //no children
}

void delete_last_node()
{
	node *itr;
	if (heap.head == heap.last_node) //only one node in heap
	{
		delete heap.head;
		heap.head = heap.last_node = nullptr;
	}
	else
	{
		update_parent_of_last_node(); //remove link from parent of last node to last node
		for (itr = heap.head; itr->next != heap.last_node; itr = itr->next)
			; //find second last node in heap
		delete heap.last_node;
		heap.last_node = itr; //re assign last node to second last node
	}
}

void update_parent_of_last_node()
{
	if (heap.last_node->parent->left_child == heap.last_node) //if last node is left child of its parent
		heap.last_node->parent->left_child = nullptr;
	else
		heap.last_node->parent->right_child = nullptr;
}

node *create_node(int value)
{
	node *new_node = new node;
	new_node->key = value;
	new_node->parent = new_node->left_child = new_node->right_child = new_node->next = nullptr;
	return new_node;
}

void display()
{
	if (heap.head == nullptr)
	{
		std::cout << "\nheap is empty\n";
		return;
	}
	std::cout << "\nin-order traversal of heap is:\n";
	inorder(heap.head);
}

void inorder(node *root)
{
	if (root != nullptr)
	{
		inorder(root->left_child);
		std::cout << root->key << " ";
		inorder(root->right_child);
	}
}

void swap(node *a, node *b)
{
	int temp = a->key;
	a->key = b->key;
	b->key = temp;
}
