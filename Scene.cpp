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
	std::vector<Tris> currentTris;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			if (!currentEdges.empty() || !currentTris.empty()) {
				shapes.emplace_back(currentEdges, currentTris);
				currentEdges.clear();
				currentTris.clear();
			}
			continue;
		}

		std::istringstream iss(line);
		std::vector<float> numbers;
		float num;
		while (iss >> num) {
			numbers.push_back(num);
		}

		if (numbers.size() == 6) {
			Eigen::Vector4f start(numbers[0], numbers[1], numbers[2], 1.0f);
			Eigen::Vector4f end(numbers[3], numbers[4], numbers[5], 1.0f);
			currentEdges.emplace_back(start, end);
		}
		else if (numbers.size() == 12) {
			Eigen::Vector4f v1(numbers[0], numbers[1], numbers[2], 1.0f);
			Eigen::Vector4f v2(numbers[3], numbers[4], numbers[5], 1.0f);
			Eigen::Vector4f v3(numbers[6], numbers[7], numbers[8], 1.0f);
			int R = static_cast<int>(numbers[9]);
			int G = static_cast<int>(numbers[10]);
			int B = static_cast<int>(numbers[11]);
			currentTris.emplace_back(v1, v2, v3, R, G, B);
		}
		else {
			std::cerr << "Nieprawid³owa liczba wartoœci w linii: " << line << std::endl;
		}
	}

	if (!currentEdges.empty() || !currentTris.empty()) {
		shapes.emplace_back(currentEdges, currentTris);
	}

	file.close();

	for (const Shape& shape : shapes) {
		std::cout << "Shape with " << shape.getEdges().size() << " edges loaded." << std::endl;
	}
}


