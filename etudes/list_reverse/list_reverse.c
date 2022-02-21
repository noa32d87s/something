#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define MAXNUM 4

typedef struct _node
{
    int val;
    struct _node *next;
} node_t;

void showHelp(char *name)
{
    printf("Usage: %s [OPTION]\n"
          "Mandatory arguments to short options.\n"
          "\t-h\tdisplay this help and exit\n"
          "\t-n\tnumber of nodes (%d is default)\n",
          name,
          MAXNUM);
}

void nodesParse(char *argv, unsigned long int *nodes)
{
    char *endptr;

    *nodes = strtoul(argv, &endptr, 10);

    if ((argv == endptr) ||
        ((errno == ERANGE)) ||
        (*endptr) ||
        (*nodes <= 1))
    {
        *nodes = MAXNUM;
    }
}

int numlistGenerate(int **numlist, unsigned long int nodes)
{
    unsigned long int i;

    *numlist = (int *)malloc(sizeof(int) * nodes);
    if (!*numlist)
    {
        printf("failed to alloc %lu nodes: %s\n", nodes, strerror(errno));
        return -1;
    }

    srand(time(NULL));

    for (i = 0; i < nodes; i++)
        (*numlist)[i] = rand() % 10;

    return 0;
}

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

node_t *listPut(node_t **node, int val)
{
    *node = (node_t *)malloc(sizeof(node_t));
    (*node)->val = val;
    (*node)->next = NULL;
    return *node;
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

int main(int argc, char *argv[])
{
    char opt;
    struct timespec tstart = {0, 0}, tend={0, 0};
    node_t *head, *ptr;
    int *numlist = NULL;
    unsigned long int nodes = MAXNUM;
    int i;

    setbuf(stdout, NULL);

    while ((opt = getopt(argc, argv, "hn:")) != -1)
    {
        switch (opt)
        {
            case 'n':
                nodesParse(optarg, &nodes);
                break;

            case 'h':
            default:
                showHelp(argv[0]);
                return 0;
        }
    }

    printf("generate numlist:");

    if (numlistGenerate(&numlist, nodes) < 0)
        return -1;

    printf("done\ngenerate node-list:");

    ptr = listPut(&head, numlist[0]);
    for (i = 1; i < nodes; i++)
        ptr = listPut(&ptr->next, numlist[i]);

    printf("done\n");

    if (nodes < 50)
        listPrint("initial ", head);
    else
        printf("too many nodes; print list is disabled\n");

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    head = listReverse(head);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    if (nodes < 50)
        listPrint("reversed", head);

    printf("Total nums: [%lu]%s; Time: %f sec\n",
            nodes,
            (nodes == MAXNUM)? " (default)":"",
            ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    while(head)
    {
        ptr = head;
        head = head->next;
        free(ptr);
    }

    free(numlist);

    if (nodes == MAXNUM)
        showHelp(argv[0]);

    return 0;
}

