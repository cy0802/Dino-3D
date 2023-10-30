#include "ObjReader.h"

std::vector<float> ObjReader::read(char* path) {
	std::ifstream objFile;
	objFile.open(path);
	if (objFile.fail()) {
		std::cout << "error while opening the obj file\n";
		exit(1);
	} else {
		std::cout << "open file successfully\n";
	}
	char dataType;
	float left = 300.0, right = -300.0, bottom = 300.0, top = -300.0, near = -300.0, far = 300.0;
	glm::vec3 tmp;
	std::vector<glm::vec3> vertex;
	vertex.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	// std::vector<glm::vec3> face;
	std::vector<float> allData; // vertex + normal
	while (objFile >> dataType) {
		objFile >> tmp.x >> tmp.y >> tmp.z;
		// std::cout << dataType << ": " << tmp.x << " " << tmp.y << " " << tmp.z << "\n";
		if (dataType == 'v') {
			vertex.push_back(tmp);
			if (tmp.x < left) left = tmp.x;
			if (tmp.x > right) right = tmp.x;
			if (tmp.y > top) top = tmp.y;
			if (tmp.y < bottom) bottom = tmp.y;
			if (tmp.z > near) near = tmp.z;
			if (tmp.z < far) far = tmp.z;
		} else if (dataType == 'f') {
			glm::vec3 _vertex[3];
			_vertex[0] = vertex[tmp.x]; _vertex[1] = vertex[tmp.y]; _vertex[2] = vertex[tmp.z];
			glm::vec3 normal = glm::cross(_vertex[1] - _vertex[0], _vertex[2] - _vertex[0]);
			for (int i = 0; i < 3; i++) {
				allData.push_back(_vertex[i].x);
				allData.push_back(_vertex[i].y);
				allData.push_back(_vertex[i].z);
				allData.push_back(normal[0]);
				allData.push_back(normal[1]);
				allData.push_back(normal[2]);
			}
		} else {
			std::cout << "invalid format: " << dataType << " " << tmp.x << " " << tmp.y << " " << tmp.z << "\n";
		}
	}
	allData.push_back(right - left);
	allData.push_back(top - bottom);
	allData.push_back(near - far);
	std::cout << "number of vertex: " << vertex.size() - 1 << "\n";
	std::cout << "number of face: " << allData.size() / 18 << "\n";
	std::cout << "max width: " << right - left << "\n";
	std::cout << "max height: " << top - bottom << "\n";
	std::cout << "max depth: " << near - far << "\n";
	objFile.close();
	return allData;
}