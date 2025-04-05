#include "MyRenderer.h"

Eigen::Matrix4f MyRenderer::getProjectionMatrix()
{
	return Eigen::Matrix4f();
}

void MyRenderer::setProjectionMatrix(Eigen::Matrix4f projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
}
void MyRenderer::setD(int d)
{
	this->d = d;
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
