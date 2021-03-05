#pragma once
typedef struct node
{
    int key;
    struct node *parent;
    struct node *left_child;
    struct node *right_child;
    struct node *next;
} node;
