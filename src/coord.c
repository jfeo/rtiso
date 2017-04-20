#include "coord.h"

#include "renderer.h"
#include <stdio.h>
#include <math.h>

struct coord_tile coord_tile_from_camera(struct coord_camera pos, int up) {
	struct coord_tile result;
	result = coord_tile_from_real(coord_real_from_camera(pos, (float)up));
	return result;
}

struct coord_tile coord_tile_from_real(struct coord_real pos) {
	struct coord_tile result;
	result.ne = floor(pos.ne);
	result.up = floor(pos.up);
	result.nw = floor(pos.nw);
	return result;
}

struct coord_real coord_real_from_camera(struct coord_camera pos, float up) {
	struct coord_real result;
	float scaled_x = pos.x / (tile_pixel_size.x / 2.0f);
	float scaled_y = pos.y / (tile_pixel_size.y / 2.0f);
	result.ne = (scaled_x - scaled_y) / 2.0f;
	result.nw = - (scaled_x + scaled_y) / 2.0f;
	result.up = up;
	return result;
}

struct coord_camera coord_camera_from_tile(struct coord_tile pos) {
	struct coord_camera result;
	/* result.x = camera_pos.x - pos.nw * tile_pixel_size.x / 2.0f + pos.ne * tile_pixel_size.x / 2.0f; */
	/* result.y = camera_pos.y - pos.nw * tile_pixel_size.y + pos.ne * tile_pixel_size.y; */
	return result;
}

struct coord_window coord_window_from_camera(struct coord_camera pos) {
	struct coord_window result;
	result.x = pos.x + window_size.x / 2.0f;
	result.y = pos.y + window_size.y / 2.0f;
	return result;
}

struct coord_camera coord_camera_from_window(struct coord_window pos) {
	struct coord_camera result;
	struct coord_camera cam_scaled;
	cam_scaled.x = (camera_pos.x * tile_pixel_size.x) / SQRT_2;
	cam_scaled.y = (camera_pos.y * tile_pixel_size.y) / SQRT_2 * 2.0f;

	result.x = pos.x - window_size.x / 2.0f - cam_scaled.x;
	result.y = pos.y - window_size.y / 2.0f + cam_scaled.y;
	return result;
}
