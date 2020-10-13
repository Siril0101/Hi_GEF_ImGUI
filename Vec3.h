#pragma once
#include "maths/vector4.h"
class Vec3
{
public:

	Vec3() {}
	~Vec3() {}
	Vec3(const float new_x_, const float new_y_, const float new_z_);

	//	Standard operators:
	const Vec3 operator - (const Vec3& _vec) const;
	const Vec3 operator + (const Vec3& _vec) const;
	Vec3& operator -= (const Vec3& _vec);
	Vec3& operator += (const Vec3& _vec);
	const Vec3 operator * (const float _scalar) const;
	const Vec3 operator / (const float _scalar) const;
	Vec3& operator *= (const float _scalar);
	Vec3& operator /= (const float _scalar);
	const float operator[] (const int index) const;
	const Vec3 operator - () const;

	//	Getters:
	gef::Vector4 GetAsVec4() { return gef::Vector4(values_[0], values_[1], values_[2], 0.0f); }
	float GetX() const { return values_[0]; }
	float GetY() const { return values_[1]; }
	float GetZ() const { return values_[2]; }
	//	Setters:
	void SetX(float new_x_) { values_[0] = new_x_; }
	void SetY(float new_y_) { values_[1] = new_y_; }
	void SetZ(float new_z_) { values_[2] = new_z_; }
	void SetValue(float new_x_, float new_y_, float new_z_);
	//	Increment:
	void UpdateX(float increment_) { values_[0] += increment_; }
	void UpdateY(float increment_) { values_[1] += increment_; }
	void UpdateZ(float increment_) { values_[2] += increment_; }
	void UpdateValue(float x_increment_, float y_increment_, float z_increment_);

protected:
	float values_[3];
};

