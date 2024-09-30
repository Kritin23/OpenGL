#version 330 core

in layout(location=0) vec4 vPosition;
in layout(location=1) vec4 vNormal;
in layout(location=2) vec3 vColor;
in layout(location=3) vec2 texCoord;

out vec3 Color;
varying vec2 tex;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform vec4 lightSource;

void main()
{
	gl_Position = ProjMat * ViewMat * ModelMat * vPosition;
	Color = vColor;
	tex = texCoord;
}