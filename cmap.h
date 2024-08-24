#pragma once
#include <stddef.h>

typedef struct cmap_entry {
	char *key;
	void *data;
	struct cmap_entry *next;
} cmap_entry;

typedef struct cmap {
	size_t size;
	cmap_entry **entries;
	void (*free_data_function)(void*);
} cmap;


cmap* cmap_create(size_t size);

void cmap_free(cmap *map);

size_t cmap_hash(const char *str, cmap* m);

cmap_entry* cmap_entry_create(const char *key, void *data);

void cmap_entry_free(cmap *m, cmap_entry *entry);

int cmap_insert(cmap *m, const char *key, void *data);

int cmap_remove(cmap *m, const char *key);

void* cmap_get(cmap *m, const char *key);

void cmap_print(cmap *m);

// cmap_resize does not free the original map
cmap* cmap_resize(cmap *m, size_t size);
