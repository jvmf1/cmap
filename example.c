#include "cmap.h"
#include <stdio.h>

int main() {
	// cmap_create(map_size)
	cmap *map = cmap_create(10);
	cmap_insert(map, "key", "value");
	printf("%s\n", (char*)cmap_get(map, "key"));
	cmap_remove(map, "key");
	printf("%p\n", cmap_get(map, "key"));
	cmap_free(map);
}
