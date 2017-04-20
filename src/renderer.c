#include "renderer.h"


#include <stdio.h>
#include <math.h>


#include "util.h"
#include "coord.h"
#include "shader.h"


mat4x4 model, model_unit_trans, model_unit_rot;
mat4x4 view;
mat4x4 proj;
mat4x4 camera;

mat4x4 text_proj;

vec3 pos = {0.0f, 0.0f, 0.0f};
vec3 front = {0.0f, 0.0f, 1.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

GLfloat tile_vertices[] = {
	// South
	0.0f, 0.0f, 0.0f, 0.5f / 4.0f, 1.0f / 4.0f,
	// East
	0.0f, 0.0f, 1.0f, 1.0f / 4.0f, 0.5f / 4.0f,
	// West
	1.0f, 0.0f, 0.0f, 0.0f, 0.5f / 4.0f,
	// North
	1.0f, 0.0f, 1.0f, 0.5f / 4.0f, 0.0f
};

GLuint tile_indices[] = {
	0, 2, 3,
	0, 3, 1
};


GLfloat unit_vertices[] = {
	// Top
	// South
	0.0f, 2.0f, 0.0f, 0.5f, 2.0f / 6.0f,
	// East
	0.0f, 2.0f, 1.0f, 1.0f, 1.0f / 6.0f,
	// West
	1.0f, 2.0f, 0.0f, 0.0f, 1.0f / 6.0f,
	// North
	1.0f, 2.0f, 1.0f, 0.5f, 0.0f,

	// Bottom
	// South
	0.0f, 0.0f, 0.0f, 0.5f, 1.0f,
	// East
	0.0f, 0.0f, 1.0f, 1.0f, 5.0f / 6.0f,
	// West
	1.0f, 0.0f, 0.0f, 0.0f, 5.0f / 6.0f,
};

GLuint unit_indices[] = {
	// top face
	0, 2, 3,
	3, 1, 0,

	// right face
	6, 2, 0,
	0, 4, 6,

	// left face
	5, 1, 0,
	0, 4, 5
};

struct coord_window window_size = { .x = 800.0f, .y = 600.0f };
struct coord_camera ortho_size;
struct coord_camera camera_pos  = { .x = 0.0f, .y = 0.0f };
struct coord_window tile_pixel_size = { .x = 16.0f, .y = 8.0f };
struct coord_window mouse_pos = { .x = 0.0f, .y = 0.0f };

GLint renderer_check_gl_error(const char* msg) {
	GLint err = glGetError();
	if (err != 0) {
		printf("Error '%s': %d\n", msg, err);
	}
	return err;
}

void renderer_init(GLFWwindow* win, struct map* map) {
	window = win;

	if (FT_Init_FreeType(&ft)) {
		printf("Could not init freetype library\n");
		return;
	}

	if (FT_New_Face(ft, "assets/DejaVuSans.ttf", 0, &face)) {
		printf("Could not open font\n");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	FT_Set_Pixel_Sizes(face, 0, 14);
	g = face->glyph;

	// vao, vbo_tiles, ibo_tiles, etc.
	glGenVertexArrays(1, &vao_tiles);
	glGenBuffers(1, &vbo_tiles);
	glGenBuffers(1, &ibo_tiles);
	glBindVertexArray(vao_tiles);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile_vertices), tile_vertices, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_tiles);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tile_indices), tile_indices, GL_STREAM_DRAW);
	texture = texture_create("assets/tex/tiles.png");
	renderer_check_gl_error("Texture creation");
	glBindTexture(GL_TEXTURE_2D, texture.id);

	// set up matrices
	mat4x4_identity(camera);
	mat4x4_identity(model);

	mat4x4_look_at(view, pos, front, up);
	mat4x4_rotate_X(view, view, -RTISO_PI / 6);
	mat4x4_rotate_Y(view, view, RTISO_2_PI / 8);

	// create shader
	GLuint vshader = shader_create_from_file(GL_VERTEX_SHADER,
			"assets/shaders/simple.vert.glsl");
	GLuint fshader = shader_create_from_file(GL_FRAGMENT_SHADER,
			"assets/shaders/simple.frag.glsl");

	shader = shader_program(vshader, fshader);
	renderer_check_gl_error("Create program");
	glUseProgram(shader);
	renderer_check_gl_error("Use program");

	// Setup uniforms
	shader_attrib_pos = glGetAttribLocation(shader, "position");
	shader_attrib_tex = glGetAttribLocation(shader, "texcoord");
	shader_attrib_tile_pos = glGetAttribLocation(shader, "tile_pos");
	shader_attrib_tile_type = glGetAttribLocation(shader, "tile_type");
	shader_attrib_tile_subtype = glGetAttribLocation(shader, "tile_subtype");
	shader_mat_model = glGetUniformLocation(shader, "model");
	shader_mat_view = glGetUniformLocation(shader, "view");
	shader_mat_proj = glGetUniformLocation(shader, "proj");
	shader_mat_camera = glGetUniformLocation(shader, "camera");
	renderer_check_gl_error("Get attribute locations");

	glUniformMatrix4fv(shader_mat_view, 1, GL_FALSE, (GLfloat*)view);
	glUniformMatrix4fv(shader_mat_model, 1, GL_FALSE, (GLfloat*)model);
	glUniformMatrix4fv(shader_mat_camera, 1, GL_FALSE, (GLfloat*)camera);
	renderer_check_gl_error("glUniform");

	glEnableVertexAttribArray(shader_attrib_pos);
	glEnableVertexAttribArray(shader_attrib_tex);
	renderer_check_gl_error("glEnableVertexAttribArray");

	glVertexAttribPointer(shader_attrib_pos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(shader_attrib_tex, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GLfloat)));
	renderer_check_gl_error("glVertexAttribPointer: Vertex");

	glGenBuffers(1, &vbo_tiles);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct tile) * map->size.nw * map->size.ne, map->tiles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(shader_attrib_tile_pos);
	glEnableVertexAttribArray(shader_attrib_tile_type);
	glEnableVertexAttribArray(shader_attrib_tile_subtype);
	renderer_check_gl_error("glEnableVertexAttribArray: Instance");

	glVertexAttribIPointer(shader_attrib_tile_pos, 3, GL_INT, sizeof(struct tile), 0);
	glVertexAttribIPointer(shader_attrib_tile_type, 1, GL_INT, sizeof(struct tile),
			(GLvoid*)(sizeof(struct coord_tile)));
	glVertexAttribIPointer(shader_attrib_tile_subtype, 1, GL_INT, sizeof(struct tile),
			(GLvoid*)(sizeof(struct coord_tile) + sizeof(int)));
	renderer_check_gl_error("glVertexAttribPointer: Instance");

	// setup instancing
	glVertexAttribDivisor(shader_attrib_tile_pos, 1);
	glVertexAttribDivisor(shader_attrib_tile_type, 1);
	glVertexAttribDivisor(shader_attrib_tile_subtype, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Text rendering stuff
	glGenVertexArrays(1, &vao_text);
	glGenBuffers(1, &vbo_text);
	glGenTextures(1, &tex_text);
	glBindVertexArray(vao_text);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
	glBindTexture(GL_TEXTURE_2D, tex_text);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// create shader
	vshader = shader_create_from_file(GL_VERTEX_SHADER,
			"assets/shaders/text.vert.glsl");
	fshader = shader_create_from_file(GL_FRAGMENT_SHADER,
			"assets/shaders/text.frag.glsl");


	shader_text = shader_program(vshader, fshader);
	renderer_check_gl_error("Create program");
	glUseProgram(shader_text);
	renderer_check_gl_error("Use program");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnableVertexAttribArray(shader_text_attrib_pos);
	glVertexAttribPointer(shader_text_attrib_pos, 4, GL_FLOAT, GL_FALSE, 0, 0);
	renderer_check_gl_error("Text: attrib pointer pos");

	shader_text_color = glGetUniformLocation(shader_text, "textColor");
	GLfloat color_white[3] = {1, 1, 1};
	glUniform3fv(shader_text_color, 1, color_white);
	shader_text_mat_proj = glGetUniformLocation(shader_text, "proj");
	glUniformMatrix4fv(shader_text_mat_proj, 1, GL_FALSE, (GLfloat*)text_proj);

	glBindVertexArray(0);

	renderer_update_projection();
	renderer_check_gl_error("update proj");

	mat4x4_identity(model_unit_trans);
	mat4x4_identity(model_unit_rot);

	glGenVertexArrays(1, &vao_unit);
	glGenBuffers(1, &vbo_unit);
	glGenBuffers(1, &ibo_unit);

	glBindVertexArray(vao_unit);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_unit);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_unit);
	glUseProgram(shader);

	glEnableVertexAttribArray(shader_attrib_pos);
	glEnableVertexAttribArray(shader_attrib_tex);
	renderer_check_gl_error("glEnableVertexAttribArray");

	glVertexAttribPointer(shader_attrib_pos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(shader_attrib_tex, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT),
			(GLvoid*)(3 * sizeof(GLfloat)));

	glBufferData(GL_ARRAY_BUFFER, sizeof(unit_vertices), unit_vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unit_indices), unit_indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void renderer_update_map(struct map* map) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct tile) * map->size.nw * map->size.ne, map->tiles, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderer_draw_unit(struct unit* unit) {
	mat4x4 model_unit;
	glBindVertexArray(vao_unit);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_unit);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_unit);
	glBindTexture(GL_TEXTURE_2D, unit->tex->id);

	mat4x4_translate(model_unit_trans, unit->pos.nw, unit->pos.up, unit->pos.ne);
	mat4x4_rotate_Y(model_unit_rot, model_unit_rot, 0.00f);
	mat4x4_mul(model_unit, model_unit_trans, model_unit_rot);
	glUniformMatrix4fv(shader_mat_model, 1, GL_FALSE, model_unit[0]);

	renderer_check_gl_error("unit model uniform");

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	renderer_check_gl_error("unit draw");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderer_camera_move(float x, float y) {
	camera_pos.x += x;
	camera_pos.y += y;
}

void renderer_camera_update() {
	glUseProgram(shader);
	mat4x4_translate(camera, camera_pos.x, camera_pos.y, 0);
	glUniformMatrix4fv(shader_mat_camera, 1, GL_FALSE, camera[0]);
	glUseProgram(0);
}

double ang = 0.0f;

void renderer_render(float tdiff, struct map* map, struct unit* test_unit) {
	struct coord_tile mpos_tile;
	struct coord_camera mpos_cam;
	struct coord_window mpos_win;
	struct coord_real mpos_real;
	glfwGetCursorPos(window, &mpos_win.x, &mpos_win.y);
	mpos_cam = coord_camera_from_window(mpos_win);
	mpos_tile = coord_tile_from_camera(mpos_cam, 0.0f);
	mpos_real = coord_real_from_camera(mpos_cam, 0.0f);

	// Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	renderer_camera_update();
	renderer_draw_map(map);

	if (test_unit)
		renderer_draw_unit(test_unit);

	char cstr[64], mstr_w[64], mstr_c[64], mstr_t[64], mstr_r[64], fpsstr[64];
	sprintf(fpsstr, "FPS: %0.3f", 1000.0f  / tdiff);
	sprintf(mstr_w, "Mouse (window): %0.2f %0.2f", mpos_win.x, mpos_win.y);
	sprintf(mstr_c, "Mouse (camera): %0.2f %0.2f", mpos_cam.x, mpos_cam.y);
	sprintf(mstr_t, "Mouse (tile): %d %d", mpos_tile.nw, mpos_tile.ne);
	sprintf(mstr_r, "Mouse (real): %0.2f %0.2f", mpos_real.nw, mpos_real.ne);
	sprintf(cstr, "Camera: %0.0f %0.0f", camera_pos.x, camera_pos.y);
	renderer_draw_text(fpsstr, 5.0f, window_size.y - 19.0f);
	renderer_draw_text(mstr_w, 5.0f, 24.0f);
	renderer_draw_text(mstr_c, 5.0f, 43.0f);
	renderer_draw_text(mstr_t, 5.0f, 62.0f);
	renderer_draw_text(mstr_r, 5.0f, 81.0f);
	renderer_draw_text(cstr, 5.0f, 5.0f);

	glfwSwapBuffers(window);
}

void renderer_draw_map(struct map* map) {
	glBindVertexArray(vao_tiles);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_tiles);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glUseProgram(shader);

	glUniformMatrix4fv(shader_mat_model, 1, GL_FALSE, model[0]);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, map->size.nw * map->size.ne);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void renderer_draw_text(const char *text, float x, float y) {
	float sx = 1.0f;
	float sy = 1.0f;

	glBindVertexArray(vao_text);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
	glBindTexture(GL_TEXTURE_2D, tex_text);
	glUseProgram(shader_text);

	const char *p;
	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x/64) * sx;
		y += (g->advance.y/64) * sy;
	}
	glBindVertexArray(0);
}

void renderer_deinit() {
	glfwTerminate(); // deinitialize glfw
}

void renderer_update_projection() {
	glUseProgram(shader);
	float aspect = window_size.y / window_size.x;
	float screen_tiles = window_size.x / tile_pixel_size.x;
	ortho_size.x = screen_tiles * SQRT_2;
	ortho_size.y = ortho_size.x * aspect;
	mat4x4_ortho(proj, -ortho_size.x/2, ortho_size.x/2, -ortho_size.y/2,
			   ortho_size.y/2, 0.0f, 1000.0f);
	glUniformMatrix4fv(shader_mat_proj, 1, GL_FALSE, (GLfloat*)proj);

	glUseProgram(shader_text);
	mat4x4_ortho(text_proj, 0, window_size.x, 0, window_size.y, 0, 1.0f);
	glUniformMatrix4fv(shader_text_mat_proj, 1, GL_FALSE, (GLfloat*)text_proj);
}

void printm(mat4x4 m) {
	for (int i = 0; i < 4; i++) {
		printf("%f %f %f %f\n", m[0][i], m[1][i], m[2][i], m[3][i]);
	}
}
