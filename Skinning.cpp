#include <GL/glew.h>

#include "Skinning.h"



Skinning::Skinning(char* file, Joint* skel)
{
	load(file, skel);
	
}
Skinning::Skinning(std::string file, Joint* skel)
{
	load((char*)(file.c_str()), skel);
}

Skinning::~Skinning()
{
}

void Skinning::load(char* file, Joint* skel)
{
	Tokenizer* obj = new Tokenizer();
	std::cerr << "Load call\n";
	joint_load(skel);
	if (obj->Open(file))
	{
		
		rLoad(obj, this);

	}
	displayValues();
	Update();
	
}

void Skinning::joint_load(Joint* root) {
	skel = root->all_joint;

}

void Skinning::rLoad(Tokenizer* obj, Skinning* current) {

	char token[256] = "";
	obj->GetToken(token);
	std::cerr << "rLoad call\n";
	while (true) {
		//std::cout << token << std::endl;
		if (strcmp(token, "}") == 0) {
		}
		else if (strcmp(token, "positions") == 0) {
			
			int c = obj->GetInt();
			obj->GetToken(token);
			memset(token, 0, 256);
			for(int i = 0; i<c; i++) {
				float x = obj->GetFloat();
				float y = obj->GetFloat();
				float z = obj->GetFloat();
				positions.push_back(glm::vec3(x,y,z));
				//std::cout << "<"<<x<<","<<y<<","<<z<<">" << std::endl;
			}
		}
		else if (strcmp(token, "normals") == 0) {
			
			int c = obj->GetInt();
			obj->GetToken(token);
			memset(token, 0, 256);
			for (int i = 0; i<c; i++) {
				float x = obj->GetFloat();
				float y = obj->GetFloat();
				float z = obj->GetFloat();
				normals.push_back(glm::vec3(x,y,z));
				
			}
		}
		else if (strcmp(token, "skinweights") == 0) {
			
			int c = obj->GetInt();
			std::cout << c << std::endl;
			obj->GetToken(token);
			memset(token, 0, 256);
			std::cout << "start  outer loop" << std::endl;
			for (int i = 0; i<c; i++) {
				int c2 = obj->GetInt();
				
				std::vector<std::pair<int, float>> weights;
				for (int i2 = 0; i2 < c2; i2++) {
					//obj->GetToken(token);
					
					//std::cout << "line: " << obj->GetInt() << " ";
					//memset(token, 0, 256);
					//obj->GetToken(token);
					
					//std::cout << obj->GetFloat() << std::endl;
					//memset(token, 0, 256);
					int first = obj->GetInt();
					float second = obj->GetFloat();
					weights.push_back(std::pair<int, float>(first, second));
				}
				skinWeights.push_back(weights);
				memset(token, 0, 256);
			}
		}
		else if (strcmp(token, "triangles") == 0) {
			
			int c = obj->GetInt();
			obj->GetToken(token);
			memset(token, 0, 256);
			for (int i = 0; i<c; i++) {
				float x = obj->GetFloat();
				float y = obj->GetFloat();
				float z = obj->GetFloat();
				triangles.push_back(glm::vec3(x,y,z));
			}
		}
		else if (strcmp(token, "bindings") == 0) {
			
			int c = obj->GetInt();
			obj->GetToken(token);
			memset(token, 0, 256);
			for (int i = 0; i<c; i++) {
				obj->GetToken(token);
				obj->GetToken(token);
				glm::mat4 m;
				m[0][0] = obj->GetFloat();
				m[0][1] = obj->GetFloat();
				m[0][2] = obj->GetFloat();
				m[0][3] = 0.0f;
				m[1][0] = obj->GetFloat();
				m[1][1] = obj->GetFloat();
				m[1][2] = obj->GetFloat();
				m[1][3] = 0.0f;
				m[2][0] = obj->GetFloat();
				m[2][1] = obj->GetFloat();
				m[2][2] = obj->GetFloat();
				m[2][3] = 0.0f;
				m[3][0] = obj->GetFloat();
				m[3][1] = obj->GetFloat();
				m[3][2] = obj->GetFloat();
				m[3][3] = 1.0f;
				
				
				bindings.push_back(m);
				obj->GetToken(token);
			}
			break;
		}

		memset(token, 0, 256);
		obj->GetToken(token);
	}
	
}
void Skinning::rotate_x(int joint_id, float deg) {
	skel.at(joint_id%skel.size())->rotate_x(deg);
}
void Skinning::rotate_y(int joint_id, float deg) {
	skel.at(joint_id%skel.size())->rotate_y(deg);
}
void Skinning::rotate_z(int joint_id, float deg) {
	skel.at(joint_id%skel.size())->rotate_z(deg);
}
void Skinning::Update() {

	std::vector<glm::vec3> temp_positions, temp_normals;

	//loops through all positions
	for (int i = 0; i < positions.size(); i++) {
		glm::vec4 final_pos = glm::vec4(0.0, 0.0, 0.0, 0.0);
		glm::vec4 final_norm = glm::vec4(0.0, 0.0, 0.0, 0.0);
		//std::cout << "\t" << skinWeights.at(i).size() << " ";
		for (auto it = skinWeights.at(i).begin(); it != skinWeights.at(i).end(); it++) {
			//applies smooth skin algorithm
			/*std::cout << it->first << " " << std::setprecision(3) << it->second << " ";
			std::cerr << "Joint: " << skel.at(it->first)->name << std::endl;
			std::cerr << "\t" << std::setprecision(3) << (skel.at(it->first)->World)[0][0] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(it->first)->World)[1][0] << " " << std::setprecision(3) << (skel.at(it->first)->World)[2][0] << " " << std::setprecision(3) << (skel.at(it->first)->World)[3][0] << std::endl
				<< "\t" << std::setprecision(3) << (skel.at(it->first)->World)[0][1] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(it->first)->World)[1][1] << " " << std::setprecision(3) << (skel.at(it->first)->World)[2][1] << " " << std::setprecision(3) << (skel.at(it->first)->World)[3][1] << std::endl
				<< "\t" << std::setprecision(3) << (skel.at(it->first)->World)[0][2] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(it->first)->World)[1][2] << " " << std::setprecision(3) << (skel.at(it->first)->World)[2][2] << " " << std::setprecision(3) << (skel.at(it->first)->World)[3][2] << std::endl
				<< "\t" << std::setprecision(3) << (skel.at(it->first)->World)[0][3] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(it->first)->World)[1][3] << " " << std::setprecision(3) << (skel.at(it->first)->World)[2][3] << " " << std::setprecision(3) << (skel.at(it->first)->World)[3][3] << std::endl
				<< std::endl;*/
			
			
			
			
			
			final_pos += (float)it->second*(skel.at(it->first)->World*(glm::inverse(bindings.at(it->first)) *   glm::vec4(positions.at(i), 1.0)));
			//final_norm += (float)it->second*( * ( glm::vec4(normals.at(i), 1.0));

		}
		//std::cout << std::endl;
		//stores changed vectors
		temp_positions.push_back(final_pos);
		temp_normals.push_back(final_norm);
	}

	current_positions = temp_positions;
	current_normals = temp_normals;
	//std::cerr << "#of joints: " << skel.size() << std::endl;
}
void Skinning::Draw(GLuint shaderProgram) {
	


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);

	//std::cerr << "triangle_size: " << triangles.size() << std::endl;
	//std::cerr << "position_size: " << positions.size() << std::endl;
	//std::cerr << "normals_size: " << normals.size() << std::endl; 
	//std::cerr << "skin_weight" << skinWeights.size() << std::endl;
	//for (int i = 0; i < skel.size(); i++) {
	//	std::cerr << " Joint:" << i << " " << skel.at(i)->name << std::endl;
	//}

	for (int i = 0; i < triangles.size(); i++) {
		
		glm::vec3 pos0 = glm::vec3((current_positions.at(triangles.at(i)[0]))[0], (current_positions.at(triangles.at(i)[0]))[1], (current_positions.at(triangles.at(i)[0]))[2]);
		glm::vec3 pos1 = glm::vec3((current_positions.at(triangles.at(i)[1]))[0], (current_positions.at(triangles.at(i)[1]))[1], (current_positions.at(triangles.at(i)[1]))[2]);
		glm::vec3 pos2 = glm::vec3((current_positions.at(triangles.at(i)[2]))[0], (current_positions.at(triangles.at(i)[2]))[1], (current_positions.at(triangles.at(i)[2]))[2]);

		glColor3f(i/255.0f, 55, i / 255.0f);
		glVertex3f(pos0[0], pos0[1], pos0[2]);
		glVertex3f(pos1[0], pos1[1], pos1[2]);
		glVertex3f(pos2[0], pos2[1], pos2[2]);
		
	}
	glEnd();
}
void Skinning::displayValues() {
	std::cout << "positions " << positions.size() << " {\n";
	for (auto it = positions.begin(); it != positions.end(); it++) {
		std::cout << "\t" << std::setprecision(3) << it->x << " " << std::setprecision(3) << it->y << " " << std::setprecision(3) << it->z << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::cout << "normals " << normals.size() << " {\n";
	for (auto it = normals.begin(); it != normals.end(); it++) {
		std::cout << "\t" << std::setprecision(3) << it->x << " " << std::setprecision(3) << it->y << " " << std::setprecision(3) << it->z << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::cout << "skinweights " << skinWeights.size() << " {\n";
	for (auto it = skinWeights.begin(); it != skinWeights.end(); it++) {
		std::cout << "\t" << it->size() << " ";
		for (auto it2 = it->begin(); it2 != it->end(); it2++) {
			std::cout << it2->first << " " << std::setprecision(3) << it2->second << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::cout << "triangles " << triangles.size() << std::endl;
	for (auto it = triangles.begin(); it != triangles.end(); it++) {
		std::cout << "\t" << it->x << " " << it->y << " " << it->z << std::endl;
	}
	std::cout << std::endl << std::endl;

	std::cout << "bindings " << bindings.size() << std::endl;
	for (auto it = bindings.begin(); it != bindings.end(); it++) {
		std::cout << "\t" << std::setprecision(3) << (*it)[0][0] << std::setprecision(3) << " " << std::setprecision(3) << (*it)[1][0] << " " << std::setprecision(3) << (*it)[2][0] << " " << std::setprecision(3) << (*it)[3][0] << std::endl
		          << "\t" << std::setprecision(3) << (*it)[0][1] << std::setprecision(3) << " " << std::setprecision(3) << (*it)[1][1] << " " << std::setprecision(3) << (*it)[2][1] << " " << std::setprecision(3) << (*it)[3][1] << std::endl
		          << "\t" << std::setprecision(3) << (*it)[0][2] << std::setprecision(3) << " " << std::setprecision(3) << (*it)[1][2] << " " << std::setprecision(3) << (*it)[2][2] << " " << std::setprecision(3) << (*it)[3][2] << std::endl
		          << "\t" << std::setprecision(3) << (*it)[0][3] << std::setprecision(3) << " " << std::setprecision(3) << (*it)[1][3] << " " << std::setprecision(3) << (*it)[2][3] << " " << std::setprecision(3) << (*it)[3][3] << std::endl
		          << std::endl;
	}


	std::cout << "Joints:" << std::endl;
	for (int i = 0; i < skel.size(); i++) {
		std::cout << "Joint:" << i << " name:" << (skel.at(i))->name <<std::endl;
		
		std::cout << "\t" << std::setprecision(3) << (skel.at(i)->World)[0][0] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(i)->World)[1][0] << " " << std::setprecision(3) << (skel.at(i)->World)[2][0] << " " << std::setprecision(3) << (skel.at(i)->World)[3][0] << std::endl
			<< "\t" << std::setprecision(3) << (skel.at(i)->World)[0][1] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(i)->World)[1][1] << " " << std::setprecision(3) << (skel.at(i)->World)[2][1] << " " << std::setprecision(3) << (skel.at(i)->World)[3][1] << std::endl
			<< "\t" << std::setprecision(3) << (skel.at(i)->World)[0][2] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(i)->World)[1][2] << " " << std::setprecision(3) << (skel.at(i)->World)[2][2] << " " << std::setprecision(3) << (skel.at(i)->World)[3][2] << std::endl
			<< "\t" << std::setprecision(3) << (skel.at(i)->World)[0][3] << std::setprecision(3) << " " << std::setprecision(3) << (skel.at(i)->World)[1][3] << " " << std::setprecision(3) << (skel.at(i)->World)[2][3] << " " << std::setprecision(3) << (skel.at(i)->World)[3][3] << std::endl
			<< std::endl;
	}
}