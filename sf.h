/*
	Spam filter
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#define null NULL

/*
    This struct represents a node in a SpamFilter. The nodes are placed
    depending on their id and the order of insertion.
*/
struct SpamFilterNode {
    struct SpamFilterNode *parent;
    struct SpamFilterNode *left;
    struct SpamFilterNode *right;
    struct SpamFilterNode *previous;
    struct SpamFilterNode *next;
    char *id;
    long value;
    long time;
};

/*
    This struct represents a base for SpamFilterNodes.
*/
struct SpamFilter {
    struct SpamFilterNode *base;
    struct SpamFilterNode *head;
    struct SpamFilterNode *tail;
    long time;
};

/*
    This function allocates a filter. A pointer to the allocated filter,
    or null, if the allocation failed, is returned.

    time: maximum life time of a node in the filter
*/
struct SpamFilter *SF_AllocFilter(long time);

/*
    This function frees an allocated filter and any nodes within it.

    filter: filter to free
*/
void SF_FreeFilter(struct SpamFilter *filter);

/*
    This function allocates a node. The id should point to a null terminated buffer.
    The contents of id are copied into an allocated buffer, and then assigned to node's id member.
    A pointer to the allocated node, or null, if the allocation failed, is returned.

    parent: parent node in the tree
    left: left node in the tree
    right: right node in the tree
    previous: previous node in the list
    next: next node in the list
    id: id of the node
    value: value of the node
    time: time the node was created
*/
struct SpamFilterNode *SF_AllocNode(struct SpamFilterNode *parent, struct SpamFilterNode *left, struct SpamFilterNode *right, struct SpamFilterNode *previous, struct SpamFilterNode *next, const char *id, long value, long time);

/*
    This function frees an allocated node.

    node: node to free
*/
void SF_FreeNode(struct SpamFilterNode *node);

/*
    This function finds the node with greatest id beginning at node.
    A pointer to the greatest node is returned.

    node: node that serves as the base node for the search
*/
struct SpamFilterNode *SF_FindGreatest(struct SpamFilterNode *node);

/*
    This function creates a node with the id and value,
    and inserts it into the filter. The id should point to a
    null terminated buffer. The contents of id are copied into
    an allocated buffer and then assigned to the
    node's id member. A pointer to the allocated node, or null,
    if the allocation failed, is returned.

    id: id of the node
    value: value of the node
    filter: filter to insert the node into
*/
struct SpamFilterNode *SF_Insert(const char *id, long value, struct SpamFilter *filter);

/*
    This function attempts to locate a node with the given id in the filter.
    A pointer to the found node, or null, if not found, is returned.

    id: id of the node
    filter: filter to search
*/
struct SpamFilterNode *SF_Find(const char *id, struct SpamFilter *filter);

/*
    This function frees every node in the filter with
    a greater time - SpamFilterNode::time value than
    SpamFilter::time.

    time: current time
    filter: filter to search
*/
void SF_Delete(struct SpamFilter *filter, long time);
