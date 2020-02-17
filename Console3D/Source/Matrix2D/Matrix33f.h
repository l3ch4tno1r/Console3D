#pragma once

#include "Stack\SqrSMatrix.h"

#define PI 3.14159265
#define TORAD(A) (PI * A) / 180

using SqrSMatrix33f = Matrix::SqrSMatrix<float, 3>;
using    SMatrix31f = Matrix::SMatrix<float, 3, 1>;

class Matrix33f : public SqrSMatrix33f
{
public:
	Matrix33f();

	Matrix33f(float x, float y, float a);

	void SetRotationAngle(float a);

	void SetTranslation(float x, float y);
};

class Matrix31f : public SMatrix31f
{
public:
	Matrix31f();

	Matrix31f(float x, float y);

	float& X();
	float& Y();

	const float& X() const;
	const float& Y() const;
};