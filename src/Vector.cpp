
#include <stdio.h>
#include "Vector.h"


Vector::Vector(const float coords[])
{
	x = coords[0];
	y = coords[1];
	z = coords[2];
	h = 1.0f;
}

Vector::Vector(const Vector &target)
{
	x = target.x;
	y = target.y;
	z = target.z;
	h = target.h;
}

bool Vector::operator==(const Vector &target)
{
	return  x == target.x &&
		y == target.y &&
		z == target.z &&
		h == target.h;
}

bool Vector::operator!=(const Vector &target)
{
	return !(x == target.x &&
		y == target.y &&
		z == target.z &&
		h == target.h);
}

Vector Vector::operator*(const float k) const
{
	return Vector(k*x, k*y, k*z);
}

Vector Vector::operator/(const float k) const
{
	return Vector(x / k, y / k, z / k);
}

Vector Vector::CrossProduct(const Vector &target) const
{
	return *this | target;
}

Vector Vector::operator | (const Vector &target) const
{
	return Vector(y * target.z - z * target.y,
		z * target.x - x * target.z,
		x * target.y - y * target.x);
}

/// Vector addition
Vector Vector::operator + (const Vector &target) const
{
	return Vector(x + target.x,
		y + target.y,
		z + target.z);
}

Vector Vector::operator - (const Vector &target) const
{
	return Vector(x - target.x, y - target.y, z - target.z);
}

Vector Vector::ComponentWise(const Vector &target) const
{
	return *this * target;
}

Vector Vector::operator * (const Vector &target) const
{
	return Vector(x * target.x,
		y * target.y,
		z * target.z);
}

float Vector::DotProduct(const Vector &target) const
{
	return *this ^ target;
}

float Vector::operator ^ (const Vector &target) const
{
	return  x * target.x +
		y * target.y +
		z * target.z;
}

Vector Vector::Normal() const
{
	return *this * InvLength();
}

void Vector::Normalize()
{
	*this = Normal();
}