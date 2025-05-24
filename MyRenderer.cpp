#include "MyRenderer.h"
#include <SDL2/SDL_render.h>
#include <iostream>
#include<stdlib.h>
#include "Scene.h"
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <omp.h>

#pragma region MatrixCalculations

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
	else if (value < 0) {
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
		0, 0, (far + near) / (far - near), 2 * far * near / (far - near),
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
#pragma endregion

void MyRenderer::drawFaces(Scene& scene)
{
	std::vector<Tris> validTris;
	for (Shape& shape : scene.getShapes()) {

		for (Tris& tris : shape.getTris_list()) {
			Eigen::Vector3f cameraToTrisVector = cameraPosition - tris.getV1().head<3>();
			float wynik = tris.getNormal().dot(cameraToTrisVector);

			Eigen::Vector4f v1 = projectPoint(tris.getV1());
			Eigen::Vector4f v2 = projectPoint(tris.getV2());
			Eigen::Vector4f v3 = projectPoint(tris.getV3());

			bool v1_visible = v1.w() > 0;
			bool v2_visible = v2.w() > 0;
			bool v3_visible = v3.w() > 0;

			if (v1_visible && v2_visible && v3_visible && wynik > 0) {
				validTris.push_back(tris);
			}
		}
	}

	for (Tris& tris : validTris) {
		Eigen::Vector4f v1 = projectPoint(tris.getV1());
		Eigen::Vector4f v2 = projectPoint(tris.getV2());
		Eigen::Vector4f v3 = projectPoint(tris.getV3());

		float v1X_ndc = v1.x() / v1.w();
		float v1Y_ndc = v1.y() / v1.w();

		float v2X_ndc = v2.x() / v2.w();
		float v2Y_ndc = v2.y() / v2.w();

		float v3X_ndc = v3.x() / v3.w();
		float v3Y_ndc = v3.y() / v3.w();


		int v1ScreenX = static_cast<int>((v1X_ndc + 1.0f) * WIDTH / 2.0f);
		int v1ScreenY = static_cast<int>((1.0f - v1Y_ndc) * HEIGHT / 2.0f);

		int v2ScreenX = static_cast<int>((v2X_ndc + 1.0f) * WIDTH / 2.0f);
		int v2ScreenY = static_cast<int>((1.0f - v2Y_ndc) * HEIGHT / 2.0f);

		int v3ScreenX = static_cast<int>((v3X_ndc + 1.0f) * WIDTH / 2.0f);
		int v3ScreenY = static_cast<int>((1.0f - v3Y_ndc) * HEIGHT / 2.0f);

		Eigen::Vector2f v1ScreenPos(v1ScreenX, v1ScreenY);
		Eigen::Vector2f v2ScreenPos(v2ScreenX, v2ScreenY);
		Eigen::Vector2f v3ScreenPos(v3ScreenX, v3ScreenY);
		tris.SetV1Projection(v1ScreenPos);
		tris.SetV2Projection(v2ScreenPos);
		tris.SetV3Projection(v3ScreenPos);

	}


	for (int i = 0; i < validTris.size(); i++) {
		Tris tris = validTris[i];

		int v1ScreenX = tris.getV1Projection().x();
		int v1ScreenY = tris.getV1Projection().y();

		int v2ScreenX = tris.getV2Projection().x();
		int v2ScreenY = tris.getV2Projection().y();

		int v3ScreenX = tris.getV3Projection().x();
		int v3ScreenY = tris.getV3Projection().y();

		SDL_Color trisColor = { tris.getR(), tris.getG(), tris.getB(), 255 };
		SDL_Vertex verts[3] = {
			{ {v1ScreenX, v1ScreenY}, trisColor, {0, 0} },
			{ {v2ScreenX, v2ScreenY}, trisColor, {0, 0} },
			{ {v3ScreenX, v3ScreenY}, trisColor, {0, 0} }
		};

		rasterizeTriangle(surface, tris, lightPos, cameraPosition.head<3>() - cameraForward * 30, lightColor);
	}
}




#pragma region CameraMovement

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
	if (align > 0) {
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
#pragma endregion


void MyRenderer::barycentric(float x, float y, const Eigen::Vector2f& v0, const Eigen::Vector2f& v1, const Eigen::Vector2f& v2, float& u, float& v, float& w)
{
	float denom = (v1.y() - v2.y()) * (v0.x() - v2.x()) + (v2.x() - v1.x()) * (v0.y() - v2.y());
	u = ((v1.y() - v2.y()) * (x - v2.x()) + (v2.x() - v1.x()) * (y - v2.y())) / denom;
	v = ((v2.y() - v0.y()) * (x - v2.x()) + (v0.x() - v2.x()) * (y - v2.y())) / denom;
	w = 1.0f - u - v;
}

void MyRenderer::putPixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;
	Uint32* pixels = (Uint32*)surface->pixels;
	pixels[y * surface->w + x] = color;
}

Eigen::Vector3f MyRenderer::phongLighting(const Eigen::Vector3f& fragPos, const Eigen::Vector3f& normal, const Eigen::Vector3f& lightPos, const Eigen::Vector3f& viewPos, const Eigen::Vector3f& lightColor, const Eigen::Vector3f& baseColor)
{
	Eigen::Vector3f N = normal;
	Eigen::Vector3f L = (lightPos - fragPos).normalized();
	Eigen::Vector3f V = (viewPos - fragPos).normalized();
	Eigen::Vector3f R = (2.0f * N.dot(L) * N - L).normalized();

	float ambientStrength = 0.1f;
	Eigen::Vector3f ambient = ambientStrength * lightColor;

	float diff = std::max(N.dot(L), 0.0f);
	Eigen::Vector3f diffuse = diff * lightColor;

	float specularStrength = 20.0f;
	float shininess = 32.0f;
	float spec = powf(std::max(R.dot(V), 0.0f), shininess);
	Eigen::Vector3f specular = specularStrength * spec * lightColor;

	Eigen::Vector3f result = ambient.cwiseProduct(baseColor) +
		diffuse.cwiseProduct(baseColor)
		+ specular(baseColor);


	return result.cwiseMin(1.0f).cwiseMax(0.0f);
}


void MyRenderer::rasterizeTriangle(SDL_Surface* surface, const Tris& tri,
	const Eigen::Vector3f& lightPos,
	const Eigen::Vector3f& viewPos,
	const Eigen::Vector3f& lightColor)
{

	auto v0 = tri.getV1Projection();
	auto v1 = tri.getV2Projection();
	auto v2 = tri.getV3Projection();


	int minX = std::max(0, (int)std::floor(std::min({ v0.x(), v1.x(), v2.x() })));
	int maxX = std::min(surface->w - 1, (int)std::ceil(std::max({ v0.x(), v1.x(), v2.x() })));
	int minY = std::max(0, (int)std::floor(std::min({ v0.y(), v1.y(), v2.y() })));
	int maxY = std::min(surface->h - 1, (int)std::ceil(std::max({ v0.y(), v1.y(), v2.y() })));

	Eigen::Vector3f baseColor(tri.getR() / 255.f, tri.getG() / 255.f, tri.getB() / 255.f);


	auto P0 = tri.getV1().head<3>();
	auto P1 = tri.getV2().head<3>();
	auto P2 = tri.getV3().head<3>();

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			float u, v, w;
			barycentric(x + 0.5f, y + 0.5f, v0, v1, v2, u, v, w);
			if (u >= 0 && v >= 0 && w >= 0) {

				Eigen::Vector3f fragPos = u * P0 + v * P1 + w * P2;

				Eigen::Vector3f color = phongLighting(fragPos, tri.getNormal(), lightPos, viewPos, lightColor, baseColor);

				Uint8 r = static_cast<Uint8>(std::min(color.x(), 1.0f) * 255);
				Uint8 g = static_cast<Uint8>(std::min(color.y(), 1.0f) * 255);
				Uint8 b = static_cast<Uint8>(std::min(color.z(), 1.0f) * 255);

				Uint32 pixelColor = SDL_MapRGB(surface->format, r, g, b);
				putPixel(surface, x, y, pixelColor);
			}
		}
	}
}


void MyRenderer::ChangeLightColor(float R, float G, float B)
{
	lightColor.x() = R;
	lightColor.y() = G;
	lightColor.z() = B;
}

void MyRenderer::orbitLightAroundY(const Eigen::Vector3f& center, float angleRadians)
{
	Eigen::Vector3f relative = lightPos - center;
	Eigen::AngleAxisf rotation(angleRadians, Eigen::Vector3f::UnitY());
	Eigen::Vector3f rotated = rotation * relative;
	lightPos = rotated + center;
}

