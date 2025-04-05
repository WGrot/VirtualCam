#pragma once
#include "Vector3.h"
#include <Eigen/Core>
class Camera
{
private:
	Eigen::Vector3f position;
	Eigen::Vector3f rotation;

public:
	Camera() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f) {}
	Eigen::Vector3f getPosition() const { return position; }
	Eigen::Vector3f getRotation() const { return rotation; }
	void setPosition(const Eigen::Vector3f& newPosition) { position = newPosition; }
	void setRotation(const Eigen::Vector3f& newRotation) { rotation = newRotation; }
};

