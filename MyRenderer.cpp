#include "MyRenderer.h"
#include <SDL2/SDL_render.h>
#include <iostream>
#include<stdlib.h>

Eigen::Matrix4f MyRenderer::getProjectionMatrix()
{
	return projectionMatrix;
}

Eigen::Matrix4f MyRenderer::getViewMatrix()
{
	return viewMatrix;
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
	if (value > 0 && d - d_step > 0) {
		d -= d_step;
	}
	else if (value < 0 ) {
		d += d_step;
	}
	RecalculateProjectionMatrix();

}

void MyRenderer::RecalculateProjectionMatrix()
{

	float f = 1.0f / tan(d / 2.0f);

	projectionMatrix <<
		f / widthToHeightRatio, 0, 0, 0,
		0, f, 0, 0,  
		0, 0, (far + near) / (far - near),  2 * far * near / (far - near),
		0, 0, 1, 0;
}

void MyRenderer::RecalculateViewMatrix()
{

	Eigen::Matrix3f rotation;
	rotation.col(0) = cameraRight;
	rotation.col(1) = cameraUp;
	rotation.col(2) = cameraForward; 

	viewMatrix.setIdentity();
	viewMatrix.block<3, 3>(0, 0) = rotation.transpose();
	viewMatrix.block<3, 1>(0, 3) = -rotation.transpose() * cameraPosition;

}

Eigen::Vector4f MyRenderer::projectPoint(const Eigen::Vector4f& point)
{
	return projectionMatrix * viewMatrix * point;
}

void MyRenderer::drawShape(const Shape& shape) {

	for (const Edge& edge : shape.getEdges()) {
		Eigen::Vector4f start_clip = projectPoint(edge.getStart());
		Eigen::Vector4f end_clip = projectPoint(edge.getEnd());

		bool start_visible = start_clip.w() > 0;
		bool end_visible = end_clip.w() > 0;

		if (start_visible && end_visible) {
			float startX_ndc = start_clip.x() / start_clip.w();
			float startY_ndc = start_clip.y() / start_clip.w();

			float endX_ndc = end_clip.x() / end_clip.w();
			float endY_ndc = end_clip.y() / end_clip.w();


			int startScreenX = static_cast<int>((startX_ndc + 1.0f) * WIDTH / 2.0f);
			int startScreenY = static_cast<int>((1.0f - startY_ndc) * HEIGHT / 2.0f);

			int endScreenX = static_cast<int>((endX_ndc + 1.0f) * WIDTH / 2.0f);
			int endScreenY = static_cast<int>((1.0f - endY_ndc) * HEIGHT / 2.0f);

			SDL_RenderDrawLine(renderer, startScreenX, startScreenY, endScreenX, endScreenY);

		}
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
}

void MyRenderer::moveCameraForward(int align)
{
	if (align > 0) {
		cameraPosition += cameraStep * cameraForward;
	}
	else {
		cameraPosition -= cameraStep * cameraForward;
	}
}

void MyRenderer::moveCameraUp(int align)
{
	if (align >0) {
		cameraPosition += cameraStep * cameraUp;
	}
	else {
		cameraPosition -= cameraStep * cameraUp;
	}
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
}
