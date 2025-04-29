#include "Scene.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Scene::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Nie mo¿na otworzyæ pliku: " << filename << std::endl;
		return;
	}

	std::string line;
	std::vector<Edge> currentEdges;

	while (std::getline(file, line)) {

		if (line.empty() || line[0] == '#') {
			if (!currentEdges.empty()) {
				shapes.emplace_back(currentEdges);
				currentEdges.clear();
			}
			continue;
		}

		std::istringstream iss(line);
		float x1, y1, z1, x2, y2, z2;
		if (!(iss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2)) {
			std::cerr << "B³¹d parsowania linii: " << line << std::endl;
			continue;
		}

		Eigen::Vector4f start(x1, y1, z1, 1.0f);
		Eigen::Vector4f end(x2, y2, z2, 1.0f);
		currentEdges.emplace_back(start, end);
	}
	if (!currentEdges.empty()) {
		shapes.emplace_back(currentEdges);
	}

	file.close();

	for (Shape shape : shapes)
	{
		std::cout << "Shape with " << shape.getEdges().size() << " edges loaded." << std::endl;
	}
}

