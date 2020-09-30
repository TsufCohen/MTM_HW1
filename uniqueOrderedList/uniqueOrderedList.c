#include <stdio.h>
#include "uniqueOrderedList.h"
#include <stdlib.h>
#include <assert.h>


typedef struct node {
    Element data;
    struct node* next;
} *Node;

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              createNode                   \||/  *******************
 *****   \/           ***************************       \/   **************** */
static Node createNode(Element* data) {
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr){
        return NULL;
    }

    ptr->data=data;
    ptr->next=NULL;
    return ptr;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              createNode                   /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

struct uniqueOrderedList_t {
    Node head;
    int list_size;
    Node current_element;

    copyElements copy_element;
    freeElements free_element;
    elementsEquals equal_elements;
    elementGreaterThan greater_element;
};

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListCreate         \||/  *******************
 *****   \/           ***************************       \/   **************** */
UniqueOrderedList uniqueOrderedListCreate(copyElements copy_element,
                                          freeElements free_element,
                                          elementsEquals equal_elements,
                                          elementGreaterThan greater_element) {
    if(!copy_element || !free_element || !equal_elements || !greater_element) {
        return NULL;
    }

    UniqueOrderedList new_list = malloc(sizeof(*new_list));
    if(!new_list) {
        return NULL;
    }

    new_list->list_size =0;
    new_list->head=NULL;
    new_list->current_element=NULL;
    new_list->copy_element = copy_element;
    new_list->free_element = free_element;
    new_list->equal_elements = equal_elements;
    new_list->greater_element = greater_element;

    return new_list;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\            uniqueOrderedListCreate        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListDestroy        \||/  *******************
 *****   \/           ***************************       \/   **************** */
void uniqueOrderedListDestroy(UniqueOrderedList list) {
    if(!list) {
        return;
    }
    uniqueOrderedListClear(list);
    free(list);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListDestroy        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListCopy           \||/  *******************
 *****   \/           ***************************       \/   **************** */
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList list) {
    if(!list) {
        return NULL;
    }
    UniqueOrderedList new_copy = uniqueOrderedListCreate(list->copy_element,
                                                         list->free_element,
                                                         list->equal_elements,
                                                         list->greater_element);
    if(!new_copy) {
        return NULL;
    }
    for(Node iterator = list->head ; iterator ; iterator = iterator->next) {
        switch(uniqueOrderedListInsert(new_copy,(iterator->data))){
            case UNIQUE_ORDERED_LIST_SUCCESS:
                continue;
            default:
                uniqueOrderedListDestroy(new_copy);
                return NULL;
        }
    }
    return new_copy;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              uniqueOrderedListCopy        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListSize           \||/  *******************
 *****   \/           ***************************       \/   **************** */
int uniqueOrderedListSize(UniqueOrderedList list) {
    if(!list) {
        return -1;
    }
    int tmp_size = list->list_size;
    return tmp_size;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              uniqueOrderedListSize        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListContains       \||/  *******************
 *****   \/           ***************************       \/   **************** */
bool uniqueOrderedListContains(UniqueOrderedList list, Element element) {
    if(!list || !element) {
        return false;
    }
    for(Node iterator = list->head ; iterator ; iterator = iterator->next) {
        if (list->equal_elements(element, (iterator->data))) {
            return true;
        }
    }
    return false;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListContains       /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListInsert         \||/  *******************
 *****   \/           ***************************       \/   **************** */
UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList list,
                                                            Element element) {
    if(!list || !element) {
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    else if(uniqueOrderedListContains(list, element)) {
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    }
    else if(!(list->head)) { //if  empty list.
        list->head = createNode(list->copy_element(element));
        if(!(list->head)) {
            return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
        }
        list->list_size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    else if(list->greater_element((list->head->data), element)) {
        Node tmp_previous = list->head;
        list->head = createNode(list->copy_element(element));
        if(!(list->head)) {
            list->head = tmp_previous;
            return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
        }
        list->head->next = tmp_previous;
        list->list_size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    else {
        Node tmp_previous = list->head;
        for(Node iterator=list->head->next; iterator ;iterator=iterator->next) {
            if (list->greater_element((iterator->data), element)) {
                tmp_previous->next = createNode(list->copy_element(element));
                if(!(tmp_previous->next)) {
                    tmp_previous->next = iterator;
                    return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
                }
                tmp_previous->next->next = iterator;
                list->list_size++;
                return UNIQUE_ORDERED_LIST_SUCCESS;
            }
            tmp_previous = iterator;
        }
        tmp_previous->next = createNode(list->copy_element(element));
        if(!(tmp_previous->next)) {
            return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
        }
        list->list_size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListInsert         /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/           uniqueOrderedListRemove         \||/  *******************
 *****   \/           ***************************       \/   **************** */
UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList list,
                                                            Element element) {
    if(!list || !element) {
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    else if(!uniqueOrderedListContains(list, element)) {
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    }
    else if(list->equal_elements((list->head->data), element)) {
        Node toDelete = list->head;
        list->head = toDelete->next;
        list->free_element(toDelete->data);
        free(toDelete);
        list->list_size--;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    else {
        Node tmp_previous = list->head;
        for(Node iterator=list->head->next; iterator ;iterator=iterator->next) {
            if (list->equal_elements(element, (iterator->data))) {
                Node toDelete = iterator;
                tmp_previous->next = toDelete->next;
                list->free_element(toDelete->data);
                free(toDelete);
                list->list_size--;
                return UNIQUE_ORDERED_LIST_SUCCESS;
            }
            tmp_previous = iterator;
        }
    }
    return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListRemove         /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/          uniqueOrderedListGetLowest       \||/  *******************
 *****   \/           ***************************       \/   **************** */
Element uniqueOrderedListGetLowest(UniqueOrderedList list) {
    if(!list || !(list->head)) {
        return NULL;
    }
    list->current_element = list->head;
    Element tmp = (list->current_element->data);
    return tmp;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\          uniqueOrderedListGetLowest       /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/         uniqueOrderedListGetGreatest      \||/  *******************
 *****   \/           ***************************       \/   **************** */
Element uniqueOrderedListGetGreatest(UniqueOrderedList list) {
    if(!list || !(list->head)){
        return NULL;
    }
    if(!(list->current_element)){
        list->current_element = list->head;
    }
    Element tmp = (list->current_element->data);
    while(list->current_element->next){
        tmp = uniqueOrderedListGetNext(list);
    }
    return tmp;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\          uniqueOrderedListGetGreatest     /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/            uniqueOrderedListGetNext       \||/  *******************
 *****   \/           ***************************       \/   **************** */
Element uniqueOrderedListGetNext(UniqueOrderedList list) {
    if(!list || !(list->current_element)){
        return NULL;
    }
    if(!(list->current_element->next)){
        return NULL;
    }
    list->current_element=list->current_element->next;
    Element tmp = (list->current_element->data);
    return tmp;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListGetNext        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/            uniqueOrderedListClear         \||/  *******************
 *****   \/           ***************************       \/   **************** */
void uniqueOrderedListClear(UniqueOrderedList list) {
    if(!list) {
        return ;
    }
    while(list->head) {
        Node toDelete = list->head;
        list->head = toDelete->next;
        list->free_element(toDelete->data);
        free(toDelete);
    }
    list->current_element = list->head;
    list->list_size = 0;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\           uniqueOrderedListClear          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************
