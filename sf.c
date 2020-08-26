
#include "sf.h"

struct SpamFilter *SF_AllocFilter(long time)
{
    struct SpamFilter *filter;

    filter = (struct SpamFilter *)malloc(sizeof(struct SpamFilter));
    if(filter != nullptr) {
        filter->base = nullptr;
        filter->head = nullptr;
        filter->tail = nullptr;
        filter->time = time;
    }

    return filter;
}

void SF_FreeFilter(struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->head, *next;
    
    while(node != nullptr) {
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
    if(node != nullptr) {
        len = strlen(id);
        node->id = (char *)malloc(len + 1);
        if(node->id == nullptr) {
            free(node);
            return nullptr;
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
    while(node->right != nullptr)
        node = node->right;

    return node;
}

struct SpamFilterNode *SF_Insert(const char *id, long value, struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->base, *parent, **pointer;
    time_t timer;
    int res;

    if(node == nullptr) {
        filter->base = SF_AllocNode(nullptr, nullptr, nullptr, nullptr, nullptr, id, value, (long)time(&timer));
        if(filter->base != nullptr) {
            filter->head = filter->base;
            filter->tail = filter->base;
        }
        return filter->base;
    }
    else {
        while(node != nullptr) {
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
    *pointer = SF_AllocNode(parent, nullptr, nullptr, filter->tail, nullptr, id, value, (long)time(&timer));
    if(*pointer != nullptr)
        filter->tail = *pointer;
    
    return *pointer;
}

struct SpamFilterNode *SF_Find(const char *id, struct SpamFilter *filter)
{
    struct SpamFilterNode *node = filter->head;
    int res;

    while(node != nullptr) {
        res = strcmp(id, node->id);
        switch(res) {
            case -1:
                node = node->left;
                break;
            case 0:
                return node;
            case 1:
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

    while(node != nullptr) {
        next = node->next;
        if(time - node->time > filter->time) {
            // List reassignment
            if(node->next == nullptr && node->previous == nullptr) {
                filter->head = nullptr;
                filter->tail = nullptr;
            }
            else if(node->next == nullptr) {
                filter->tail = node->previous;
                node->previous->next = nullptr;
            }
            else if(node->previous == nullptr) {
                filter->head = node->next;
                node->next->previous = nullptr;
            }
            else {
                node->next->previous = node->previous;
                node->previous->next = node->next;
            }
            // Tree reassignment
            if(node->left == nullptr && node->right == nullptr) {
                if(node->parent == nullptr)
                    filter->base = nullptr;
                else if(node == node->parent->left)
                    node->parent->left = nullptr;
                else
                    node->parent->right = nullptr;
            }
            else if(node->left == nullptr) {
                if(node->parent == nullptr)
                    filter->base = node->right;
                else if(node == node->parent->left)
                    node->parent->left = node->right;
                else
                    node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            else {
                if(node->parent == nullptr)
                    filter->base = node->left;
                else if(node == node->parent->left)
                    node->parent->left = node->left;
                else
                    node->parent->right = node->left;
                node->left->parent = node->parent;
                if(node->right != nullptr) {
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
