//implementation of fibonacci heap
#include<iostream>
#include<cmath>
using namespace std;

class Node
{
public:
    int key;
    int degree;
    bool marked;
    Node *parent;
    Node *left,*right,*child;

    Node() {}
    Node(int key)
    {
        this->key = key;
        this->degree = 0;
        this->marked = false;
        this->left = NULL;
        this->right = NULL;
        this->child = NULL;
        this->parent = NULL;
    }
};

class FibonacciHeap
{
public:
    Node *min_node; //pointer to minimum node of heap
    int node_count; //keeps count of number of nodes in heap
    FibonacciHeap()
    {
        this->min_node = NULL;
        this->node_count = 0;
    }

    void insert_key(int value)
    {
        if(this->min_node == NULL) //if heap is empty
        {
            Node *new_node = new Node(value);   //create a new node
            new_node->left = new_node->right = new_node;    //establishing circular link to itself
            this->min_node = new_node;  //set new node as the min node of heap
            this->node_count++;     //increase count for adding first node
        }
        else //if heap is not empty
        {
            FibonacciHeap H2;   //create a new heap
            H2.insert_key(value);   //insert the key into the new heap
            this->unify(H2); //concatenate both heaps
        }
    }

    void unify(FibonacciHeap H2)
    {
        Node *min1_left,*min2_left;
        min1_left = this->min_node->left;   //node to left of min node in first heap
        min2_left = H2.min_node->left;     //node to left of min node in second heap
        //concatenating root lists of both heaps
        this->min_node->left = min2_left;   //linking first heap min node with second heap last node
        min2_left->right = this->min_node;  //linking second heap last node with first heap min node
        H2.min_node->left = min1_left;  //linking second heap min node with first heap last node
        min1_left->right = H2.min_node; //linking first heap last node with second heap min node
        this->node_count = this->node_count + H2.node_count;	//update node count
        if(this->min_node->key > H2.min_node->key)  //if second heap has a smaller key than first heap
            this->min_node = H2.min_node;   //update the min_node of concatenated heap
    }

    void extract_min()
    {
        if(this->node_count == 0)
        {
            cout<<"\nheap is empty\n";
            return;
        }
        if(this->min_node->degree != 0)  //if min node has any children
        {
            Node *itr,*min_nodes_child;
            int degree = this->min_node->degree;
            //add children to root list
            itr = this->min_node->child;
            for(int i = 0; i < degree; i++)
            {
				min_nodes_child = itr;
                itr = itr->left;
				add_node_to_root_list(min_nodes_child);
            }
        }
        cout<<"\n"<<this->min_node->key<<" is deleted\n";
        if(this->node_count == 1)   //if heap has only one node
        {
            delete this->min_node;
            this->min_node = NULL;
            this->node_count = 0;
        }
        else    //if heap has more than one node
        {
            Node *old_min = this->min_node;
            this->min_node = this->min_node->right; //set min node to node towards right of min node
            //remove links to min_node from neighbors of min_node
            old_min->left->right = old_min->right;
            old_min->right->left = old_min->left;
            delete old_min;
            this->node_count--;
            if(this->node_count > 1)    //if there are more than one nodes
                consolidate();
        }
    }

    inline void add_node_to_root_list(Node *new_node)
    {
        //unlink from siblings
        new_node->left->right = new_node->right;
        new_node->right->left = new_node->left;
        //update properties of parent
        if(new_node->parent != NULL)
        {
            new_node->parent->degree--; //parent's degree reduces by 1
            new_node->parent = NULL;    //node does not have any parent
        }
        new_node->marked = false;   //node in root list is unmarked
        //add node to left of min_node
        new_node->left = this->min_node->left;
        new_node->right = this->min_node;
        new_node->left->right = new_node;
        new_node->right->left = new_node;
    }

    void consolidate()
    {
        int highest_degree = ceil(log2(this->node_count)); //find highest possible degree for any tree
        Node **auxillary_array = new Node* [highest_degree + 1];   //create an auxillary array to accomodate node with highest possible degree of tree
        for(int i = 0; i <= highest_degree; i++)
            auxillary_array[i] = NULL;  //initialize the array to NULL
        Node *itr,*current_node,*conflicting_node,*temp;
        int degree;
        //break the circular root list
        this->min_node->left->right = NULL;
        //consolidate the heap
        for(itr = this->min_node; itr != NULL; )   //check every node in root list
        {
            current_node = itr;
            itr = itr->right;	//move to the next node
            degree = current_node->degree;
            while(auxillary_array[degree] != NULL)  //until there is no other node with same degree
            {
                //find a position for the node
                conflicting_node = auxillary_array[degree];    //get node with same degree
                if(conflicting_node->key < current_node->key)   //if conflicting node has smaller key than current node
                {
                    //make current node point to the smaller key
                    temp = current_node;
                    current_node = conflicting_node;
                    conflicting_node = temp;
                }
                merge_nodes(current_node,conflicting_node); //merge both nodes into a single tree
                auxillary_array[degree] = NULL; //merged node no longer has the same degree, so vacate its position in the auxillary array
                degree++;   //merged node's degree increases by 1
            }
            auxillary_array[degree] = current_node;    //place node in the auxillary array
        }
        //construct root list with nodes from auxillary array & find min node
        reconstruct_root_list(auxillary_array,highest_degree);
    }

    void reconstruct_root_list(Node **auxillary_array,int highest_degree)
    {
        Node *min = NULL; //least key in auxillary array
		Node *last_node = NULL;   //assumed last node of list
		Node *first_node = NULL;  //first node found in auxillary array
		for(int i = 0; i <= highest_degree; i++)
		{
			if(auxillary_array[i] != NULL)   //if auxillary array contains a node
			{
                if((min == NULL) || (min->key > auxillary_array[i]->key))   //if (min is not initialized) || (key smaller than assumed min is found)
					min = auxillary_array[i];	//new min is the node with smallest key in auxillary array
				if(first_node == NULL)	first_node = auxillary_array[i]; //if first node was not initialized, initialize it
				if(last_node != NULL)   //if a node is found after the assumed last node
				{
                    //link new node and assumed last node
                    last_node->right = auxillary_array[i];
                    auxillary_array[i]->left = last_node;
				}
				last_node = auxillary_array[i]; //new node found is assumed to be the last node
			}
		}
        this->min_node = min;   //update min node of heap
        //make the circular connection of last node and first node
        first_node->left = last_node;
        last_node->right = first_node;
		cout<<"\nnew min is : "<<this->min_node->key<<"\n";
    }

    void merge_nodes(Node *parent,Node *new_child)
    {
        if(parent->child == NULL)   //if parent did not have any children
        {
			new_child->left = new_child->right = new_child; //making circular link at new child
            parent->child = new_child;
        }
        else    //parent had children
        {
			//add node into the circular list of children
			new_child->left = parent->child->left;
			new_child->right = parent->child;
			new_child->left->right = new_child;
			new_child->right->left = new_child;
			if(parent->child->key > new_child->key)	//if new child has smaller key than any other existing child
				parent->child = new_child;  //update parent's child pointer to new_child
		}
        new_child->parent = parent; //linking from child to parent
        parent->degree++;    //degree of parent increases by 1
    }

    void decrease_key(int old_value,int new_value)
    {
        if(this->min_node == NULL)
        {
            cout<<"\nheap is empty\n";
            return;
        }
        if(old_value < new_value)
        {
            cout<<"\ninvalid operation\nnew value is greater than existing value\n";
            return;
        }
        Node *node;
        if((node = search_heap(this->min_node,old_value)) == NULL)  //search the heap for the node
        {
            cout<<"\nkey not found\n";
            return; //key is not present in heap, so return
        }
        node->key = new_value;  //update key of node
        Node *parent = node->parent;
        if(node->parent != NULL)    //if node is not a root node
        {
            if(parent->key > node->key) //if min heap property is violated
            {
                cut(node);  //cut node and put it in root list
                cascade_cut(parent);    //recursively apply cut operation on parent
            }
        }
        if(node->key < this->min_node->key) //if new value of node is less than existing min of heap
            this->min_node = node;  //update the min of heap
        cout<<"\nkey "<<old_value<<" has been decreased to "<<new_value<<"\n";
    }

    Node * search_heap(Node *explorer,int x)
    {
        if(explorer == NULL)    //no path to explore
            return NULL;
        Node *node = NULL;  //stores result of search operation
        explorer->left->right = NULL;   //break the circular list
        for(Node *itr = explorer; itr != NULL; itr = itr->right)    //explore the level
        {
            if(x == itr->key)   //if match is found
            {
                node = itr;
                break;
            }
            else if(x > itr->key && itr->child !=NULL)  //if match was not found and there are chances of finding the node down the sub-tree
            {
                if((node = search_heap(itr->child,x)) != NULL)  //if key was found at descendant level
                    break;
            }
        }
        explorer->left->right = explorer;   //repair the circular list
        return node;
    }

    void cut(Node *node)    //cuts a node from a parent and puts node in root list
    {
        Node *parent = node->parent;
        if(node->left == node)   //if node was the only child of parent
            parent->child = NULL; //remove link from parent to child
        else if(parent->child == node)  //if there are more than one children and parent points to this child
        {
            Node *next_smallest_child = NULL;
            for(Node *itr = parent->child->right; itr != parent->child; itr = itr->right)   //find second smallest child among children
            {
                if((next_smallest_child == NULL) || (next_smallest_child->key > itr->key))  //if a smaller key is found
                    next_smallest_child = itr;
            }
            parent->child = next_smallest_child;
        }
        add_node_to_root_list(node);
    }

    void cascade_cut(Node *node)
    {
        Node *parent = node->parent;
        if(parent != NULL)  //if node is not a root
        {
            if(node->marked == true)    //if node is marked
            {
                cut(node);  //cut the node
                cascade_cut(parent);    //cascade cut operation on parent
            }
            else    //node is unmarked
                node->marked = true;    //mark the node
        }
    }

    void delete_key(int value)
    {
        Node *node = search_heap(this->min_node,value);
        if(node == NULL)
        {
            cout<<"\nInvalid operation\n";
            if(this->min_node == NULL)
                cout<<"Heap is empty\n";
            else
                cout<<"Node not found\n";
            return;
        }
        cout<<"\nRemoving "<<value<<" from heap\n";
        decrease_key(value,-999);   //decrease key value to -999 (lowest possible key value)
        extract_min();  //deleting the value from heap
    }

    int get_min()
    {
        if(this->min_node != NULL) return this->min_node->key;
        else cout<<"\nheap is empty\n";
    }

    void display_heap()
    {
		Node *itr;
		if(this->min_node == NULL)
		{
		    cout<<"\nheap is empty\n";
		    return;
		}
		cout<<"\nHeap contains:\n";
		this->min_node->left->right = NULL; //break the circular link
		for(itr = this->min_node; itr != NULL; itr = itr->right)   //iterate through the root list
		{
		    cout<<"\nRoot of tree: "<<itr->key<<"\tdegree: "<<itr->degree;
		    cout<<"\ndescendants:\n";
		    display_tree(itr);
		}
		this->min_node->left->right = this->min_node;
		cout<<endl;
    }

    void display_tree(Node *root)   //explores each tree in the root list depth wise
    {
		if(root->child == NULL)
		{
			cout<<"\n";
			return;
		}
		else
		{
			Node *itr;
			root->child->right->left = NULL;
			for(itr = root->child; itr != NULL; itr = itr->left)
			{
				cout<<itr->key<<" "<<"degree: "<<itr->degree<<" | ";
				display_tree(itr);
			}
			root->child->right->left = root->child;
		}
	}
};

int main()
{
    FibonacciHeap H;
    int choice,value,new_value;
    while(true)
    {
        cout<<"\n\n1> Insert node\n2> Delete min\n3> Delete node\n4> Get min key\n5> Decrease Key\n6> Display heap\n";
        cin>>choice;
        switch(choice)
        {
            case 1:
                cout<<"Enter the value to be inserted > ";
                cin>>value;
                H.insert_key(value);
                cout<<value<<" is inserted into the heap\n";
            break;
            case 2:
                H.extract_min();
            break;
            case 3:
                cout<<"Enter the value to be deleted > ";
                cin>>value;
                H.delete_key(value);
            break;
            case 4:
                cout<<"Min key is: "<<H.get_min()<<"\n";
            break;
            case 5:
                cout<<"Enter the key > ";
                cin>>value;
                cout<<"Enter the new value > ";
                cin>>new_value;
                H.decrease_key(value,new_value);
            break;
            case 6:
                H.display_heap();
            break;
            default: return 0;
        }
    }
}
