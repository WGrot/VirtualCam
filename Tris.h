#pragma once
#include <Eigen/Core>

class Tris
{
private:
    Eigen::Vector4f v1;
    Eigen::Vector4f v2;
	Eigen::Vector4f v3;
    
    int R;
    int G;
	int B;


public:
	Tris(const Eigen::Vector4f& v1, const Eigen::Vector4f& v2, const Eigen::Vector4f& v3, int R, int G, int B)
		: v1(v1), v2(v2), v3(v3), R(R), G(G), B(B) {
	}
	Eigen::Vector4f getV1() const { return v1; }
	Eigen::Vector4f getV2() const { return v2; }
	Eigen::Vector4f getV3() const { return v3; }
	int getR() const { return R; }
	int getG() const { return G; }
	int getB() const { return B; }
};
