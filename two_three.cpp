//implementation of 2-3 tree
#include<iostream>
using namespace std;
class Node
{
public:
    int l_key,r_key;    //keys of node
    Node *parent;   //pointer to parent
    Node *left,*right,*middle;  //pointer to left, middle and right subtree

    Node(int key)   //create a node with one key
    {
        l_key = key;
        r_key = -1;
        parent = left = right = middle = NULL;
    }

    Node(int key, Node *p)  //create a node with one key and initialize its parent
    {
        l_key = key;
        r_key = -1;
        parent = p;
        left = right = middle = NULL;
    }

    bool is_full()
    {
        return (r_key != -1);
    }

    bool is_leaf()
    {
        return ((left == NULL) && (right == NULL));
    }

    void insert_value(int value) //insert value in a two node at appropriate position
    {
        if(value < l_key)   //if value is less than left key
        {
            r_key = l_key;
            l_key = value;
        }
        else
            r_key = value;
    }

    int compare_key(int value)  //compare value with all keys in the node
    {
        if(value == l_key) return 0;    //value matches left key
        if(value < l_key) return 1; //value is less than left most key  --follow left pointer
        if(r_key == -1) return 3;   //node is a two node    --follow right pointer
        //node is a three node
        if(value == r_key) return 0;    //value matches right key
        if(value < r_key) return 2; //value is greater than leftmost key and less than rightmost key    --follow middle pointer
        return 3;   //value is greater than rightmost key   --follow right pointer
    }

    void display()  //display a node's contents
    {
        cout<<"["<<l_key;
        if(is_full()) cout<<","<<r_key; //display second key if node is a three node
        cout<<"]";
    }
};

class TwoThreeTree
{
private:
    void insert(Node *current_node, int value)  //insert a value in the tree by searching for appropriate leaf node
    {
        if(current_node->is_leaf()) //reached a leaf node, insert value in current node
        {
            if(current_node->is_full() == false)    //node is a two node
                current_node->insert_value(value);  //put value in two node
            else    //node is full
            {
                split_leaf_node(current_node, value);   //split node and make it a two node
                if(current_node->parent == NULL)    //if current node has no parent
                    root = current_node;    //it becomes the new root of the tree
                else    //current node has a parent
                    push_up(current_node);  //try to merge current node with parent
            }
            return; //value inserted successfully
        }
        //node is an internal node, traverse downwards towards leaf node
        switch(current_node->compare_key(value))    //compare value with keys of current node
        {
            case 1: //value is smaller than left key
                insert(current_node->left, value);
                break;
            case 2: //node is a three node and value is less than right key
                insert(current_node->middle, value);
                break;
            case 3: //node is a two node or three node and value is greater than all keys
                insert(current_node->right, value);
        }
    }

    void split_leaf_node(Node *current_node, int value) //split a full leaf node
    {
        switch(current_node->compare_key(value))
        {
            case 1: //value less than left most key
                current_node->left = new Node(value, current_node); //create a node towards left with value
                current_node->right = new Node(current_node->r_key, current_node);  //create node towards right with right key
                current_node->r_key = -1;   //retain left key and discard right key
                break;
            case 2: //value is greater than left key and less than right key
                current_node->left = new Node(current_node->l_key, current_node);   //create node towards left with left key
                current_node->right = new Node(current_node->r_key, current_node);  //create node towards right with right key
                current_node->l_key = value;    //store value in node
                current_node->r_key = -1;   //discard right key
                break;
            case 3: //value is greater than right most key
                current_node->left = new Node(current_node->l_key, current_node);   //create node towards left with left key
                current_node->right = new Node(value, current_node);    //create node towards right with value
                current_node->l_key = current_node->r_key;  //retain right key in node
                current_node->r_key = -1;
        }
    }

    void split_internal_node(Node *parent, Node *current_node) //split parent node with current_node containing the extra key
    {
        Node *temp;
        int value = current_node->l_key;    //the extra value to be inserted into parent
        switch(parent->compare_key(value))
        {
            case 1: //value is less than left most key
                parent->left = current_node;    //link node as left child of parent
                current_node->parent = parent;
                temp = new Node(parent->r_key, parent); //create a new node towards right with right key
                parent->r_key = -1; //discard right key of parent
                temp->right = parent->right;    //right link of new node gets right subtree of parent
                temp->right->parent = temp;
                temp->left = parent->middle;    //left link of new node gets middle subtree of parent
                temp->left->parent = temp;
                parent->right = temp;   //link new node as right child of parent
                parent->middle = NULL;  //parent no longer has middle subtree
                break;
            case 2: //value is greater than left most key but less than right most key
                temp = new Node(parent->l_key, parent); //create new node with left key of parent
                temp->left = parent->left;  //left link of new node gets left subtree of parent
                temp->left->parent = temp;
                temp->right = current_node->left;   //right link of new node gets left subtree of node to be merged
                temp->right->parent = temp;
                parent->left = temp;    //link new node as left child of parent
                temp = new Node(parent->r_key, parent); //create another node towards right of parent with right key
                temp->left = current_node->right;   //left link of new node gets right subtree of node to be merged
                temp->left->parent = temp;
                temp->right = parent->right;    //right link of new node gets right subtree of parent
                temp->right->parent = temp;
                parent->right = temp;   //link new node as right child of parent
                parent->l_key = value;  //store value to be merged into the parent
                parent->r_key = -1; //discard older values
                parent->middle = NULL;  //parent no longer has a middle subtree
                break;
            case 3: //value is greater than right most key
                parent->right = current_node;   //node to be merged becomes right child of parent
                parent->right->parent = parent;
                temp = new Node(parent->l_key, parent); //create new node towards left of parent with left key
                parent->l_key = parent->r_key;  //retain right key in the parent
                parent->r_key = -1;
                temp->left = parent->left;  //left link of new node gets left subtree of parent
                temp->left->parent = temp;
                temp->right = parent->middle;   //right link of new node gets middle subtree of parent
                temp->right->parent = temp;
                parent->left = temp;    //link new node as left child of parent
                parent->middle = NULL;  //parent no longer has a middle subtree
        }
    }

    void push_up(Node *current_node) //try to insert value in parent of current node, if parent is full, split parent and push up
    {
        Node *parent = current_node->parent;    //parent of current node with which we want to merge the extra key
        int value = current_node->l_key;    //the key which has to be merged with parent node
        if(parent->is_full() == false) //if parent is two node
        {   //merge parent and current node
            if(parent->compare_key(value) == 1) //value to insert is less than first key
            {
                parent->left = current_node->left;  //left subtree of node becomes left subtree of parent
                parent->left->parent = parent;
                parent->middle = current_node->right;   //right subtree of node becomes middle subtree of parent
                parent->middle->parent = parent;
                parent->insert_value(value);    //place key of node in parent node
                delete current_node;    //current node is merged successfully and no longer needed
            }
            else    //value to insert is greater than first key
            {
                parent->right = current_node->right;    //right subtree of node becomes right subtree of parent
                parent->right->parent = parent;
                parent->middle = current_node->left;    //left subtree of node becomes middle subtree of parent
                parent->middle->parent = parent;
                parent->insert_value(value);    //place key of node parent node
                delete current_node;    //current node is merged successfully and no longer needed
            }
        }
        else    //node is full and needs to be split
        {
            split_internal_node(parent,current_node);   //split parent of node and make it a two node
            if(parent->parent == NULL)  //if node no longer has a parent after splitting
                root = parent;  //it becomes the new root of the tree
            else    //node has a parent
                push_up(parent);    //try to merge node with parent
        }
    }

    Node * search(Node *explorer, int key)
    {
        if(explorer == NULL) return NULL;   //end of path to search
        switch (explorer->compare_key(key))
        {
            case 0: return explorer;    //key match is found
            case 1: return (search(explorer->left,key));    //key less than left key
            case 2: return (search(explorer->middle,key));  //key greater than left key and less than right key
            case 3: return (search(explorer->right,key));   //key greater than right key
        }
    }

    void display(Node *explorer)
    {
        explorer->display();    //display current node
        cout<<" ";
        if(explorer->is_leaf()) return; //if node is leaf, no more path to explore
        display(explorer->left);    //explore left path
        if(explorer->is_full()) display(explorer->middle);  //if node is a three node, explore middle path
        display(explorer->right);   //explore right path
    }

public:
    Node *root;

    TwoThreeTree()
    {
        this->root = NULL;
    }

    bool is_empty()
    {
        return root == NULL;
    }

    Node * search(int value)
    {
        if(is_empty())
        {
            cout<<"\nTree is empty\n";
            return NULL;
        }
        return search(this->root, value);
    }

    void insert(int value)
    {
        if(root == NULL)
            root = new Node(value);
        else
            insert(root,value);
        cout<<"\n"<<value<<" is inserted\n";
    }

    void display()
    {
        if(is_empty())
        {
            cout<<"\nTree is empty\n";
            return;
        }
        cout<<"\nPreorder traversal of tree is:\n";
        display(root);
        cout<<"\n";
    }
};

int main()
{
    TwoThreeTree t;
    int choice,value;

    while(true)
    {
        cout<<"\n\n1> insert key\n2> search key\n3> display tree\n";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"\nEnter the key to be inserted > ";
                    cin>>value;
                    t.insert(value);
                    break;
            case 2: cout<<"\nEnter the key to search > ";
                    cin>>value;
                    if(t.search(value) != NULL)
                        cout<<"\nkey found\n";
                    else
                        cout<<"\nkey not found\n";
                    break;
            case 3: t.display();
                    break;
            default:return 0;
        }
    }
}
