#include "MyRenderer.h"
#include <SDL2/SDL_render.h>
#include <iostream>
#include<stdlib.h>

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

void MyRenderer::RecalculateViewMatrix()
{

	/*cameraForward.normalize();
	cameraRight = cameraForward.cross(cameraUp).normalized();
	cameraUp = cameraRight.cross(cameraForward).normalized();*/

	// Macierz rotacji kamery
	Eigen::Matrix3f rotation;
	rotation.col(0) = cameraRight;
	rotation.col(1) = cameraUp;
	rotation.col(2) = -cameraForward; // Uwaga: forward musi byæ "w g³¹b ekranu"

	// Buduj pe³n¹ macierz widoku
	viewMatrix.setIdentity();
	viewMatrix.block<3, 3>(0, 0) = rotation.transpose(); // transpozycja = odwrotnoœæ obrotu
	viewMatrix.block<3, 1>(0, 3) = -rotation.transpose() * cameraPosition;

	// Debug
	//std::cout << "ViewMatrix:\n" << viewMatrix << "\n";
	
	//viewMatrix <<
	//	cameraRight.x(), cameraRight.y(), cameraRight.z(), -cameraPosition.x(),
	//	cameraUp.x(), cameraUp.y(), cameraUp.z(), -cameraPosition.y(),
	//	-cameraForward.x(), -cameraForward.y(), -cameraForward.z(), -cameraPosition.z(),
	//	0, 0, 0, 1;


}

Eigen::Vector2f MyRenderer::projectPoint(const Eigen::Vector4f& point)
{

	Eigen::Vector4f projected = projectionMatrix * viewMatrix *  point;
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

void MyRenderer::moveCameraRight(int align)
{
	if (align > 0) {
		cameraPosition += cameraStep * cameraRight;
	}
	else {
		cameraPosition -= cameraStep * cameraRight;
	}
	//RecalculateViewMatrix();
}

void MyRenderer::moveCameraForward(int align)
{
	if (align > 0) {
		cameraPosition += cameraStep * cameraForward;
	}
	else {
		cameraPosition -= cameraStep * cameraForward;
	}

	//RecalculateViewMatrix();
}

void MyRenderer::moveCameraUp(int align)
{
	if (align >0) {
		cameraPosition += cameraStep * cameraUp;
	}
	else {
		cameraPosition -= cameraStep * cameraUp;
	}

	//RecalculateViewMatrix();
}

void MyRenderer::RotateCamera(int mouseX, int mouseY, int mouseZ)
{
	float angleX = mouseX * M_PI / (180.0);
	float angleY = mouseY * M_PI / (180.0);
	float angleZ = mouseZ * M_PI / (180.0);

	Eigen::Vector3f axisX = cameraRight;
	Eigen::Vector3f axisY = cameraUp;
	Eigen::Vector3f axisZ = cameraForward;

	Eigen::Quaternionf qx(Eigen::AngleAxisf(angleX, axisX));
	Eigen::Quaternionf qy(Eigen::AngleAxisf(angleY, axisY));
	Eigen::Quaternionf qz(Eigen::AngleAxisf(angleZ, axisZ));

	Eigen::Quaternionf q = qy * qx * qz;

	cameraForward = q * cameraForward;
	cameraRight = q * cameraRight;
	cameraUp = q * cameraUp;
	//RecalculateViewMatrix();
}
