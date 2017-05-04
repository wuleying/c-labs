//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/memory/mem.h>

static size_t used_memory = 0;

void *
luo_malloc(size_t size) {
    void *ptr = malloc(size + sizeof(size_t));

    if (!ptr) {
        return NULL;
    }

    *((size_t *) ptr) = size;
    used_memory += size + sizeof(size_t);

    return (char *) ptr + sizeof(size_t);
}

void *
luo_realloc(void *ptr, size_t size) {
    void   *real_ptr;
    void   *new_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return luo_malloc(size);
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    new_ptr  = realloc(real_ptr, size + sizeof(size_t));

    if (!new_ptr) {
        return NULL;
    }

    *((size_t *) new_ptr) = size;
    used_memory -= old_size;
    used_memory += size;

    return (char *) new_ptr + sizeof(size_t);
}

void
luo_free(void *ptr) {
    void   *real_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return;
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    used_memory -= old_size + sizeof(size_t);

    free(real_ptr);
}

char *
luo_strdup(const char *s) {
    size_t l  = strlen(s) + 1;
    char   *p = luo_malloc(l);

    memcpy(p, s, l);

    return p;
}

size_t
luo_malloc_used_memory(void) {
    return used_memory;
}