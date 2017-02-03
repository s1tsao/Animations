#include "stdafx.h"
#include "Joint.h"


Joint::Joint(char* file)
{
	Load(file);
	Update();
}
Joint::Joint(std::string file)
{
	Load((char*)file.c_str());
	Update();
}
Joint::Joint()
{
}

Joint::~Joint()
{
}
Joint* Joint::Load(char* file) {
	Tokenizer* obj = new Tokenizer();
	std::cerr << "Load call\n";
	if (obj->Open(file))
	{
		//Joint* current;
		char token[256] = "";
		obj->GetToken(token);
		if (strcmp("balljoint", token) == 0) {
			//current = new Joint();
			memset(token, 0, 256);
			obj->GetToken(token);
			strcpy(this->name, token);
			this->all_joint.push_back(this);
			return rLoad(obj, this, this);
		}
	}
}
Joint* Joint::rLoad(Tokenizer* obj, Joint* current, Joint* root) {

	char token[256] = "";
	obj->GetToken(token);
	std::cerr << "rLoad call\n";
	while (strcmp(token, "}") != 0) {

		if (strcmp(token, "{") == 0) {
		}
		else if (strcmp(token, "offset") == 0) {
			current->offset = Vector3(obj->GetFloat(), obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp(token, "boxmin") == 0) {
			current->boxmin = Vector3(obj->GetFloat(), obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp(token, "boxmax") == 0) {
			current->boxmax = Vector3(obj->GetFloat(), obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp(token, "pose") == 0) {
			current->rotx.setValue(obj->GetFloat());
			current->roty.setValue(obj->GetFloat());
			current->rotz.setValue(obj->GetFloat());
		}
		else if (strcmp(token, "rotxlimit") == 0) {
			current->rotx.SetMinMax(obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp(token, "rotylimit") == 0) {
			current->roty.SetMinMax(obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp(token, "rotzlimit") == 0) {
			current->rotz.SetMinMax(obj->GetFloat(), obj->GetFloat());
		}
		else if (strcmp("balljoint", token) == 0) {
			std::cerr << "recursive call" << std::endl;
			Joint* child = new Joint();
			root->all_joint.push_back(child);
			child->offset = current->offset;
			memset(token, 0, 256);
			obj->GetToken(token);
			strcpy(child->name, token);

			current->children.push_back(rLoad(obj, child, root));
		}
		memset(token, 0, 256);
		obj->GetToken(token);
	}
	current->Value();
	return current;
}

void Joint::rotate_x(float d) {
	rotx.rot(d);
	Update();
}
void Joint::rotate_y(float d) {
	roty.rot(d);
	Update();
}
void Joint::rotate_z(float d) {
	rotz.rot(d);
	Update();
}

void Joint::Update() {
	this->Update(glm::mat4(1.0f), glm::mat4(1.0f));
}

void Joint::Update(glm::mat4 p_local, glm::mat4 p_world) {
	corners.clear();
	glm::mat4 local = glm::translate(glm::mat4(1.f), glm::vec3(offset.x, offset.y, offset.z))
		* glm::rotate(glm::mat4(1.0f), (float)rotz.getValue(), glm::vec3(0.0, 0.0, 1.0))
		* glm::rotate(glm::mat4(1.0f), (float)roty.getValue(), glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)rotx.getValue(), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 world = p_world * (local);

	this->World = world;
	this->local = local;
	const float *pSource = (const float*)glm::value_ptr(local);
	/*printf("%f, %f, %f\n", local[3], local[7], local[11]);*/
	corners.push_back(world* glm::vec4(boxmin.x, boxmin.y, boxmin.z, 1.0)
	);

	corners.push_back(world* glm::vec4(boxmax.x, boxmin.y, boxmin.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmin.x, boxmax.y, boxmin.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmin.x, boxmin.y, boxmax.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmax.x, boxmax.y, boxmax.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmin.x, boxmax.y, boxmax.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmax.x, boxmin.y, boxmax.z, 1.0)
	);

	corners.push_back(world*  glm::vec4(boxmax.x, boxmax.y, boxmin.z, 1.0)
	);
	for (auto it = children.begin(); it != children.end(); it++) {
		(*it)->Update(local, world);
	}
}

void Joint::Draw() {
	glBegin(GL_LINES);
	glVertex3f(corners.at(0).x, corners.at(0).y, corners.at(0).z);
	glVertex3f(corners.at(1).x, corners.at(1).y, corners.at(1).z);
	glVertex3f(corners.at(0).x, corners.at(0).y, corners.at(0).z);
	glVertex3f(corners.at(2).x, corners.at(2).y, corners.at(2).z);
	glVertex3f(corners.at(0).x, corners.at(0).y, corners.at(0).z);
	glVertex3f(corners.at(3).x, corners.at(3).y, corners.at(3).z);

	glVertex3f(corners.at(4).x, corners.at(4).y, corners.at(4).z);
	glVertex3f(corners.at(5).x, corners.at(5).y, corners.at(5).z);
	glVertex3f(corners.at(4).x, corners.at(4).y, corners.at(4).z);
	glVertex3f(corners.at(6).x, corners.at(6).y, corners.at(6).z);
	glVertex3f(corners.at(4).x, corners.at(4).y, corners.at(4).z);
	glVertex3f(corners.at(7).x, corners.at(7).y, corners.at(7).z);

	glVertex3f(corners.at(1).x, corners.at(1).y, corners.at(1).z);
	glVertex3f(corners.at(6).x, corners.at(6).y, corners.at(6).z);
	glVertex3f(corners.at(1).x, corners.at(1).y, corners.at(1).z);
	glVertex3f(corners.at(7).x, corners.at(7).y, corners.at(7).z);

	glVertex3f(corners.at(2).x, corners.at(2).y, corners.at(2).z);
	glVertex3f(corners.at(5).x, corners.at(5).y, corners.at(5).z);
	glVertex3f(corners.at(2).x, corners.at(2).y, corners.at(2).z);
	glVertex3f(corners.at(7).x, corners.at(7).y, corners.at(7).z);

	glVertex3f(corners.at(3).x, corners.at(3).y, corners.at(3).z);
	glVertex3f(corners.at(5).x, corners.at(5).y, corners.at(5).z);
	glVertex3f(corners.at(3).x, corners.at(3).y, corners.at(3).z);
	glVertex3f(corners.at(6).x, corners.at(6).y, corners.at(6).z);

	glEnd();

	for (auto it = children.begin(); it != children.end(); it++) {
		(*it)->Draw();
	}
}

void Joint::Reset() {}

void Joint::Value() {
	std::cout << "Joint:" << name << std::endl
		<< "offset: <" << offset.x << ", " << offset.y << ", " << offset.z << ">" << std::endl
		<< "boxmin: <" << boxmin.x << ", " << boxmin.y << ", " << boxmin.z << ">" << std::endl
		<< "boxmax: <" << boxmax.x << ", " << boxmax.y << ", " << boxmax.z << ">" << std::endl
		<< "DOF x value/min/max: " << rotx.getValue() << "/" << rotx.Min << "/" << rotx.Max << std::endl
		<< "DOF y value/min/max: " << roty.getValue() << "/" << roty.Min << "/" << roty.Max << std::endl
		<< "DOF z value/min/max: " << rotz.getValue() << "/" << rotz.Min << "/" << rotz.Max << std::endl << std::endl;

		      
}