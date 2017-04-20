#include <stdlib.h>
#include <stdio.h>

#include "map.h"

struct map map_init(unsigned int width, unsigned int height) {
	struct map map;

	map.size.ne = width;
	map.size.nw = height;
	map.tiles = (struct tile*)malloc(sizeof(struct tile) * width * height);

	int ne, nw, i;
	for (ne = 0; ne < map.size.ne; ne++) {
		for (nw = 0; nw < map.size.nw; nw++) {
			i = ne * map.size.nw + nw;
			/* map.tiles[i].type = ne % 2 + nw % 2; */
			map.tiles[i].subtype = random() % 4;
			map.tiles[i].pos = (struct coord_tile) { .ne = ne, .nw = nw };
		}
	}

	return map;
}
