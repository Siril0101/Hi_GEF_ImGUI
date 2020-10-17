#include "StandardObject.h"

StandardObject::StandardObject()
{
	m_up = gef::Vector4(0.0f, 1.0f, 0.0f);
	m_right = gef::Vector4(1.0f, 0.0f, 0.0f);
}

float StandardObject::ClampTo(float value_, float min_, float max_)
{
	if (value_ < min_)
		return min_;
	if (value_ > max_)
		return max_;

	return value_;
}

