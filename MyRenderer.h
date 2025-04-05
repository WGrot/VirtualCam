#pragma once
#include "Shape.h"
#include<Eigen/Core>
#include <SDL2/SDL.h>
class MyRenderer
{
private:
	SDL_Renderer* renderer;
	Eigen::Matrix4f projectionMatrix;
    float d;
	float d_step = 0.2f;
	float widthToHeightRatio;
	int WIDTH;
	int HEIGHT;

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
	};

	Eigen::Matrix4f getProjectionMatrix();

	void setProjectionMatrix(Eigen::Matrix4f projectionMatrix);

	void setD(float d);
	void stepD(int value);

	void RecalculateProjectionMatrix();

	Eigen::Vector2f projectPoint(const Eigen::Vector4f& point);

	void drawShape(const Shape& shape);

};


