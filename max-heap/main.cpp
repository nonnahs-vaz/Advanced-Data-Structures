#include "max_heap.h"
#include <iostream>

int main()
{
    int choice, value;
    init_heap();

    while (1)
    {
        std::cout << "\n\nHeap operations\n1> insert element\n2> delete element\n3> display heap\n";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "Enter the value > ";
            std::cin >> value;
            insert(value);
            break;
        case 2:
            delete_max();
            break;
        case 3:
            display();
            break;
        default:
            return 0;
        }
    }
}
