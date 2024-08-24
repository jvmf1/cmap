#include "cmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cmap* cmap_create(size_t size, bool (*compare_key_function)(void*,void*), size_t (*hash_function)(void*)) {
	cmap *map = malloc(sizeof(cmap));
	if (map==NULL) return NULL;
	map->size=size;
	map->compare_key_function=compare_key_function;
	map->hash_function=hash_function;
	map->entries=malloc(sizeof(cmap_entry*)*size);
	if (map->entries==NULL) return NULL;
	for (size_t i=0;i<size;i++) {
		map->entries[i]=NULL;
	}
	return map;
}

void cmap_free(cmap *m) {
	for (size_t i=0;i<m->size;i++) {
		if (m->entries[i]==NULL) continue;
		cmap_entry *tmp = m->entries[i];
		cmap_entry *next;
		while (tmp!=NULL) {
			next = tmp->next;
			cmap_entry_free(m, tmp);
			tmp=next;
		}
	}
	free(m->entries);
	free(m);
}

cmap_entry* cmap_entry_create(void *key, void *data) {
	cmap_entry *entry = malloc(sizeof(cmap_entry));
	if (entry==NULL) return NULL;
	entry->key=key;
	entry->next=NULL;
	entry->data=data;
	return entry;
}

void cmap_entry_free(cmap *m, cmap_entry *entry) {
	if (m->free_key_function)
		m->free_key_function(entry->key);
	if (m->free_data_function!=NULL)
		m->free_data_function(entry->data);
	free(entry);
}

int cmap_insert(cmap *m, void *key, void *data) {
	size_t hash = m->hash_function(key) % m->size;
	cmap_entry *tmp = m->entries[hash];
	cmap_entry *last;
	if (tmp==NULL) {
		// is empty
		m->entries[hash]=cmap_entry_create(key, data);
		if (m->entries[hash]==NULL) return -1;
		return 0;
	}

	while (tmp!=NULL) {
		if (m->compare_key_function(key, tmp->key)) {
			// is overwriting data
			if (m->free_data_function!=NULL)
				m->free_data_function(tmp->data);
			tmp->data=data;
			return 0;
		}
		last=tmp;
		tmp=tmp->next;
	}
	tmp=cmap_entry_create(key, data);
	if (tmp==NULL) return -1;
	last->next=tmp;
	return 0;
}

int cmap_remove(cmap *m, void *key) {
	size_t hash = m->hash_function(key) % m->size;
	cmap_entry *tmp = m->entries[hash];
	cmap_entry *last=NULL;

	while (tmp!=NULL) {
		if (m->compare_key_function(key, tmp->key)) {
			if (last==NULL) {
				if (tmp->next==NULL) {
					// is the first and only
					cmap_entry_free(m, tmp);
					m->entries[hash]=NULL;
					return 0;
				} else {
					// is the first
					m->entries[hash]=tmp->next;
					cmap_entry_free(m, tmp);
					return 0;
				}
			}
			last->next=tmp->next;
			cmap_entry_free(m, tmp);
			return 0;
		}
		last=tmp;
		tmp=tmp->next;
	}
	return -1;
}

void* cmap_get(cmap *m, void *key) {
	size_t hash = m->hash_function(key) % m->size;
	cmap_entry *tmp = m->entries[hash];

	while (tmp!=NULL) {
		if (m->compare_key_function(key, tmp->key)) {
			return tmp->data;
		}
		tmp=tmp->next;
	}
	return NULL;
}

void cmap_print(cmap *m) {
	printf("map:%p size:%zu\n", m, m->size);
	for (size_t i=0;i<m->size;i++) {
		if (m->entries[i]==NULL) continue;
		cmap_entry *tmp = m->entries[i];
		printf("\tentry[%zu]\n", i);
		while (tmp!=NULL) {
			printf("\t\t%p key:'%p' data:%p next:%p\n", tmp, tmp->key, tmp->data, tmp->next);
			tmp=tmp->next;
		}
	}
}

cmap* cmap_resize(cmap *m, size_t size) {
	cmap *new_map = cmap_create(size, m->compare_key_function, m->hash_function);
	if (new_map==NULL) return NULL;

	for (size_t i=0;i<m->size;i++) {
		if (m->entries[i]==NULL) continue;
		cmap_entry *tmp = m->entries[i];
		while (tmp!=NULL) {
			if (cmap_insert(new_map, tmp->key, tmp->data)<0) {
				cmap_free(new_map);
				return NULL;
			}
			tmp=tmp->next;
		}
	}
	new_map->free_data_function=m->free_data_function;
	new_map->free_key_function=m->free_key_function;
	return new_map;
}
