#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

layout(location = 2) in ivec3 tile_pos;
layout(location = 3) in int tile_type;
layout(location = 4) in int tile_subtype;

out vec2 texcoord_out;

uniform sampler2D tex_sampler;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 camera;

void main() {
	gl_Position = proj * camera * view * model * vec4(position + vec3(tile_pos), 1.0);
	texcoord_out = vec2(float(tile_subtype) / 4.0 + texcoord.x, float(tile_type) / 4.0 + texcoord.y);
}
