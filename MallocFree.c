#include <assert.h>
#include <string.h>
#include <sys/types.h>

#include <stdio.h>
#include <unistd.h>
#define align8(x) (((((x)-1) >> 3) << 3) + 8)

typedef struct block* p_block;

struct block {
    size_t size;
    p_block next;
    p_block prev;
    int free;
    void* ptr;
    char data[1];
};

#define BLOCK_SIZE sizeof(struct block)
void* base = NULL;

p_block find_block(p_block *last, size_t size){
    p_block b = base;
    while(b && !(b->free && b->size >= size)){
        *last = b;
        b = b->next;
    }
    return b;
};


p_block extend_heap(p_block last, size_t s){
    p_block b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void*)-1){
        return NULL;
    }
    b->size = s;
    b->prev = last;
    b->next = NULL;
    b->ptr = b->data;
    if(last){
        last->next = b; 
    }

    b->free = 0;
    return b;
};

void split_block(p_block b, size_t s){
    p_block new;
    new = (p_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    new->ptr = new->data;
    b->size = s;
    b->next = new;
    if(new->next)
        new->next->prev = new;
};

void* Malloc(size_t size){
    size_t s;
    p_block last, b;
    s = align8(size);
    if(base){
        last = base;
        b = find_block(&last, s);
        if(b){
            if(b->size - s >= BLOCK_SIZE + 8)
                split_block(b, s);
            b->free = 0;
        } else {
            b = extend_heap(last, s);
            if(!b) return NULL;
        }
    } else {
        b = extend_heap(NULL, s);
        if(!b) return NULL;
        base = b;
    }

    return b->data;
}

void* Calloc(size_t number, size_t size){
    size_t *new;
    size_t s8, i;
    new = Malloc(number * size);
    if(new){
        s8 = align8(number * size) << 3;
        for(i = 0; i < s8; i++){
            new[i] = 0;
        }
    }

    return 0;
};

p_block fusion(p_block b){
    if(b->next && b->next->free){
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if(b->next){
            b->next->prev = b;    
        }
    }
    return b;
}


p_block get_block(void* p){
    return (p-=BLOCK_SIZE);
};

int valid_addr(void* p){
    if(base){
        if(p > base && p < sbrk(0)){
            return p == (get_block(p))->ptr;
        }
    }
    return 0;
};


void Free(void* p){
    p_block b;
    if(valid_addr(p)){
        b = get_block(p);
        b->free = 1;
        if(b->prev && b->prev->free) fusion(b->prev);
        if(b->next) fusion(b);
        else {
            if(b->prev) b->prev->next = NULL;
            else base = NULL;
            brk(b);
        }
    }
}

void* Realloc(void *p, size_t size){
    // TODO
    return NULL;
};


int main() {
  int *ptr = Malloc(sizeof(int));
  if (ptr == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr = 1;
  *ptr = 100;

  Free(ptr);

  printf("malloc'd an int, assigned to it, and free'd it\n");

  int *ptr2 = Malloc(sizeof(int));
  if (ptr2 == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr2 = 2;
  *ptr2 = 200;

  Free(ptr2);
  printf("malloc'd an int, assigned to it, and free'd it #2\n");

  Malloc(1); // Screw up alignment.

  int *ptr3 = Malloc(sizeof(int));
  if (ptr3 == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr3 = 3;
  *ptr3 = 300;

  Free(ptr3);
  printf("malloc'd an int, assigned to it, and free'd it #3\n");

  return 0;
}