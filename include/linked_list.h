#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#ifndef container_of
#ifdef __LIST_HAVE_TYPEOF
#define container_of(ptr, type, member)                            \
    __extension__({                                                \
        const __typeof__(((type *) 0)->member) *__pmember = (ptr); \
        (type *) ((char *) (ptr) - offsetof(type, member));        \
    })
#else
#define container_of(ptr, type, member) \
    ((type *) ((char *) (ptr) - offsetof(type, member)))
#endif
#endif


#ifndef linked_list_H
#define linked_list_H
struct List_head{
    struct List_head *prev;
    struct List_head *next;
};
typedef struct List_head list_head;

typedef struct {
    char *value;
    struct List_head list;
} element_t;
list_head* new_head();
int queue_size(list_head* head);
void insert_tail(list_head* head, char* data);
void print_queue(list_head* head);
char* search_queue(list_head* head, int target);
#endif

#define max_queue_size 16

list_head* new_head(){
	list_head* new = malloc(sizeof(list_head));
	new->prev = new;
	new->next = new;
	return new;
}

int queue_size(list_head* head){
	if(!head){
		perror("head is NULL\n");
		return -1;		
	}else{
		int size = 0;
		list_head* current = head->next;
		while(current != head){
			size++;
			current = current->next;
		}
		return size;
	}	
}

void insert_tail(list_head* head, char* data) {
    if (!head) {
        perror("head is NULL\n");
        return;
    } else if (queue_size(head) < max_queue_size) {
        element_t *new_node = malloc(sizeof(element_t));
        if (!new_node) {
            perror("malloc error\n");
            return;
        }

        new_node->value = malloc(sizeof(char) * (strlen(data) + 1));
        if (!new_node->value) {
            perror("malloc error\n");
            free(new_node);
            return;
        }


        strncpy(new_node->value, data, strlen(data) + 1);
        /*
        printf("Allocated for new_node: %p\n", new_node);
        printf("Allocated value for new_node: %p\n", new_node->value);
		*/
        // Insert into the linked list
        list_head *temp = head->prev;
        temp->next = &new_node->list;
        new_node->list.prev = temp;
        new_node->list.next = head;
        head->prev = &new_node->list;

    } else {
        // Queue is full, remove the oldest element
        list_head *temp = head->next;
        element_t *ele = container_of(temp, element_t, list);
        if (ele && ele->value) {
            free(ele->value);
        }
        free(ele);  // No need to free ele->list, as it's part of the structure

        head->next = temp->next;
        temp->next->prev = head;

        insert_tail(head, data);  // Insert the new element
    }
}



void print_queue(list_head* head) {
    if (!head) {
        perror("head is NULL\n");
        return;
    }

    int count = 1;
    list_head* current = head->next;  // Start from the first element
    printf("history cmd\n");

    while (current != head) {  // Traverse the list
        // Use the container_of macro to get the element_t struct
        element_t *ele = container_of(current, element_t, list);
        /*
        printf("head: %p\n", head);
        printf("current: %p\n", current);
        printf("Element obtained, ele: %p\n", ele);
        printf("Element value obtained, ele->value: %p\n", ele->value);
		*/
        if (ele && ele->value) {
            printf("%2d: %s\n", count++, ele->value);  // Print the command history
        } else {
            printf("Invalid element or value is NULL.\n");
        }

        current = current->next;  // Move to the next element
    }

}

char* search_queue(list_head* head, int target) {
    if (!head) {
        perror("head is NULL\n");
        return NULL;
    }

    int count = 1;
    list_head* current = head->next;

    while (target != count) {
    	if(current == head)
    		perror("record index not found\n");
    	count++;	
    	current = current->next;
    }
    element_t *ele = container_of(current, element_t, list);
    
    if(ele->value){
    	return ele->value;
    }
    return NULL;
}




