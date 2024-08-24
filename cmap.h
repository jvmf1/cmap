#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct cmap_entry {
	void *key;
	void *data;
	struct cmap_entry *next;
} cmap_entry;

typedef struct cmap {
	size_t size;
	cmap_entry **entries;
	void (*free_data_function)(void*);
	void (*free_key_function)(void*);
	bool (*compare_key_function)(void*, void*);
	size_t (*hash_function)(void*);
} cmap;

cmap* cmap_create(size_t size, bool (*compare_key_function)(void*,void*), size_t (*hash_function)(void*), void (*free_data_function)(void*),
		void (*free_key_function)(void*));

void cmap_free(cmap *map);

cmap_entry* cmap_entry_create(void *key, void *data);

void cmap_entry_free(cmap *m, cmap_entry *entry);

int cmap_insert(cmap *m, void *key, void *data);

int cmap_remove(cmap *m, void *key);

void* cmap_get(cmap *m, void *key);

void cmap_print(cmap *m);

// cmap_resize does not free the original map
cmap* cmap_resize(cmap *m, size_t size);
