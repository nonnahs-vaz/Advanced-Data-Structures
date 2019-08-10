//implementation of splay tree
#include<iostream>
using namespace std;
class Node
{
public:
    int key;
    Node *left,*right;

    Node(int value)
    {
        key = value;
        left = right = NULL;
    }
};

class SplayTree
{
private:
    Node *root;

    Node * splay(Node *current_root, int key)
    {
        if((current_root == NULL) || (current_root->key == key))    //if subtree is empty or node is found
            return current_root;
        if(key > current_root->key) //if key is probably in right subtree
        {
            if(current_root->right == NULL) //there is no right subtree
                return current_root;
            if(key > current_root->right->key)  //if key is greater than the root of right subtree
            {
                current_root->right->right = splay(current_root->right->right,key); //get the node in the right subtree of the right subtree
                current_root = rotate_left(current_root);   //rotate root left to get node as root of right subtree
            }
            else if(key < current_root->right->key) //if key is less than the root of right subtree
            {
                current_root->right->left = splay(current_root->right->left,key);   // get the node in the left subtree of the right subtree
                if(current_root->right->left != NULL) //if the left subtree of right subtree is not empty
                    current_root->right = rotate_right(current_root->right);    //rotate right subtree towards left to get node as root of right subtree
            }
            if(current_root->right != NULL) //if right subtree of root is not empty
                current_root = rotate_left(current_root);   //rotate root towards left to get node as root
            return current_root;
        }
        else    //key is probably in the left subtree
        {
            if(current_root->left == NULL)  //there is no left subtree
                return current_root;
            if(key > current_root->left->key)  //if key is greater than root of left subtree
            {
                current_root->left->right = splay(current_root->left->right,key);   //get node in right subtree of left subtree
                if(current_root->left->right != NULL)   //if the right subtree of the left subtree is not empty
                    current_root->left = rotate_left(current_root->left);  //rotate left subtree towards left to get node as root of left subtree
            }
            else if(key < current_root->left->key)  //if key is less than root of left subtree
            {
                current_root->left->left = splay(current_root->left->left,key); //get node in left subtree of left subtree
                current_root = rotate_right(current_root);  //rotate root right to get node as root of left subtree
            }
            if(current_root->left != NULL)  //if left subtree of root is not empty
                current_root = rotate_right(current_root);
            return current_root;
        }
    }

	Node * rotate_left(Node *node)
	{
		Node *new_root = node->right;	//right child of node becomes new root
		node->right = new_root->left;	//old root's right subtree is updated to left subtree of new root
		new_root->left = node;	//old root becomes left child of new root
		return new_root;
	}

	Node * rotate_right(Node *node)
	{
		Node *new_root = node->left;	//left child of node becomes new root
		node->left = new_root->right;	//old root's left subtree is updated to right subtree of new root
		new_root->right = node;	//old root becomes right child of new root
		return new_root;
	}

    Node * get_max(Node *explorer)
    {
        while(explorer->right != NULL)  //return right most node as max
            explorer = explorer->right;
        return explorer;
    }

    void display(Node *explorer)
    {
        if(explorer != NULL)
        {
            cout<<explorer->key<<" ";
            display(explorer->left);
            display(explorer->right);
        }
    }

public:
    SplayTree()
    {
        root = NULL;
    }

    Node * search(int key)
    {
        return (this->root = splay(this->root,key));
    }

    void insert(int key)
    {
        if(this->root == NULL)  //if tree is empty
        {
            this->root = new Node(key);
            return;
        }
        Node *parent = NULL;
        for(Node *explorer = root; explorer != NULL; )  //explore path to leaf position where node should be inserted
        {
            parent = explorer;
            if(key > explorer->key) explorer = explorer->right; //key should be in right subtree
            else explorer = explorer->left; //key should be in left subtree
        }
        if(key > parent->key)   parent->right = new Node(key);  //place node as right child
        else parent->left = new Node(key);  //place node as left child
        this->root = splay(this->root,key); //splay the newly inserted node
    }

    void delete_key(int key)
    {
        if(this->root == NULL)  //tree is empty
        {
            cout<<"\ntree is empty\n";
            return;
        }
        Node *node = search(key);
        if(key != node->key)    //key is not present in tree
        {
            cout<<"\nkey not found\n";
            return;
        }
        Node *left_subtree,*right_subtree;
        left_subtree = this->root->left;
        right_subtree = this->root->right;
        delete this->root;  //delete the key
        if(left_subtree == NULL)
            this->root = right_subtree;
        else
        {
            left_subtree = splay(left_subtree,get_max(left_subtree)->key);  //splay the largest key in left subtree
            left_subtree->right = right_subtree;
            this->root = left_subtree;
        }
        cout<<key<<" has been deleted\n";
    }

    void display()
    {
        cout<<"\nPreorder traversal of tree is:\n";
        display(this->root);
    }

    bool is_empty()
    {
        return (this->root == NULL);
    }
};

int main()
{
    SplayTree t;
    int choice,value;
    while(1)
    {
        cout<<"\n\n1> Insert key\n2> Delete key\n3> Search key\n4> Display tree\n";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"Enter the key : ";
                    cin>>value;
                    t.insert(value);
                    cout<<"\n"<<value<<" has been inserted\n";
                    break;
            case 2: cout<<"Enter the key : ";
                    cin>>value;
                    t.delete_key(value);
                    break;
            case 3:
            {
                cout<<"Enter the key : ";
                cin>>value;
                Node *node = t.search(value);
                if(node->key == value)  cout<<"\nkey found";
                else    cout<<"\nkey not found";
                cout<<"\n";
                break;
            }
            case 4: if(t.is_empty())    cout<<"\nTree is empty";
                    else    t.display();
                    break;
            default:return 0;
        }
    }
}
