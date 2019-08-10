//implementation of AVL tree
#include<iostream>
#define MAX(x,y) (x > y)? x:y
using namespace std;
class Node
{
public:
	int key;	//key stored in node
	Node *left,*right;	//pointers to left and right subtree
	int height;	//height of node
	Node(int value)
	{
		this->key = value;
		this->left = NULL;
		this->right = NULL;
		this->height = 1;
	}

	int balance_factor()	//computes balance factor of node
	{
		if(this->left && this->right)	//if node has both children
			return (this->left->height - this->right->height);	//return height of left subtree - height of right subtree
		else if(this->left)	//if node has only left subtree
			return this->left->height;
		else if(this->right) //if node has only right subtree
			return -(this->right->height);
		else	//node is a leaf
			return 0;
	}
};

class AVLTree
{
public:
	Node *root;

	AVLTree()
	{
		root = NULL;
	}

	bool is_empty()
	{
		return (this->root == NULL);
	}

	Node * insert(Node *node,int key)
	{
		if(node == NULL)	//if reached end of path
			return new Node(key);	//place new node
		else if(key > node->key)	//if value is supposed to be in right subtree
			node->right = insert(node->right,key);
		else if(key < node->key)	//if value is supposed to be in left subtree
			node->left = insert(node->left,key);
		else	//value already exists
			return node;

		update_height(node);	//update height of the parent of newly inserted node
		int balance_factor = node->balance_factor();	//get balance factor of node
		if(balance_factor > 1) //if tree is skewed towards left
			if(key < node->left->key) //key is inserted in left subtree of left child
				return rotate_right(node);	//perform right right rotation
			else	//key is inserted in right subtree of left child
				return rotate_left_right(node);	//perform left right rotation
		else if(balance_factor < -1)	//tree is skewed towards right
			if(key > node->right->key)	//key is inserted in right subtree of right child
				return rotate_left(node);	//perform left left rotation
			else
				return rotate_right_left(node);	 //perform right left rotation
		else	return node;	//balance factor lies within [-1,1], hence no rotations required
	}

	Node * rotate_left(Node *node)
	{
		Node *new_root = node->right;	//right child of node becomes new root
		node->right = new_root->left;	//old root's right subtree is updated to left subtree of new root
		new_root->left = node;	//old root becomes left child of new root
		update_height(node);
		update_height(new_root);
		return new_root;
	}

	Node * rotate_right(Node *node)
	{
		Node *new_root = node->left;	//left child of node becomes new root
		node->left = new_root->right;	//old root's left subtree is updated to right subtree of new root
		new_root->right = node;	//old root becomes right child of new root
		update_height(node);
		update_height(new_root);
		return new_root;
	}

	Node * rotate_left_right(Node *node)
	{
		node->left = rotate_left(node->left);
		return rotate_right(node);
	}

	Node * rotate_right_left(Node *node)
	{
		node->right = rotate_right(node->right);
		return rotate_left(node);
	}

	int height(Node *node)
	{
		if(node != NULL) return node->height;
		else	return 0;
	}

	void update_height(Node *node)
	{
		node->height = 1 + MAX(height(node->left),height(node->right));
	}

	Node * search(Node *temp,int key)
	{
		if(temp == NULL) return NULL;	//path is over
		else if(temp->key == key) return temp;	//match found
		else if(key > temp->key) return (search(temp->right,key));	//value is probably in right subtree
		else return (search(temp->left,key));	//value is probably in left subtree
	}

	void inorder(Node *temp)
	{
		if(temp != NULL)
		{
			inorder(temp->left);
			cout<<temp->key<<" ";
			inorder(temp->right);
		}
	}

	void preorder(Node *temp)
	{
		if(temp != NULL)
		{
			cout<<temp->key<<" ";
			inorder(temp->left);
			inorder(temp->right);
		}
	}

	void display()
	{
		cout<<"\nInorder traversal of tree:\n";
		inorder(this->root);
		cout<<"\n\nPreorder traversal of tree:\n";
		preorder(this->root);
		cout<<"\n";
	}
};

int main()
{
	int choice,value;
	AVLTree t;
	while(true)
	{
		cout<<"\n\n1> Insert key\n2> Display tree\n3> Search\n";
		cin>>choice;
		switch(choice)
		{
			case 1: cout<<"\nEnter the value > ";
					cin>>value;
					t.root = t.insert(t.root,value);
					cout<<value<<" is inserted\n";
					break;
			case 2: if(t.is_empty()) cout<<"\nTree is empty\n";
					else t.display();
					break;
			case 3: if(t.is_empty()) cout<<"\nTree is empty\n";
					else
					{
						cout<<"\nEnter the key to search > ";
						cin>>value;
						if(t.search(t.root,value) != NULL) cout<<"\nkey found\n";
						else cout<<"\nkey not found\n";
					}
					break;
			default:return 0;
		}
	}
}
