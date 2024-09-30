#version 330 core

in vec3 NormalVector;
in vec4 lSource;
in vec3 Color;
in vec3 VertexPosition;

out vec3 frag_color;

//V = -VertexPosition
//R = L - 2*(L.N)N

void main()
{
	vec3 diffuse = vec3(.8, .8, .8); //diffuse light color
	vec3 specular = vec3(1., 1., 1.);
	vec3 ambient = vec3(.2, .1, .1);
	float shininess = 250;
	float kDiff = 250.0;
	float kSpec = 50.0;
	float LightIntensity = .1;

	//vec3 eye = -1*VertexPosition;
	vec3 spec = vec3(0.0);
	vec3 lVector = vec3(lSource) - VertexPosition;
	//vec3 Reflect = lVector - dot(lVector , NormalVector) * NormalVector;
	//float spec_int = dot(Reflect, eye);
	
	float r2 = pow(length(lVector),2.0);
	vec3 lDir = normalize(lVector);

	float dotProd = dot(NormalVector, lDir);
	float intensity = kDiff * max(dotProd, 0.0) / r2;
	//float intensity = max(dotProd, 0.0);
	if(dotProd >= 0.0)
	{
		vec3 eye = normalize(-1*VertexPosition);
		vec3 h = normalize(lDir + eye );
   		float intSpec = max(dot(h,NormalVector), 0.0);	
        spec = kSpec * specular * intensity * pow(intSpec, shininess) / r2;
	}

	frag_color = (LightIntensity*(intensity*diffuse + spec)*Color + ambient*Color);
}