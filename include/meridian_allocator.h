#ifndef MERIDIAN_ALLOCATOR_H
#define MERIDIAN_ALLOCATOR_H

#include "meridian_common.h"

typedef struct Chunk {
    u8* data;
    usize used;

    struct Chunk* next;
} Chunk;

typedef struct {
    Chunk *start, *current;
    usize chunksize;
} Allocator;

Allocator Allocator_make(u64 chunksize);
void Allocator_free(Allocator* allocator);

void* Allocator_malloc(Allocator* allocator, usize size);

//
// --- GLOBAL ALLOCATOR ---
//

void MainAllocator_init(void);
void MainAllocator_free(void);

void* MainAllocator_malloc(usize size);

#endif//MERIDIAN_ALLOCATOR_H
