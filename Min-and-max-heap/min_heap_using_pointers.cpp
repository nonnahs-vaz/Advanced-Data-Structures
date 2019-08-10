//implementation of min heap
#include<iostream>
using namespace std;
typedef struct node
{
    int key;
    struct node *parent;
    struct node *left_child;
    struct node *right_child;
    struct node *next;
}node;

typedef struct list
{
    node *head; //start of heap
    node *last_node;    //last node added into heap
}list;
list heap;

void init_heap();
node * create_node(int value);
void insert(int value);
node * find_incomplete_node();
void sift_up(node *child);
void delete_min();
node * find_smallest_child(node *parent);
void delete_last_node();
void sift_down(node *parent);
void resolve_relationships(node *new_node);
void display();
void inorder(node *root);
void swap(node *a, node *b);
void update_parent_of_last_node();

int main()
{
    int choice,value;
    init_heap();

    while(1)
    {
        cout<<"\n\nHeap operations\n1> insert element\n2> delete element\n3> display heap\n";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"Enter the value > ";
                    cin>>value;
                    insert(value);
                    break;
            case 2: delete_min();
                    break;
            case 3: display();
                    break;
            default:return 0;
        }
    }
}

void init_heap()
{
    heap.head = NULL;
    heap.last_node = NULL;
}

void insert(int value)
{
    node *new_node;
    new_node = create_node(value);
    if(heap.head == NULL)   //if heap is empty
        heap.head = new_node;
    else
    {
		heap.last_node->next = new_node;    //append node at end of heap
		resolve_relationships(new_node);	//connecting new node to the existing heap
		sift_up(new_node);	//checking for heap property
	}
    heap.last_node = new_node;
    cout<<value<<" has been inserted\n";
}

void resolve_relationships(node *new_node)
{
	node *incomplete_node = find_incomplete_node();	//find the first incomplete node in the heap
	if(incomplete_node->left_child == NULL)
		incomplete_node->left_child = new_node;
	else
		incomplete_node->right_child = new_node;
	new_node->parent = incomplete_node;
}

node * find_incomplete_node()
{
	node *itr;
	for(itr = heap.head; itr != NULL; itr=itr->next)	//search for first incomplete node
	{
		if((itr->left_child == NULL) || (itr->right_child == NULL))
			return itr;
	}
	return NULL;
}

void sift_up(node *child)
{
	node *parent = child->parent;
	if(parent == NULL) //reached root
		return;
	if(child->key < parent->key)
	{
		swap(child,parent);
		sift_up(parent);
	}
}

void delete_min()
{
	if(heap.head == NULL)
	{
		cout<<"\nheap is empty\n";
		return;
	}
	swap(heap.head,heap.last_node);    //swap min element with the last node in heap
	cout<<heap.last_node->key<<" is deleted\n";
	delete_last_node();    //delete last node in the heap
	if(heap.head != NULL)  //if the heap is not empty
		sift_down(heap.head);
}

void sift_down(node *parent)
{
	node *smallest_child = find_smallest_child(parent);	//find smallest child of node
	if(smallest_child == NULL) //leaf node
		return;
	if(smallest_child->key < parent->key)  //if child violates heap property
	{
		swap(smallest_child,parent);
		sift_down(smallest_child);
	}
}

node * find_smallest_child(node *parent)
{
	if(parent->left_child && parent->right_child)	//if both children exist
		return parent->left_child->key < parent->right_child->key ? parent->left_child : parent->right_child;	//return smallest child
	else if(parent->left_child)	//if only left child exists
		return parent->left_child;
	else if(parent->right_child)	//if only right child exist
		return parent->right_child;
	else
		return NULL; //no children
}

void delete_last_node()
{
	node *itr;
	if(heap.head == heap.last_node)	//only one node in heap
	{
		delete heap.head;
		heap.head = heap.last_node = NULL;
	}
	else
	{
		update_parent_of_last_node(); //remove link from parent of last node to last node
		for(itr = heap.head; itr->next != heap.last_node; itr = itr->next);	//find second last node in heap
		delete heap.last_node;
		heap.last_node = itr;	//re assign last node to second last node
	}
}

void update_parent_of_last_node()
{
	if(heap.last_node->parent->left_child == heap.last_node)	//if last node is left child of its parent
		heap.last_node->parent->left_child = NULL;
	else
		heap.last_node->parent->right_child = NULL;
}

node * create_node(int value)
{
    node *new_node = new node;
    new_node->key = value;
    new_node->parent = new_node->left_child = new_node->right_child = new_node->next = NULL;
    return new_node;
}

void display()
{
	if(heap.head == NULL)
	{
		cout<<"\nheap is empty\n";
		return;
	}
	cout<<"\nin-order traversal of heap is:\n";
	inorder(heap.head);
}

void inorder(node *root)
{
	if(root != NULL)
	{
		inorder(root->left_child);
		cout<<root->key<<" ";
		inorder(root->right_child);
	}
}

void swap(node *a, node *b)
{
	int temp = a->key;
	a->key = b->key;
	b->key = temp;
}
