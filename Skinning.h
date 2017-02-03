#pragma once

#include "vector3.h"
#include "Joint.h"

#include "DOF.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "token.h"
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <cmath>

class Skinning
{
public:
	Skinning(char* file, Joint* skel);
	Skinning(std::string file, Joint* skel);
	~Skinning();
	void load(char*, Joint*);
	void joint_load(Joint* root);
	void rLoad(Tokenizer* obj, Skinning* current);
	void displayValues();
	void Update();
	void Draw(GLuint);
	void rotate_x(int, float);
	void rotate_y(int, float);
	void rotate_z(int, float);


	std::vector<Joint*> skel;
	std::vector<glm::vec3> positions, normals;
	std::vector<glm::vec3> current_positions;
	std::vector<glm::vec3> current_normals;
	glm::vec3 color = glm::vec3(50.0f, 50.0f, 50.0f);
	glm::vec3 direction_light = glm::vec3(0.0f, 100.0f, 0.0f);
	//glm::vec3 matAmbient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	//glm::vec3 matDiffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	//glm::vec3 matSpecular = glm::vec3(0.633f, 0.727811f, 0.633f);
	float matIntensity = 1.0f;
	//std::vector<glm::vec3> colors;
	std::vector<std::vector<std::pair<int, float>>> skinWeights;
	std::vector<float> joint, weight;
	std::vector<glm::vec3> triangles;
	std::vector<glm::mat4> bindings;

	//added from previous project
	glm::vec3 viewDir;
	glm::vec3 matAmbient, matDiffuse, matSpecular;
	glm::vec3 pointLightColor, dirLightColor, spotLightColor, lightPos, lightDir, cameraPos;
	std::vector<glm::vec3> colors;
	glm::mat4 toWorld = glm::mat4(1.0f);
	GLfloat minX, maxX, minY, maxY, minZ, maxZ;
	GLfloat avgX, avgY, avgZ;
	GLfloat rangeX, rangeY, rangeZ;
	GLuint VBO, VAO, EBO, NBO;
	GLfloat longestAxis;
	GLfloat light;
	GLuint colorSelection;
	float size;
	float shininess;
	float exponent, cutOff;

};
