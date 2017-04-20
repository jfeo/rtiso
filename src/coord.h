#ifndef __RTISO_COORD_H__
#define __RTISO_COORD_H__

struct coord_tile {
	int nw, up, ne;
};

struct coord_real {
	float nw, up, ne;
};

struct coord_camera {
	double x, y;
};

struct coord_window {
	double x, y;
};

// conversions between coordinates
struct coord_tile coord_tile_from_camera(struct coord_camera pos, int up);
struct coord_tile coord_tile_from_real(struct coord_real pos);
struct coord_real coord_real_from_camera(struct coord_camera pos, float up);
struct coord_window coord_window_from_tile(struct coord_tile pos);
struct coord_camera coord_camera_from_window(struct coord_window pos);
struct coord_window coord_window_from_camera(struct coord_camera pos);
/* coord_tile_t tile_from_real(coord_real_t coord); */
/* coord_real_t real_from_window(coord_window_t coord); */
/* coord_real_t real_from_tile(coord_tile_t coord, coord_real_t frac); */
/* coord_window_t window_from_real(coord_real_t coord); */

#endif /* ifndef __RTISO_COORD_H__ */
