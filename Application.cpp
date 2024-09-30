#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

#include <map>

#include "gl_framework.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void compute_rotate(glm::mat4& rot, float theta)
{
	float cos = glm::cos(theta);
	float sin = glm::sin(theta);
	/*rot = {
		cos,   sin, 0.0f, 0.0f,
		-sin,   cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};*/
	rot = glm::mat4(1);
}
void compute_view(glm::mat4& view, float theta)
{
	//theta = glm::pi<float>() / 2;
	float cos = glm::cos(theta);
	float sin = glm::sin(theta); 
	view = {
		 cos,  0.0f,  sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin, 0.0f,  cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	//view = glm::mat4(1.0f);
}

float vertices[178200];
unsigned int indices[96120];



void genSphere(float radius, int lat, int lon, float* vertices, unsigned int* indices)
{
	ASSERT(lat * lon < 100000);
	
	
	const float PI = glm::pi<float>();
	const float dPhi = 2 * PI / lon;
	const float dTheta = PI / (lat - 1);

	//std::cout << "dPhi = " << dPhi << "\ndTheta = " << dTheta << "\n";

	int vertex_idx = 0;
	const glm::vec3 clr = { 1.0f, 0.25f, 0.25f };

	//generate vertices
	for (int i = 0; i < lat; i++)
	{
		for (int j = 0; j < lon; j++)
		{
			float theta = i * dTheta;
			float phi = j * dPhi;
			glm::vec4 ver = { radius * glm::sin(theta) * glm::cos(phi),
							radius * glm::sin(theta) * glm::sin(phi),
							radius * glm::cos(theta) ,1.0f};
			vertices[vertex_idx] =     ver.x;
			vertices[vertex_idx + 1] = ver.y;
			vertices[vertex_idx + 2] = ver.z;
			vertices[vertex_idx + 3] = ver.w;

			vertices[vertex_idx + 4] = ver.x/radius;
			vertices[vertex_idx + 5] = ver.y/radius;
			vertices[vertex_idx + 6] = ver.z/radius;
			vertices[vertex_idx + 7] = ver.w;
			vertices[vertex_idx + 8] = clr.r;
			vertices[vertex_idx + 9] = clr.g;
			vertices[vertex_idx + 10]= clr.b;
			vertex_idx += 11;

		}
	}
	std::cout << "Vertices: " << vertex_idx << "\n";
	
	unsigned int indices_idx = 0;

	//generate index buffer data
	for (int i = 0; i < lat - 1; i++)
	{
		for (int j = 0; j < lon-1; j++)
		{
			indices[indices_idx] = i * lon + j;
			indices[indices_idx + 1] = (i + 1) * lon + j;
			indices[indices_idx + 2] = (i + 1) * lon + j+1;

			indices_idx += 3;
			indices[indices_idx] = i * lon + j;
			indices[indices_idx + 1] = (i + 1) * lon + j + 1;
			indices[indices_idx + 2] = i * lon + j + 1;
			indices_idx += 3;
		}
		indices[indices_idx] = i * lon + lon - 1;
		indices[indices_idx + 1] = (i + 1) * lon + lon - 1;
		indices[indices_idx + 2] = (i + 1) * lon;

		indices_idx += 3;
		indices[indices_idx] = i * lon + lon - 1;
		indices[indices_idx + 1] = (i + 1) * lon;
		indices[indices_idx + 2] = i * lon;
		indices_idx += 3;
	}
	std::cout << "Indices: " << indices_idx << "\n";
	std::cout << std::endl << "\n";
	//vbp = new VertexBuffer(vertices, 11 * sizeof(float) * lat * lon);
	//ibp = new IndexBuffer(indices, (lat - 1) * lon * 6);

}

void genSphereIco(unsigned int iter, float* vertices, unsigned int* indices, unsigned int& vSize, unsigned int& iSize)
{
	int currSize = 12;
	int currIndSize = 60;

	float p = (1.0 + sqrt(5)) / 2; //golden ratio

	struct Vertex {
		float x;
		float y;
		float z;
	};

	std::vector<Vertex> vertices_s;
	vertices_s.reserve(12);

	vertices_s.push_back({ -1,  p,  0 });
	vertices_s.push_back({ 1,   p,  0 });
	vertices_s.push_back({ -1, -p,  0 });
	vertices_s.push_back({ 1,  -p,  0 });

	vertices_s.push_back({ 0, -1,  p });
	vertices_s.push_back({ 0,  1,  p });
	vertices_s.push_back({ 0, -1, -p });
	vertices_s.push_back({ 0,  1, -p });

	vertices_s.push_back({  p , 0, -1});
	vertices_s.push_back({  p , 0,  1});
	vertices_s.push_back({ -p , 0, -1});
	vertices_s.push_back({ -p , 0,  1});

	for (int i=0;i<vertices_s.size();i++)
	{
		auto& v = vertices_s[i];
		float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		v.x /= norm;
		v.y /= norm;
		v.z /= norm;
	}	

	std::map<std::pair<unsigned int, unsigned int>, unsigned int> edgeMidPoints;

	std::vector<unsigned int> indices_temp = {
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,
		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,
		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,
		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};



	for (int i = 0; i < iter; i++)
	{
		std::vector<unsigned int> newIndices;
		newIndices.reserve(4 * indices_temp.size());

		for (int i = 0; i < indices_temp.size();)
		{
			unsigned int v1 = indices_temp[i++], v2 = indices_temp[i++], v3 = indices_temp[i++];
			unsigned int v4, v5, v6;
			if (edgeMidPoints.find({ v2,v1 }) != edgeMidPoints.end())
			{
				v4 = edgeMidPoints[{v2, v1}];
			}
			else
			{
				Vertex v;
				v.x = vertices_s[v1].x + vertices_s[v2].x;
				v.y = vertices_s[v1].y + vertices_s[v2].y;
				v.z = vertices_s[v1].z + vertices_s[v2].z;
				float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

				v.x /= norm;
				v.y /= norm;
				v.z /= norm;
				v4 = vertices_s.size();
				vertices_s.push_back(v);
				edgeMidPoints[{v1, v2}] = v4;
			}
			if (edgeMidPoints.find({ v3,v2 }) != edgeMidPoints.end())
			{
				v5 = edgeMidPoints[{v3, v2}];
			}
			else
			{
				Vertex v;
				v.x = vertices_s[v2].x + vertices_s[v3].x;
				v.y = vertices_s[v2].y + vertices_s[v3].y;
				v.z = vertices_s[v2].z + vertices_s[v3].z;
				float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
				v.x /= norm;
				v.y /= norm;
				v.z /= norm;
				vertices_s.push_back(v);
				v5 = vertices_s.size() - 1;

				edgeMidPoints[{v2, v3}] = v5;
			}
			if (edgeMidPoints.find({ v1,v3 }) != edgeMidPoints.end())
			{
				v6 = edgeMidPoints[{v1, v3}];
			}
			else
			{
				Vertex v;
				v.x = vertices_s[v1].x + vertices_s[v3].x;
				v.y = vertices_s[v1].y + vertices_s[v3].y;
				v.z = vertices_s[v1].z + vertices_s[v3].z;
				float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
				v.x /= norm;
				v.y /= norm;
				v.z /= norm;
				vertices_s.push_back(v);
				v6 = vertices_s.size() - 1;

				edgeMidPoints[{v3, v1}] = v6;
			}

			newIndices.push_back(v1); newIndices.push_back(v4);newIndices.push_back(v6);
			newIndices.push_back(v2); newIndices.push_back(v5); newIndices.push_back(v4);
			newIndices.push_back(v3); newIndices.push_back(v6); newIndices.push_back(v5);
			newIndices.push_back(v4); newIndices.push_back(v5); newIndices.push_back(v6);

			
		}

		indices_temp = newIndices;
		edgeMidPoints.clear();
	}
	
	unsigned int v_ptr = 0;
	for (int i = 0; i < vertices_s.size(); i++, v_ptr += 11)
	{
		auto& v = vertices_s[i];
		vertices[v_ptr + 0] = v.x;
		vertices[v_ptr + 1] = v.y;
		vertices[v_ptr + 2] = v.z;
		vertices[v_ptr + 3] = 1.0f;

		vertices[v_ptr + 4] = v.x;
		vertices[v_ptr + 5] = v.y;
		vertices[v_ptr + 6] = v.z;
		vertices[v_ptr + 7] = 1.0f;

		vertices[v_ptr + 8] = 1.0f;
		vertices[v_ptr + 9] = 1.0f;
		vertices[v_ptr + 10] = 1.0f;
		//std::cout << v.x << "\t" << v.y << "\t" << v.z << "\n";
	}

	vSize = v_ptr;
	for (int i = 0; i < indices_temp.size(); i++)
	{
		indices[i] = indices_temp[i];
		//std::cout << indices[i] << "\t";
		//if ((i + 1) % 3 == 0)
			//std::cout << "\n";
	}
	iSize = indices_temp.size();

}

void genCube(float* vertices, unsigned int* indices)
{
	glm::vec3 vertex_data[] =
	{
		glm::vec3(-0.5, -0.5, -0.5),
		glm::vec3(0.5, -0.5, -0.5),
		glm::vec3(0.5, 0.5, -0.5),
		glm::vec3(-0.5, 0.5, -0.5),
		glm::vec3(-0.5, -0.5, 0.5),
		glm::vec3(0.5, -0.5, 0.5),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(-0.5, 0.5, 0.5)
	};

	glm::vec3 normal_data[] =
	{
		glm::vec3(0., 0., -1.),
		glm::vec3(1., 0., 0.),
		glm::vec3(0., 0., 1.),
		glm::vec3(-1., 0., 0.),
		glm::vec3(0., 1., 0.),
		glm::vec3(0., -1., 0.)
	};

	glm::vec2 tex_data[] =
	{
		glm::vec2(0.0, 0.0),
		glm::vec2(1.0, 0.0),
		glm::vec2(0.0, 1.0),
		glm::vec2(1.0, 1.0)

	};

	uint8_t index_data[6 * 6] =
	{
		0, 3, 1, 1, 3, 2,
		1, 2, 5, 5, 2, 6,
		7, 4, 6, 6, 4, 5,
		3, 0, 7, 7, 0, 4,
		6, 2, 7, 7, 2, 3,
		1, 5, 0, 0, 5, 4
	};

	for (int i = 0; i < 36; i++)
	{
		//position
		vertices[i * 13 + 0] = vertex_data[index_data[i]].x;
		vertices[i * 13 + 1] = vertex_data[index_data[i]].y;
		vertices[i * 13 + 2] = vertex_data[index_data[i]].z;
		vertices[i * 13 + 3] = 1.0;

		//normal
		vertices[i * 13 + 4] = normal_data[i/6].x;
		vertices[i * 13 + 5] = normal_data[i/6].y;
		vertices[i * 13 + 6] = normal_data[i/6].z;
		vertices[i * 13 + 7] = 1.0;

		//color
		vertices[i * 13 + 8] = 1.0;
		vertices[i * 13 + 9] = 1.0;
		vertices[i * 13 + 10]= 1.0;
		
		//texture coordinates
		vertices[i * 13 + 11] = tex_data[(i % 6 == 5) ? (i % 3) : 3].x;
		vertices[i * 13 + 12] = tex_data[(i % 6 == 5) ? (i % 3) : 3].y;

	}
	for (int i = 0; i < 36; i++)
	{
		indices[i] = i;
	}


}

int main(void)
{
	GLFWwindow* window;


	if (!glfwInit())
		return -1;

	//We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//We don't want the old OpenGL 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	


	window = glfwCreateWindow(640, 640, "Sphere", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLCall(glfwSwapInterval(1));

	if (glewInit() != GLEW_OK)
	{
		return 1;
	}
	{
		GLCall(glEnable(GL_DEPTH_TEST));
	
		VertexBufferLayout layout;
		layout.Push<float>(4);	//position
		layout.Push<float>(4);	//normal
		layout.Push<float>(3);	//color
		//layout.Push<float>(2);	//texture coordinates

		const int lat = 45, lon = 85;

		unsigned int vSize, iSize;

		//float vertices[23375];
		//unsigned int indices[12240];

		//genSphere(3.0, lat, lon, &vertices[0], &indices[0]);
		genSphereIco(4, vertices, indices, vSize, iSize);
		//genCube(vertices, indices);

		//Square
		/*float vertices[4 * 13] =
		{
			 //pos							normal							clr						tex
			 0.0,	0.0,	0.0,	1.0,	0.0,	0.0,	0.0,	0.0,	1.0,	0.0,	0.0,	0.0,	0.0,
			 1.0,	0.0,	0.0,	1.0,	0.0,	0.0,	0.0,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,
			 1.0,	1.0,	0.0,	1.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	1.0,	1.0,	1.0,
			 0.0,	1.0,	0.0,	1.0,	0.0,	0.0,	0.0,	0.0,	1.0,	1.0,	1.0,	0.0,	1.0

		};
		/*float vertices[7 * 4] = {
			0.0,	0.0,	0.0,	1.0,	1.0,	0.0,	0.0,
			1.0,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,
			1.0,	1.0,	0.0,	1.0,	0.0,	0.0,	1.0,
			0.0,	1.0,	0.0,	1.0,	1.0,	1.0,	1.0
		};
		unsigned int indices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};*/

		VertexBuffer vb(vertices, vSize*sizeof(float));
		IndexBuffer ib(indices, iSize);


		VertexArray vao;
		vao.AddBuffer(vb, layout);
		vao.Bind();
		ib.Bind();

		

		glm::mat4 ModelM = glm::scale(glm::one<glm::mat4>(), glm::vec3(3., 5., 3.));
		
		glm::vec3 eye = glm::vec3( 20.0f, 0.0f, 0.0f);
		glm::vec3 centre = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::mat4 ViewM = glm::lookAt(eye,
									centre,
									up);
		glm::mat4 ProjM = glm::perspective(glm::pi<float>()/4, 1.0f, 0.01f, -100.0f);
		//glm::mat4 ProjM = glm::ortho(-0.5, 1.5, -0.5, 1.5, -1.0, 100.0);
		Shader shader(std::string("res/VertexShader_perPixel.glsl"), std::string("res/FragmentShader_perPixel.glsl"));
		shader.Bind();

		shader.SetUniformMat4f("ViewMat", ViewM);
		shader.SetUniformMat4f("ProjMat", ProjM);
		
		//unsigned int texture = LoadTexture("res/Textures/Image.bmp", 256, 256);

		//GLCall(glActiveTexture(GL_TEXTURE0));
		//GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		//shader.SetUniform1i("texture", 0);

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Light Source
		glm::vec4 Light = glm::vec4(5.0f, -7.0f, 0.0f, 1.0f);
		shader.SetUniform4f("lightSource", Light);

		//TEST
		glm::vec4 v = glm::vec4(1.0, 1.0, 0.0, 1.0);
		glm::vec4 v2 = ProjM * ViewM * ModelM * v;
		std::cout << "( " << v.x << " " << v.y << " " << v.z << " " << v.w << " )\n";

		Renderer renderer;


		while (!glfwWindowShouldClose(window))
		{
			shader.SetUniformMat4f("ViewMat", ViewM);
			renderer.Clear();
			shader.SetUniformMat4f("ModelMat", ModelM);
			renderer.Draw(vao, ib, shader);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
	
}