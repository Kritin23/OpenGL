#version 330 core

in layout(location=0) vec4 vPosition;
in layout(location=1) vec4 vNormal;
in layout(location=2) vec3 vColor;

out vec3 NormalVector;
out vec4 lSource;
out vec3 Color;
out vec3 VertexPosition;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform vec4 lightSource;

void main()
{
	gl_Position = ViewMat * ModelMat * vPosition;

	mat3 NormalMat = mat3(ViewMat * ModelMat);

	NormalVector = normalize(NormalMat * vec3(vNormal));
	lSource = ViewMat * lightSource;
	Color = vColor;
	VertexPosition = vec3(gl_Position);

	gl_Position = ProjMat * gl_Position;

	//gl_Position.x = (1+0.1*pow(sin(gl_Position.y),2))*gl_Position.x;
};
