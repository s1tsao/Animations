#pragma once
#include "vector3.h"
#include "DOF.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "token.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Triangle
{
public:
	Triangle(glm::vec3 position, glm::vec3 normals, int numSkinWeights, std::vector<float> joint, std::vector<float> weight, glm::vec3 triangles, glm::mat4 bindings);
	~Triangle();
	glm::vec3 positions, normals;
	int numSkinWeights;
	std::vector<float> joint, weight;
	glm::vec3 triangles;
	glm::mat4 bindings;
};

