#include "Vec3.h"

Vec3::Vec3(const float new_x_, const float new_y_, const float new_z_)
{
	values_[0] = new_x_;
	values_[1] = new_y_;
	values_[2] = new_z_;
}

//	Standard operators:
const Vec3 Vec3::operator-(const Vec3 & _vec) const
{
	return Vec3(values_[0] - _vec.GetX(), values_[1] - _vec.GetY(), values_[2] - _vec.GetZ());
}

const Vec3 Vec3::operator+(const Vec3 & _vec) const
{
	return Vec3(values_[0] + _vec.GetX(), values_[1] + _vec.GetY(), values_[2] + _vec.GetZ());
}

Vec3 & Vec3::operator-=(const Vec3 & _vec)
{
	values_[0] -= _vec.GetX();
	values_[1] -= _vec.GetY();
	values_[2] -= _vec.GetZ();

	return *this;
}

Vec3 & Vec3::operator+=(const Vec3 & _vec)
{
	values_[0] += _vec.GetX();
	values_[1] += _vec.GetY();
	values_[2] += _vec.GetZ();

	return *this;
}

const Vec3 Vec3::operator*(const float _scalar) const
{
	return Vec3(values_[0] * _scalar, values_[1] * _scalar, values_[2] * _scalar);
}

const Vec3 Vec3::operator/(const float _scalar) const
{
	return Vec3(values_[0] / _scalar, values_[1] / _scalar, values_[2] / _scalar);
}

Vec3 & Vec3::operator*=(const float _scalar)
{
	values_[0] *= _scalar;
	values_[1] *= _scalar;
	values_[2] *= _scalar;

	return *this;
}

Vec3 & Vec3::operator/=(const float _scalar)
{
	values_[0] /= _scalar;
	values_[1] /= _scalar;
	values_[2] /= _scalar;

	return *this;
}

const float Vec3::operator[](const int index) const
{
	return values_[index];
}

const Vec3 Vec3::operator-() const
{
	return Vec3(-values_[0], -values_[1], -values_[2]);
}

//	Set all values:
void Vec3::SetValue(float new_x_, float new_y_, float new_z_)
{
	values_[0] = new_x_;
	values_[1] = new_y_;
	values_[2] = new_z_;
}
//	Increment all values:
void Vec3::UpdateValue(float x_increment_, float y_increment_, float z_increment_)
{
	values_[0] += x_increment_;
	values_[1] += y_increment_;
	values_[2] += z_increment_;
}
