#pragma once
struct Vector3
{
private:
	int x;
	int y;
	int z;
public:
	Vector3(int x, int y, int z) : x(x), y(y), z(z) {}
	int getX() const { return x; }
	int getY() const { return y; }
	int getZ() const { return z; }
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setZ(int newY) { y = newY; }
};

