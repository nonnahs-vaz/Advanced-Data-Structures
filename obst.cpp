//implementation of OBST
#include<iostream>
using namespace std;
class Node {
public:
    string key;
    Node *left,*right;

    Node(string key)
    {
        this->key = key;
        left = right = NULL;
    }
};

class OBST
{
private:
    Node *root;
    int **weight_matrix;
    int **cost_matrix;
    int **root_matrix;
    int *p;
    int *q;
    string *keys;
    int number_of_keys;

    void init_matrices() {  //initialize all matrices with default values
        weight_matrix = new int * [number_of_keys + 1];
        cost_matrix = new int * [number_of_keys + 1];
        root_matrix = new int * [number_of_keys + 1];
        for(int i = 0; i <= number_of_keys; i++)
        {
            weight_matrix[i] = new int [number_of_keys + 1];
            cost_matrix[i] = new int [number_of_keys + 1];
            root_matrix[i] = new int [number_of_keys + 1];
        }
        for(int i = 0; i <= number_of_keys; i++)
        {
            weight_matrix[i][i] = q[i];
            cost_matrix[i][i] = 0;
            root_matrix[i][i] = 0;
        }
    }

    void compute_weight_matrix()
    {
        for(int difference = 1; difference <= number_of_keys; difference++)
        {
            for(int i = 0; (i + difference) <= number_of_keys; i++)
            {
                int j = i + difference;
                weight_matrix[i][j] = weight_matrix[i][j - 1] + p[j - 1] + q[j];
            }
        }
    }

    void compute_cost_and_root_matrix()
    {
        int k;
        for(int difference = 1; difference <= number_of_keys; difference++)
        {
            for(int i = 0; (i + difference) <= number_of_keys; i++)
            {
                int j = i + difference;
                int computed_cost,min_cost = 9999,assumed_k;
                for(k = i + 1; k <= j; k++) //find value of k for which cost is minimum
                {
                    computed_cost = cost_matrix[i][k - 1] + cost_matrix[k][j];
                    if(computed_cost < min_cost)
                    {
                        assumed_k = k;
                        min_cost = computed_cost;
                    }
                }
                cost_matrix[i][j] = min_cost + weight_matrix[i][j];
                root_matrix[i][j] = assumed_k;
            }
        }
    }

    void compute_all_matrices()
    {
        init_matrices();
        compute_weight_matrix();
        compute_cost_and_root_matrix();
        cout<<"\nMatrices computed are:";
        cout<<"\nweight matrix:\n";
        display_matrix(weight_matrix);
        cout<<"\ncost matrix:\n";
        display_matrix(cost_matrix);
        cout<<"\nroot matrix:\n";
        display_matrix(root_matrix);
    }

    Node * construct_tree(int i, int j)
    {
        if(i == j)  return NULL;    //leaf node
        int k = root_matrix[i][j];  //get index of key from root matrix
        Node *node = new Node(keys[k - 1]); //create a node with the key
        node->left = construct_tree(i,k - 1);
        node->right = construct_tree(k,j);
        return node;
    }

    void display_tree(Node *explorer,string spacing)
    {
        if(explorer != NULL)
        {
            cout<<spacing+"-> "+explorer->key<<"\n";
            display_tree(explorer->left,spacing + "  ");
            display_tree(explorer->right,spacing + "  ");
        }
    }

public:
    OBST() {}
    OBST(int p[], int q[], string tokens[], int n)
    {
        this->p = p;
        this->q = q;
        keys = tokens;
        number_of_keys = n;
        compute_all_matrices();
        root = construct_tree(0,number_of_keys);
    }

    void display_matrix(int **mat)
    {
        for(int i = 0; i <= number_of_keys; i++)
        {
            for(int j = 0; j < i; j++) cout<<"\t";
            for(int j = i; j <= number_of_keys; j++)
                cout<<mat[i][j]<<"\t";
            cout<<"\n";
        }
        cout<<"\n";
    }

    void display_tree()
    {
        cout<<"\nOBST constructed is:\n";
        display_tree(root,"");
        cout<<"\n";
    }
};

template <class T>
void read_array(T array[], int size)
{
    for(int i = 0; i < size; i++)
    {
        cout<<"> ";
        cin>>array[i];
    }
}

int main()
{
    int number_of_keys;
    cout<<"Enter the number of keys in the tree: ";
    cin>>number_of_keys;
    if(number_of_keys < 1) return 1;
    int p[number_of_keys];    //probabilities of searching keys present in key
    int q[number_of_keys];    //probabilities of searching keys not in tree
    string tokens[number_of_keys];  //each key in the tree
    cout<<"\nEnter the probabilities for each key:\n";
    read_array(p,number_of_keys);
    cout<<"\nEnter the probabilities for keys not present in tree:\n";
    read_array(q,number_of_keys + 1);
    cout<<"\nEnter the keys\n";
    read_array(tokens,number_of_keys);
    OBST t(p,q,tokens,number_of_keys);
    t.display_tree();
    return 0;
}
