#include "Vector3D.h"

Vector3D::Vector3D() :
	mat(1.0f)
{}

Vector3D::Vector3D(const Vector3D & vec) :
	mat(vec.mat)
{}

Vector3D::Vector3D(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
	mat(3, 0) = 1.0f;
}

Vector3D::Vector3D(const SMatrix41f & _mat) :
	mat(_mat)
{}

Vector3D operator^(const Vector3D & vec1, const Vector3D & vec2)
{
	static Vector3D result;

	result.x = vec1.y * vec2.z - vec1.z * vec2.y;
	result.y = vec1.z * vec2.x - vec1.x * vec2.z;
	result.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return result;
}