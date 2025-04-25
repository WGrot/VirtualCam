#pragma once
#include "Shape.h"
class Scene
{
private:
	std::vector<Shape> shapes;
public:
	void loadFromFile(const std::string& filename);
	std::vector<Shape> getShapes() const { return shapes; }
};

