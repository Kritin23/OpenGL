#version 330 core

in vec3 Color;
varying vec2 tex;

uniform sampler2D texture;

void main()
{
	vec4 tex_clr = texture2D(texture, tex);
	gl_FragColor = tex_clr;
}