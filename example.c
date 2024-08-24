#include "cmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool compare_str(void *s, void*s2) {
	if (strcmp((char*)s, (char*)s2) == 0)
		return true;
	return false;
}

size_t hash_str(void *p) {
	size_t hash = 5381;
	char *str = (char*)p;
	char ch;

	for (size_t i=0;i<strlen(str);i++) {
		ch = str[i];
		hash = ((hash >> 5) + hash) + ch;
	}

	return hash;
}

bool compare_int(void *p, void *p2) {
	return *(int*)p == *(int*)p2;
}

size_t hash_int(void *p) {
	return *(int*)p;
}

int main() {
	// cmap_create(map_size, compare_function, hash_function, free_data_function, free_key_function)
	cmap *map = cmap_create(10, compare_str, hash_str, NULL, NULL);
	cmap_insert(map, "key", "value");
	printf("%s\n", (char*)cmap_get(map, "key"));
	cmap_remove(map, "key");
	printf("%p\n", cmap_get(map, "key"));
	cmap_free(map);

	cmap *int_map = cmap_create(10, compare_int, hash_int, NULL, NULL);
	cmap_insert(int_map, (int[]){15}, "fifteen");
	printf("%s\n", (char*)cmap_get(int_map, (int[]){15}));
	cmap_free(int_map);
}
