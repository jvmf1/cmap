#include "cmap.h"
#include <stdio.h>

int main() {
	// smap_create(map_size)
	smap *map = smap_create(10);
	smap_insert(map, "key", "value");
	printf("%s\n", (char*)smap_get(map, "key"));
	smap_remove(map, "key");
	printf("%p\n", smap_get(map, "key"));
	smap_free(map);
}
