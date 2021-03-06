#include "max_heap.h"
#include <iostream>

void printMenu() {
    std::cout << "\n\nHeap operations\n1> insert element\n2> delete element\n3> display heap\n";
    std::cout << "Press any other key to exit\n";
    std::cout << "> ";
}

int main()
{
    int choice, value;
    heap heap;
    heap.init_heap();

    while (1)
    {
        printMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "Enter the value > ";
            std::cin >> value;
            heap.insert(value);
            break;
        case 2:
            heap.delete_max();
            break;
        case 3:
            heap.display();
            break;
        default:
            return 0;
        }
    }
}
