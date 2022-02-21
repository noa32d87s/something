#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    int val;
    struct _node *next;
} node_t;

void listPrint(const char *label, node_t *node)
{
    printf("%s: [", label);
    while(node)
    {
        printf("%d%s", node->val, node->next? ",":"");
        node = node->next;
    }
    printf("]\n");
}

node_t *listReverse(node_t *node)
{
    node_t *head = node;
    node_t *ptr = NULL, *tmp = NULL;

    while(node->next)
    {
        ptr = node->next;
        tmp = ptr->next;
        node->next = tmp;
        ptr->next = head;
        head = ptr;
    }

    return head;
}

node_t *listPut(node_t **node, int val)
{
    *node = (node_t *)malloc(sizeof(node_t));
    (*node)->val = val;
    (*node)->next = NULL;
    return *node;
}

int main(void)
{
    int i;
    int numlist[] = {1, 2, 3, 4, 9, 2, 3, 4, 5, 1, 4, 6, 2};
    node_t *head, *ptr;

    ptr = listPut(&head, numlist[0]);
    for (i = 1; i < sizeof(numlist)/sizeof(numlist[0]); i++)
        ptr = listPut(&ptr->next, numlist[i]);

    listPrint("initial ", head);

    head = listReverse(head);

    listPrint("reversed", head);

    while(head)
    {
        ptr = head;
        head = head->next;
        free(ptr);
    }

    return 0;
}
