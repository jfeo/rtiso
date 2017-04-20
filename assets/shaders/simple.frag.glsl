#version 150 core

in vec2 texcoord_out;
out vec4 color;

uniform sampler2D tex_sampler;

void main() {
	color = texture(tex_sampler, texcoord_out);
	/* color = vec4(1.0, 1.0, 1.0, 1.0); */
}
