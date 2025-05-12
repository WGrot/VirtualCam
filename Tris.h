#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
class Tris
{
private:
    Eigen::Vector4f v1;
    Eigen::Vector4f v2;
	Eigen::Vector4f v3;
	Eigen::Vector2f v1Projection;
	Eigen::Vector2f v2Projection;
	Eigen::Vector2f v3Projection;
	Eigen::Vector3f normal;
    int R;
    int G;
	int B;

	Eigen::Vector3f calculateNormal() const {
		Eigen::Vector3f a = v2.head<3>() - v1.head<3>();
		Eigen::Vector3f b = v3.head<3>() - v1.head<3>();
		Eigen::Vector3f normal3 = a.cross(b).normalized();
		return normal3;
	}

public:
	Tris(const Eigen::Vector4f& v1, const Eigen::Vector4f& v2, const Eigen::Vector4f& v3, int R, int G, int B)
		: v1(v1), v2(v2), v3(v3), R(R), G(G), B(B) {
		normal = calculateNormal();
	}
	Eigen::Vector4f getV1() const { return v1; }
	Eigen::Vector4f getV2() const { return v2; }
	Eigen::Vector4f getV3() const { return v3; }
	Eigen::Vector2f getV1Projection() const { return v1Projection; }
	Eigen::Vector2f getV2Projection() const { return v2Projection; }
	Eigen::Vector2f getV3Projection() const { return v3Projection; }
	void SetV1Projection(const Eigen::Vector2f& v1Projection) { this->v1Projection = v1Projection; }
	void SetV2Projection(const Eigen::Vector2f& v2Projection) { this->v2Projection = v2Projection; }
	void SetV3Projection(const Eigen::Vector2f& v3Projection) { this->v3Projection = v3Projection; }
	Eigen::Vector3f getNormal() const { return normal; }
	int getR() const { return R; }
	int getG() const { return G; }
	int getB() const { return B; }

	struct Projection {
		float min = std::numeric_limits<float>::max();
		float max = -std::numeric_limits<float>::max();
	};

	static bool trianglesIntersect(const Tris& t1, const Tris& t2);
	static bool projectionsOverlap(const Projection& proj1, const Projection& proj2);
	static Tris::Projection projectTriangle(const Eigen::Vector2f& axis, const Tris& triangle);

};
