#include "List.h"

link newNode(Content* c, link next, link prev){
    link tmp = malloc(sizeof(*tmp));
    tmp -> c = c;
    tmp -> next = next;
    tmp -> prev = prev;
    return tmp;
}

LIST ListInit(){
    LIST tmp = malloc(sizeof(*tmp));
    tmp -> head = tmp -> tail = NULL;
    tmp -> n = 0;
    return tmp;
}

void insert(Content* c, LIST l){
    if (l -> head == NULL){
        l -> head = l -> tail = newNode(c, NULL, NULL);
        l -> n = 1;
        return;
    }
    l -> tail = l -> tail -> next = newNode(c, NULL, l -> tail);
    (l -> n)++;
}

Content* getAtIndex(int index, LIST l){
    int i;
    link tmp;
    if(index < l -> n){
        for(tmp = l -> head, i = 0; i != index; i++, tmp = tmp -> next);
        return tmp -> c;
    }else{
        fprintf(stdout, "Index out of bound! Max index: %i", l -> n - 1);
        return NULL;
    }
}

Content* getLast(LIST l){
    return l -> tail -> c;
}

Content* pop(LIST l){
    Content* tmp;
    if(l -> n == 1){
        l -> tail = NULL;
        tmp = l -> head -> c;
        free(l -> head);
        l -> head = NULL;
        (l -> n)--;
        return tmp;
    }
    tmp = l -> tail -> c;
    l -> tail = l -> tail -> prev;
    free(l -> tail -> next);
    l -> tail -> next = NULL;
    (l -> n)--;
    return tmp;
}

Content* deleteAtIndex(int index, LIST l){
    Content* return_value;
    link node_to_delete;
    int i;
    if(index == 0 && l -> n != 1){
        return_value = l -> head -> c;
        l -> head = l -> head -> next;
        free(l -> head -> prev);
        l -> head -> prev = NULL;
        l -> n--;
        return return_value;
    }
    if (index == 0 && l -> n == 1){
        return_value = l -> head -> c;
        free(l -> head);
        l -> head = l -> tail = NULL;
        l -> n--;
        return return_value;
    }
    
    if(index == l -> n - 1){
        return pop(l);
    }
    if(index < l -> n){
        for(node_to_delete = l -> head, i = 0; i != index; i++, node_to_delete = node_to_delete -> next);
        node_to_delete -> prev -> next = node_to_delete -> next;
        node_to_delete -> next -> prev = node_to_delete -> prev;
        return_value = node_to_delete -> c;
        free(node_to_delete);
        l -> n--;
        return return_value;
    }else{
        fprintf(stdout, "Index out of bound! Max index: %d", l -> n - 1);
        return NULL;
    }
}

void deleteContent(Content* c, LIST l){
    link node_to_delete;
    if(l -> head -> c == c && l -> n == 1){
        free(l -> head);
        l -> head = l -> tail = NULL;
        l -> n--;
        return;
    }
    if(l -> head -> c == c && l -> n != 1){
        l -> head = l -> head -> next;
        free(l -> head -> prev);
        l -> head -> prev = NULL;
        l -> n--;
        return;
    }
    if(l -> tail -> c == c){
        pop(l);
        return;
    }
    for(node_to_delete = l -> head; node_to_delete != NULL && node_to_delete -> c != c; node_to_delete = node_to_delete -> next);
    if(node_to_delete != NULL){
        node_to_delete -> prev -> next = node_to_delete -> next;
        node_to_delete -> next -> prev = node_to_delete -> prev;
        free(node_to_delete);
        l -> n--;
    }
}

void freeRec(link head){
    if(head == NULL)
        return;
    freeRec(head -> next);
    free(head);
}

void freeList(LIST l){
    freeRec(l -> head);
    free(l);
}
