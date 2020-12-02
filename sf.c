
#include "sf.h"

struct SpamFilter *SF_AllocFilter(long time)
{
    struct SpamFilter *filter;

    filter = (struct SpamFilter *)malloc(sizeof(struct SpamFilter));
    if(filter != null) {
        filter->base = null;
        filter->head = null;
        filter->tail = null;
        filter->time = time;
    }

    return filter;
}

void SF_FreeFilter(struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->head, *next;
    
    while(node != null) {
        next = node->next;
        SF_FreeNode(node);
        node = next;
    }
    free(filter);

    return;
}

struct SpamFilterNode *SF_AllocNode(struct SpamFilterNode *parent, struct SpamFilterNode *left, struct SpamFilterNode *right, struct SpamFilterNode *previous, struct SpamFilterNode *next, const char *id, long value, long time)
{
    struct SpamFilterNode *node;
    size_t len;

    node = (struct SpamFilterNode *)malloc(sizeof(struct SpamFilterNode));
    if(node != null) {
        len = strlen(id);
        node->id = (char *)malloc(len + 1);
        if(node->id == null) {
            free(node);
            return null;
        }
        else
            strcpy(node->id, id);
        node->parent = parent;
        node->left = left;
        node->right = right;
        node->previous = previous;
        node->next = next;
        node->value = value;
        node->time = time;
    }

    return node;
}

void SF_FreeNode(struct SpamFilterNode *node)
{
    free(node->id);
    free(node);

    return;
}

struct SpamFilterNode *SF_FindGreatest(struct SpamFilterNode *node)
{
    while(node->right != null)
        node = node->right;

    return node;
}

struct SpamFilterNode *SF_Insert(const char *id, long value, struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->base, *parent, **pointer;
    time_t timer;
    int res;

    if(node == null) {
        filter->base = SF_AllocNode(null, null, null, null, null, id, value, (long)time(&timer));
        if(filter->base != null) {
            filter->head = filter->base;
            filter->tail = filter->base;
        }
        return filter->base;
    }
    else {
        while(node != null) {
            parent = node;
            res = strcmp(id, node->id);
            if(res <= 0)
                node = node->left;
            else
                node = node->right;
        }
    }
    if(res <= 0)
        pointer = &(parent->left);
    else
        pointer = &(parent->right);
    *pointer = SF_AllocNode(parent, null, null, filter->tail, null, id, value, (long)time(&timer));
    if(*pointer != null)
        filter->tail = *pointer;
    
    return *pointer;
}

unsigned int SF_Strcmp(const unsigned char *s1, const unsigned char *s2)
{
    while(*s1 != '\0' && *s2 != '\0') {
        if(*s1 != *s2)
        break;
        s1++; s2++;
    }
    if(*s1 < *s2)
        return 0;
    else if(*s1 > *s2)
        return 2;
    else
        return 1;
}

struct SpamFilterNode *SF_Find(const char *id, struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->head;
    unsigned int res;

    while(node != null) {
        res = SF_Strcmp(id, node->id);
        switch(res) {
            case 0:
                node = node->left;
                break;
            case 1:
                return node;
            case 2:
                node = node->right;
                break;
            default:
                break;
        }
    }

    return node;
}

void SF_Delete(struct SpamFilter *filter, long time)
{
    struct SpamFilterNode *node = filter->head, *next, *last;

    while(node != null) {
        next = node->next;
        if(time - node->time > filter->time) {
            // List reassignment
            if(node->next == null && node->previous == null) {
                filter->head = null;
                filter->tail = null;
            }
            else if(node->next == null) {
                filter->tail = node->previous;
                node->previous->next = null;
            }
            else if(node->previous == null) {
                filter->head = node->next;
                node->next->previous = null;
            }
            else {
                node->next->previous = node->previous;
                node->previous->next = node->next;
            }
            // Tree reassignment
            if(node->left == null && node->right == null) {
                if(node->parent == null)
                    filter->base = null;
                else if(node == node->parent->left)
                    node->parent->left = null;
                else
                    node->parent->right = null;
            }
            else if(node->left == null) {
                if(node->parent == null)
                    filter->base = node->right;
                else if(node == node->parent->left)
                    node->parent->left = node->right;
                else
                    node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            else {
                if(node->parent == null)
                    filter->base = node->left;
                else if(node == node->parent->left)
                    node->parent->left = node->left;
                else
                    node->parent->right = node->left;
                node->left->parent = node->parent;
                if(node->right != null) {
                    last = SF_FindGreatest(node->left);
                    last->right = node->right;
                    node->right->parent = last;
                }
            }
            SF_FreeNode(node);
        }
        node = next;
    }

    return;
}
