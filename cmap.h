#pragma once
#include <stddef.h>

typedef struct smap_entry {
	char *key;
	void *data;
	struct smap_entry *next;
} smap_entry;

typedef struct smap {
	size_t size;
	smap_entry **entries;
	void (*free_data_function)(void*);
} smap;


smap* smap_create(size_t size);

void smap_free(smap *map);

size_t smap_hash(const char *str, smap* m);

smap_entry* smap_entry_create(const char *key, void *data);

void smap_entry_free(smap *m, smap_entry *entry);

int smap_insert(smap *m, const char *key, void *data);

int smap_remove(smap *m, const char *key);

void* smap_get(smap *m, const char *key);

void smap_print(smap *m);

// smap_resize does not free the original map
smap* smap_resize(smap *m, size_t size);
