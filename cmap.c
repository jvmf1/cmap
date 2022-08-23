#include "cmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cmap* cmap_create(size_t size) {
	cmap *map = malloc(sizeof(cmap));
	if (map==NULL) return NULL;
	map->size=size;
	map->free_data_function=NULL;
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

size_t cmap_hash(const char *str, cmap* m) {
	size_t hash = 5381;
	char ch;

	for (size_t i=0;i<strlen(str);i++) {
		ch = str[i];
		hash = ((hash >> 5) + hash) + ch;
	}

	return hash % m->size;
}

cmap_entry* cmap_entry_create(const char *key, void *data) {
	cmap_entry *entry = malloc(sizeof(cmap_entry));
	if (entry==NULL) return NULL;
	entry->key=malloc(strlen(key)+1);
	if (entry->key==NULL) {
		free(entry);
		return NULL;
	}
	strcpy(entry->key, key);
	entry->next=NULL;
	entry->data=data;
	return entry;
}

void cmap_entry_free(cmap *m, cmap_entry *entry) {
	free(entry->key);
	if (m->free_data_function!=NULL)
		m->free_data_function(entry->data);
	free(entry);
}

int cmap_insert(cmap *m, const char *key, void *data) {
	size_t hash = cmap_hash(key, m);
	cmap_entry *tmp = m->entries[hash];
	cmap_entry *last;
	if (tmp==NULL) {
		// is empty
		m->entries[hash]=cmap_entry_create(key, data);
		if (m->entries[hash]==NULL) return -1;
		return 0;
	}

	while (tmp!=NULL) {
		if (strcmp(key, tmp->key)==0) {
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

int cmap_remove(cmap *m, const char *key) {
	size_t hash = cmap_hash(key, m);
	cmap_entry *tmp = m->entries[hash];
	cmap_entry *last=NULL;

	while (tmp!=NULL) {
		if (strcmp(key, tmp->key)==0) {
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

void* cmap_get(cmap *m, const char *key) {
	size_t hash = cmap_hash(key, m);
	cmap_entry *tmp = m->entries[hash];

	while (tmp!=NULL) {
		if (strcmp(key, tmp->key)==0) {
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
			printf("\t\t%p key:'%s' data:%p next:%p\n", tmp, tmp->key, tmp->data, tmp->next);
			tmp=tmp->next;
		}
	}
}

cmap* cmap_resize(cmap *m, size_t size) {
	cmap *new_map = cmap_create(size);
	if (new_map==NULL) return NULL;
	new_map->free_data_function=m->free_data_function;

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
	return new_map;
}
