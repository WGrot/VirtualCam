#pragma once
#include "Shape.h"
#include<Eigen/Core>
#include <Eigen/Geometry>
#include <SDL2/SDL.h>
class MyRenderer
{
private:
	SDL_Renderer* renderer;
	Eigen::Matrix4f projectionMatrix;
    float d;
	float d_step = 0.05f;
	float near = 0.1f;
	float far = 100.0f;
	float widthToHeightRatio;
	int WIDTH;
	int HEIGHT;

	Eigen::Vector3f cameraPosition = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	Eigen::Vector3f cameraForward = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
	Eigen::Vector3f cameraUp = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	Eigen::Vector3f cameraRight = Eigen::Vector3f(1.0f, 0.0f, 0.0f);
	float cameraStep = 0.2f;
	Eigen::Matrix4f viewMatrix;

public:
	MyRenderer(SDL_Renderer* renderer, float d, int width, int height){

		this->d = d;
		this->renderer = renderer;
		this->WIDTH = width;
		this->HEIGHT = height;
		widthToHeightRatio = static_cast<float>(width) / static_cast<float>(height);

		projectionMatrix <<
			1 / widthToHeightRatio, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 1 / d, 0;

		viewMatrix = Eigen::Matrix4f::Identity();
		viewMatrix(0, 3) = cameraPosition.x();
		viewMatrix(1, 3) = cameraPosition.y();
		viewMatrix(2, 3) = cameraPosition.z();

		RecalculateProjectionMatrix();
	};

	Eigen::Matrix4f getProjectionMatrix();
	Eigen::Matrix4f getViewMatrix();

	void setProjectionMatrix(Eigen::Matrix4f projectionMatrix);

	void setD(float d);

	void stepD(int value);

	void RecalculateProjectionMatrix();
	void RecalculateViewMatrix();

	Eigen::Vector4f projectPoint(const Eigen::Vector4f& point);

	void drawShape(const Shape& shape);

	void DrawTris(const Tris& tris);

	void moveCameraRight(int align);
	void moveCameraForward(int align);
	void moveCameraUp(int align);
	void RotateCamera(int mouseX, int mouseY, int mouseZ);
};


