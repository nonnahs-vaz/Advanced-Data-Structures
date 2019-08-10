//implementation of deap
#include<iostream>
#include<vector>
using namespace std;
class Deap
{
public:
    vector <int> min_heap;
    vector <int> max_heap;

    void insert(int value)
    {
        if(min_heap.empty())    //if min side is empty
            min_heap.push_back(value);
        else if(max_heap.empty()) //if max side is empty
            insert_on_max_side(value);
        else    //check which side to add new node
        {
            if(!is_perfect("min_side"))  //if min side is not perfect
                insert_on_min_side(value);
            else if(!is_perfect("max_side")) //if max side is not perfect
                insert_on_max_side(value);
            else if(min_heap.size() == max_heap.size()) //min and max sides are perfect and have same no. of nodes
                insert_on_min_side(value);
            else
                insert_on_max_side(value);
        }
        cout<<"\n"<<value<<" has been inserted\n";
    }

    void insert_on_min_side(int value)
    {
        min_heap.push_back(value);
        int new_node = min_heap.size() - 1; //new node is added at end of min heap
        int partner = find_max_partner(new_node);    //find partner of newly added node
        if(min_heap[new_node] > max_heap[partner])  //if new node added is larger than its max partner
        {
            swap_between_heaps(new_node,partner);   //move node to max side
            sift_up_max_heap(partner);  //verify heap property on max side
        }
        else
            sift_up_min_heap(new_node); //verify heap property on min side
    }

    void insert_on_max_side(int value)
    {
        max_heap.push_back(value);
        int new_node = max_heap.size() - 1; //new node is added at end of max heap
        int partner = find_min_partner(new_node);    //find partner of newly added node
        if(max_heap[new_node] < min_heap[partner])  //if new node added is smaller than its min partner
        {
            swap_between_heaps(partner,new_node);   //move node to min side
            sift_up_min_heap(partner);  //verify heap property on min side
        }
        else
            sift_up_max_heap(new_node); //verify heap property on max side
    }

    void sift_up_min_heap(int child)
    {
        int parent = (child - 1)/2;
        if(min_heap[child] < min_heap[parent])
        {
            swap_within_min_heap(child,parent);
            sift_up_min_heap(parent);
        }
    }

    void sift_up_max_heap(int child)
    {
        int parent = (child - 1)/2;
        if(max_heap[child] > max_heap[parent])
        {
            swap_within_max_heap(child,parent);
            sift_up_max_heap(parent);
        }
    }

    void delete_min()
    {
        int last_node, max_partner;
        if(min_heap.empty())
        {
            cout<<"\nmin side is empty\n";
            return;
        }
        if(last_node_on_min_side())
        {
            last_node = min_heap.size() - 1;    //find last node in min heap
            swap_within_min_heap(0,last_node);  //swap last node with root of min heap
            cout<<"\n"<<min_heap.back()<<" has been deleted\n";
            min_heap.pop_back();
        }
        else
        {
            last_node = max_heap.size() - 1;    //find last node in max heap
            swap_between_heaps(0,last_node); //swap last node with root of min heap
            cout<<"\n"<<max_heap.back()<<" has been deleted\n";
            max_heap.pop_back();
        }
        if(!min_heap.empty() && min_heap.size() > 1)
            sift_down_min_heap(0);
    }

    void sift_down_min_heap(int parent)
    {
        int smallest_child = find_smallest_child(parent,"min_side");
        int max_partner, max_partner_smallest_child;
        if(!node_exists(smallest_child,"min_side")) //if node is a leaf
        {
            //verify partner property with max partner
            max_partner = find_max_partner(parent);
            if(min_heap[parent] > max_heap[max_partner])    //if partner property is violated
            {
                swap_between_heaps(parent,max_partner);
            }
            else return;
        }
        else    //node is not a leaf
        {
            max_partner = find_max_partner(parent);
            if(min_heap[parent] > max_heap[max_partner])    //if partner property is violated
            {
                swap_between_heaps(parent,max_partner);
                sift_down_max_heap(max_partner);
            }
            else if(min_heap[smallest_child] < min_heap[parent])    //if heap property is violated
            {
                swap_within_min_heap(smallest_child,parent);
                sift_down_min_heap(smallest_child);
            }
            else return;
        }
    }

    void delete_max()
    {
        int last_node, min_partner;
        if(max_heap.empty())
        {
            cout<<"\nmax side is empty";
            return;
        }

        if(last_node_on_min_side())
        {
            last_node = min_heap.size() - 1;    //find last node in min heap
            swap_between_heaps(last_node,0);    //swap last node in min heap with root of max heap
            cout<<"\n"<<min_heap.back()<<" has been deleted\n";
            min_heap.pop_back();
        }
        else
        {
            last_node = max_heap.size() - 1;    //find last node in max heap
            swap_within_max_heap(last_node,0);  //swap last node in max heap with root of max heap
            cout<<"\n"<<max_heap.back()<<" has been deleted\n";
            max_heap.pop_back();
        }
        if(!max_heap.empty())
        {
            min_partner = find_min_partner(0);  //find min partner of root of max heap
            if(max_heap[0] < min_heap[min_partner]) //verify partner property
            {
                swap_between_heaps(min_partner,0);  //swap root of max heap with its min partner
                sift_down_min_heap(0);  //verify heap property on min side
            }
            else
                sift_down_max_heap(0);  //verify heap property on max side
        }
    }

    void sift_down_max_heap(int parent)
    {
        int largest_child = find_largest_child(parent,"max_side");
        int min_partner, min_partner_largest_child;
        if(!node_exists(largest_child,"max_side"))  //if node is a leaf
        {
            //verify partner property with min partner
            min_partner = find_min_partner(parent);
            if(min_heap[min_partner] > max_heap[parent])    //if partner property is violated
            {
                swap_between_heaps(min_partner,parent); //swap with min partner
                sift_down_min_heap(min_partner);    //check heap property on min side
            }
            else //if property is satisfied, check partner property with the largest child of min partner
            {
                min_partner_largest_child = find_largest_child(min_partner,"min_side"); //find largest child of min partner
                if(node_exists(min_partner_largest_child,"min_side"))  //if min partner is not a leaf node
                {
                    if(min_heap[min_partner_largest_child] > max_heap[parent])  //if min partner's largest child violates partner property
                    {
                        swap_between_heaps(min_partner_largest_child,parent);
                    }
                }
            }
            return;
        }
        else    //node is not a leaf
        {
            min_partner = find_min_partner(parent); //find the min partner
            if(min_heap[min_partner] > max_heap[parent])    //if partner property is violated
            {
                swap_between_heaps(min_partner,parent); //swap with min partner
                sift_down_min_heap(min_partner);    //check heap property on min side
            }
            else if(max_heap[largest_child] > max_heap[parent]) //if max heap property is violated
            {
                swap_within_max_heap(largest_child,parent);
                sift_down_max_heap(largest_child);
            }
            else return;
        }
    }

    int find_smallest_child(int parent, string side) //returns index of smallest child
    {
        int left_child = parent*2 + 1;
        int right_child = left_child + 1;

        if(node_exists(left_child,side) && node_exists(right_child,side))   //if both children exist
        {
            if(side.compare("min_side") == 0)
                return min_heap[left_child] < min_heap[right_child] ? left_child : right_child; //return the smallest child of min side
            else
                return max_heap[left_child] < max_heap[right_child] ? left_child : right_child; //return the smallest child of max side
        }
        else if(node_exists(left_child,side)) //if only left child exists
            return left_child;  //return left child
        else if(node_exists(right_child,side))   //if only right child exists
            return right_child; //return right child
        else return min_heap.size(); //no children
    }

    int find_largest_child(int parent, string side) //returns index of largest child
    {
        int left_child = parent*2 + 1;
        int right_child = left_child + 1;

        if(node_exists(left_child,side) && node_exists(right_child,side))   //if both children exist
        {
            if(side.compare("min_side") == 0)
                return min_heap[left_child] > min_heap[right_child] ? left_child : right_child; //return largest child of min heap
            else
                return max_heap[left_child] > max_heap[right_child] ? left_child : right_child; //return the largest child of max heap
        }
        else if(node_exists(left_child,side)) //if only left child exists
            return left_child;  //return left child
        else if(node_exists(right_child,side))   //if only right child exists
            return right_child; //return right child
        else return max_heap.size(); //no children
    }

    bool last_node_on_min_side()
    {
        if(max_heap.empty())    //if there is only one node in deap
            return true;        //node is on min side
        if(!is_perfect("min_side")) //if min side is not a perfect tree
            return true;            //last node is on min side
        else if(!is_perfect("max_side"))    //if max side is not a perfect tree
            return false;                   //last node is on max side (and not on min side)
        else if(min_heap.size() == max_heap.size()) //if both sides have the same number of nodes
            return false;   //last node is not on min side
        else
            return true;    //last node is on min side
    }

    void swap_between_heaps(int min_node, int max_node)
    {
        int temp = min_heap[min_node];
        min_heap[min_node] = max_heap[max_node];
        max_heap[max_node] = temp;
    }

    void swap_within_min_heap(int x, int y)
    {
        int temp = min_heap[x];
        min_heap[x] = min_heap[y];
        min_heap[y] = temp;
    }

    void swap_within_max_heap(int x, int y)
    {
        int temp = max_heap[x];
        max_heap[x] = max_heap[y];
        max_heap[y] = temp;
    }

    int find_min_partner(int node)
    {
        return node;
    }

    int find_max_partner(int node)
    {
        if(!node_exists(node,"max_side"))   //if partner does not exist on max side
            return (node - 1)/2;    //return parent
        else
            return node;
    }

    bool node_exists(int node, string side)
    {
        int size;
        if(side.compare("min_side") == 0)
            size = min_heap.size();
        else
            size = max_heap.size();
        if(node >= 0 && node < size)
            return true;
        else return false;
    }

    bool is_perfect(string side)
    {
        int size;
        if(side.compare("min_side") == 0)
            size = min_heap.size();
        else
            size = max_heap.size();
        size++; //increment size to try and make it a power of 2
        for(int pow = 2; pow <= size; pow *= 2)
        {
            if(size == pow)
                return true;
        }
        return false;
    }

    void display()
    {
        cout<<"\nmin side :\n";
        display_heap(min_heap);
        cout<<"\nmax side :\n";
        display_heap(max_heap);
    }

    void display_heap(vector <int> &heap)
    {
        int size = heap.size();
        int level = 2;
        if(size == 0)
        {
            cout<<"empty\n";
            return;
        }
        for(int i = 0; i < size; i++)
        {
            if(i == level-1)  //if element lies on next level
            {
                cout<<"\n"; //print it on next line
                level*=2;   //update level
            }
            cout<<heap[i]<<" ";
        }
        cout<<"\n";
    }
};

int main()
{
    int choice,value;
    Deap deap;

    while(1)
    {
        cout<<"\n\nDeap operations\n1> insert element\n2> delete min\n3> delete max\n4> display Deap\n";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"Enter the value > ";
                    cin>>value;
                    deap.insert(value);
                    break;
            case 2: deap.delete_min();
                    break;
            case 3: deap.delete_max();
                    break;
            case 4: deap.display();
                    break;
            default:return 0;
        }
    }
}
