#pragma once
#include "vector3.h"
#include "DOF.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "token.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Joint
{
public:
	Joint(char* file);
	Joint(std::string file);
	Joint();
	~Joint();
	void Draw();
	void Update();
	void Reset();
	void Value();
	void rotate_x(float);
	void rotate_y(float);
	void rotate_z(float);
	
	std::vector<glm::vec3> corners;
	char name[256];
    glm::mat4 World = glm::mat4(1.0f);
	glm::mat4 local = glm::mat4(1.0f);
	Vector3 offset = Vector3(0,0,0);
	Vector3 boxmin = Vector3(-0.1, -0.1, -0.1);
	Vector3 boxmax = Vector3(0.1, 0.1, 0.1);
	
	DOF rotx = DOF(0.0, -10000.0, 10000.0);
	DOF roty = DOF(0.0, -10000.0, 10000.0);
	DOF rotz = DOF(0.0, -10000.0, 10000.0);
	std::vector<Joint*> children, all_joint;
	

private:
	void Draw(glm::mat4, glm::mat4, glm::mat4, glm::mat4, glm::mat4);
	Joint* Load(char* file);
	Joint* rLoad(Tokenizer* obj, Joint* current, Joint* root);
	void Update(glm::mat4, glm::mat4);
};

