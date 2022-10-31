#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void PushBack(list_t* self, void* item) {
    self->size++;
    
    if (!self->items) {
        self->items = malloc(self->itemSize);
        if (!self->items) {
            fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
            return;
        }
    }
    else {
        self->items = realloc(self->items, self->size * self->itemSize);
        if (!self->items) {
            fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
            return;
        }
    }

    self->items[self->size - 1] = item;
}

void List(list_t* self, size_t itemSize) {
    self->items = NULL;
    self->size = 0;
    self->itemSize = itemSize;

    self->PushBack = PushBack;
}

list_t* newList(size_t itemSize) {
    list_t* self = (list_t*) malloc(sizeof(list_t));
    if (!self) {
        fprintf(stderr, "ERROR: Couldn't allocate in function: %s\n", __func__);
        return NULL;
    }

    List(self, itemSize);
    return self;
}
