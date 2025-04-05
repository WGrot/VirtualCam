#pragma once
#include<Eigen/Core>
class MyRenderer
{
private:
    Eigen::Matrix4f projectionMatrix;
    int d;
	float d_step = 0.2f;
	float widthToHeightRatio;

public:
	MyRenderer(int d, float ratio) : d(d), widthToHeightRatio(ratio) {}

	Eigen::Matrix4f getProjectionMatrix() {}

	void setProjectionMatrix(Eigen::Matrix4f projectionMatrix) {}

	void setD(int d) {}

	void RecalculateProjectionMatrix() {}

	Eigen::Vector2f projectPoint(const Eigen::Vector4f& point) {}

};

