/*
 * file: main.c
 * author: jfeo (jens@feodor.dk)
 * date: 17/08/2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "renderer.h"
#include "util.h"
#include "map.h"

struct map map;
int key_down[256];

struct coord_real to = { .ne = 30.0f, .nw = 15.0f };
struct unit* test_unit;

void key_callback(GLFWwindow* win, int key, int scan, int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		key_down[key] = 1;
		break;
	case GLFW_RELEASE:
		key_down[key] = 0;
		break;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	window_size.x = width;
	window_size.y = height;
	glViewport(0, 0, width, height);
	renderer_update_projection();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	struct coord_window mpos;
	struct coord_camera mpos_cam;
	struct coord_tile mpos_tile;
	struct coord_real mpos_real;
	glfwGetCursorPos(window, &mpos.x, &mpos.y);
	mpos_cam = coord_camera_from_window(mpos);
	mpos_tile = coord_tile_from_camera(mpos_cam, 0);
	mpos_real = coord_real_from_camera(mpos_cam, 0.0f);
	switch (action) {
	case GLFW_PRESS:
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			printf("Coord Window: %f %f\n", mpos.x, mpos.y);
			printf("Coord Camera: %f %f\n", mpos_cam.x, mpos_cam.y);
			printf("Coord Tile:   %d %d\n", mpos_tile.ne, mpos_tile.nw);
			printf("Coord Real:   %f %f\n", mpos_real.ne, mpos_real.nw);
			printf("Camera Pos:   %f %f\n", camera_pos.x, camera_pos.y);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			test_unit->action->data.move.to.ne = mpos_real.ne;
			test_unit->action->data.move.to.nw = mpos_real.nw;
			break;
		}
		break;
	}
}


void handle_down_keys() {
	if (key_down[GLFW_KEY_A]) {
		renderer_camera_move(2.0f, 0.0f);
	}
	if (key_down[GLFW_KEY_D]) {
		renderer_camera_move(-2.0f, 0.0f);
	}
	if (key_down[GLFW_KEY_W]) {
		renderer_camera_move(0.0f, -2.0f);
	}
	if (key_down[GLFW_KEY_S]) {
		renderer_camera_move(0.0f, 2.0f);
	}
}

int main(int argc, char *argv[]) {
	glfwInit(); // initialize glfw

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "ctw", NULL, NULL); // Windowed
	glfwMakeContextCurrent(window); // select window

	// initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	srandom(time(NULL));

	for (int i = 0; i < 256; i++) {
		key_down[i] = 0;
	}

	map = map_init(256, 256);

	renderer_init(window, &map);

	glfwSetWindowSizeCallback(window, *window_size_callback);
	glfwSetKeyCallback(window, *key_callback);
	glfwSetMouseButtonCallback(window, *mouse_button_callback);

	struct texture texture_unit = texture_create("assets/tex/units.png");
	struct coord_real upos = { .nw = 1.0f, .up = 0.0f, .ne = 1.0f };
	test_unit = unit_create(&texture_unit, upos);
	test_unit->action = (struct action*)malloc(sizeof(struct action));
	test_unit->action->type = MOVE;
	test_unit->action->data.move.to = to;

	struct timespec ts_before, ts_after;
	double ms = 0.0f;
	while(!glfwWindowShouldClose(window)) {
		clock_gettime(CLOCK_REALTIME, &ts_before);
		renderer_render(ms, &map, test_unit);
		if (test_unit->action->type == MOVE) {
			float ratio = 1.0f / sqrt(powf(test_unit->action->data.move.to.ne - test_unit->pos.ne, 2.0f) + \
				 	          powf(test_unit->action->data.move.to.nw - test_unit->pos.nw, 2.0f));
			test_unit->pos.ne += ratio * (test_unit->action->data.move.to.ne - test_unit->pos.ne) * ms / 10.0f;
			test_unit->pos.nw += ratio * (test_unit->action->data.move.to.nw - test_unit->pos.nw) * ms / 10.0f;
		}
		glfwPollEvents();
		handle_down_keys();
		clock_gettime(CLOCK_REALTIME, &ts_after);
		ms = (ts_after.tv_sec - ts_before.tv_sec) * 1000.0f
		   + (ts_after.tv_nsec - ts_before.tv_nsec) / 1000000.0f;
	}

	renderer_deinit();

	return EXIT_SUCCESS;
}
