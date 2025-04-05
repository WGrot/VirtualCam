#include "MyRenderer.h"
#include <SDL2/SDL_render.h>

Eigen::Matrix4f MyRenderer::getProjectionMatrix()
{
	return Eigen::Matrix4f();
}

void MyRenderer::setProjectionMatrix(Eigen::Matrix4f projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
}
void MyRenderer::setD(float d)
{
	this->d = d;
	RecalculateProjectionMatrix();
}

void MyRenderer::stepD(int value)
{
	if (value > 0) {
		d += d_step;
	}
	else if (value < 0) {
		d -= d_step;
	}
	RecalculateProjectionMatrix();

}

void MyRenderer::RecalculateProjectionMatrix()
{
	projectionMatrix << 
		1 / widthToHeightRatio, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 1 / d, 0;
}

Eigen::Vector2f MyRenderer::projectPoint(const Eigen::Vector4f& point)
{
	Eigen::Vector4f projected = projectionMatrix * point;
	return Eigen::Vector2f(projected.x() / projected.w(), projected.y() / projected.w());
}

void MyRenderer::drawShape(const Shape& shape) {
	for (const Edge& edge : shape.getEdges()) {
		Eigen::Vector2f start = projectPoint(edge.getStart());
		Eigen::Vector2f end = projectPoint(edge.getEnd());

		SDL_RenderDrawLine(renderer,
			static_cast<int>((start.x() + 1) * WIDTH / 2),
			static_cast<int>((1 - start.y()) * HEIGHT / 2),
			static_cast<int>((end.x() + 1) * WIDTH / 2),
			static_cast<int>((1 - end.y()) * HEIGHT / 2));
	}
}
