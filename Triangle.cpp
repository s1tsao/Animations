#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle(glm::vec3 position, glm::vec3 normals, int numSkinWeights, std::vector<float> joint, std::vector<float> weight, glm::vec3 triangles, glm::mat4 bindings)
{
	this->positions = position;
	this->normals = normals;
	this->numSkinWeights = numSkinWeights;
	this->joint = joint;
	this->weight = weight;
	this->triangles = triangles;
	this->bindings = bindings;
}


Triangle::~Triangle()
{
}
