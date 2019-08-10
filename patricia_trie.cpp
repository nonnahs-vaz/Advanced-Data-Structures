//implementation of patricia trie
#include<iostream>
using namespace std;
class Node
{
public:
    bool *key;
    int key_size;
    int bit_number;
    Node *left,*right;

    Node(bool value[], int size, int b)
    {
        this->bit_number = b;
        this->key_size = size;
        this->key = new bool [size];
        for(int i = 0; i < size; i++)
            key[i] = value[i];
        left = right = NULL;
    }

    Node(bool value[], int size)
    {
        this->key_size = size;
        this->key = new bool [size];
        for(int i = 0; i < size; i++)
            key[i] = value[i];
        left = right = NULL;
    }

    void display()
    {
        cout<<"\nkey: ";
        for(int i = 0; i < key_size; i++)
            cout<<key[i];
        cout<<"\nbit number: "<<bit_number<<"\n";
    }
};

class PatriciaTrie
{
private:
    Node *head;
    int key_size;

public:
    PatriciaTrie(int value)
    {
        head = NULL;
        key_size = value;
    }

    bool is_empty()
    {
        return (head == NULL);
    }

    void insert(bool value[])
    {
        if(this->head == NULL)  //if patricia trie is empty
        {
            this->head = new Node(value,key_size,0);    //create a node with bit number = 0
            this->head->left = this->head;  //set left pointer to point to self
            return;
        }
        Node *new_node = new Node(value,key_size,-1);  //create a new node
        Node *q = search(value);    //get node at which search terminates
        int j = compare_keys(q,new_node);
        if(j == (key_size + 1)) //if key is already present
            return;
        new_node->bit_number = j;   //set bit number of new node
        Node *prev_node = this->head;   //search starts from the head node
        int prev_bit_number = head->bit_number;
        Node *next_node = this->head->left;
        int next_bit_number = head->left->bit_number;
        while((prev_bit_number < next_bit_number) && (next_bit_number < j))    //find position of node in patricia
        {
            prev_node = next_node;
            if(value[next_bit_number - 1] == 0) //bit value is 0
                next_node = next_node->left;    //move to left subtree
            else
                next_node = next_node->right;   //move to right subtree
            //update bit numbers
            prev_bit_number = prev_node->bit_number;
            next_bit_number = next_node->bit_number;
        }
        if(next_node == prev_node->left)    //if next node was left child of parent
            prev_node->left = new_node; //make new node the left child of parent
        else
            prev_node->right = new_node;
        if(value[j - 1] == 0) //value of bit at bit number of new node is 0
        {
            new_node->left = new_node;  //left link of new node points to self
            new_node->right = next_node;    //right link of new node points to next node
        }
        else    //value of bit at bit number of node is 1
        {
            new_node->right = new_node; //right link of new node points to self
            new_node->left = next_node; //left link of ne node points to next node
        }
        cout<<"\nnode inserted is:\n";
        new_node->display();
    }

    Node * search(bool value[])
    {
        if(head == NULL) return NULL;   //patricia is empty
        Node *prev_node = this->head;   //search starts from the head node
        int prev_bit_number = head->bit_number;
        Node *next_node = this->head->left;
        int next_bit_number = head->left->bit_number;
        while(prev_bit_number < next_bit_number)    //until a backward pointer is not reached
        {
            prev_node = next_node;
            if(value[next_bit_number - 1] == 0) //bit value is 0
                next_node = next_node->left;    //move to left subtree
            else
                next_node = next_node->right;   //move to right subtree
            //update bit numbers
            prev_bit_number = prev_node->bit_number;
            next_bit_number = next_node->bit_number;
        }
        return next_node;   //return node at which search stops
    }

    int compare_keys(Node *a, Node *b)
    {
        for(int i = 0; i < key_size; i++)
        {
            if(a->key[i] != b->key[i])
                return (i + 1);   //return index of first mismatching bit
        }
        return key_size + 1;    //both keys are equal
    }

    void display()
    {
        if(is_empty())
        {
            cout<<"\nTrie is empty\n";
            return;
        }
        cout<<"\nhead:";
        head->display();
        if(head->left == head)  //only one node in trie
            return;
        display(head->left);
    }

    void display(Node *explorer)
    {
        explorer->display();

        if(explorer->left->bit_number > explorer->bit_number)   //if backward pointer is not reached
            display(explorer->left);
        if(explorer->right->bit_number > explorer->bit_number)  //if backward pointer is not reached
            display(explorer->right);
    }
};


void read_value(bool value[], int key_size)
{
    for(int i = 0; i < key_size; i++)
    {
        cout<<"> ";
        cin>>value[i];
    }
}

int main()
{
    int key_size,choice;
    cout<<"\nEnter the size of keys for the patricia trie: ";
    cin>>key_size;
    bool *value = new bool [key_size];
    PatriciaTrie t(key_size);
    while(1)
    {
        cout<<"\n\n1> insert key\n2> search\n3> display\n";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"Enter the value:\n";
                    read_value(value,key_size);
                    t.insert(value);
                    break;
            case 2: {
                        if(t.is_empty())
                            cout<<"\ntrie is empty\n";
                        else
                        {
                            cout<<"Enter the value:\n";
                            read_value(value,key_size);
                            Node *x = new Node(value,key_size,0);   //create a node with key to be searched
                            Node *n = t.search(value);
                            if(t.compare_keys(n,x) == (key_size + 1))
                                cout<<"\nkey found\n";
                            else
                                cout<<"\nkey not found\n";
                        }
                    }
                    break;
            case 3: t.display();
                    break;
            default:return 0;
        }
    }
}
