#pragma once

#include <numeric>

#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "Reduction.h"
using namespace concurrency;
using namespace concurrency::graphics;

namespace MathHelpers
{
	float		SqrLength(const float_4& r) restrict(amp, cpu);
	float		SqrLength(const float_3& r) restrict(amp, cpu);
	float		SqrLength(const float_2& r) restrict(amp, cpu);
	float		Length(const float_4& r) restrict(amp, cpu);
	float		Length(const float_3& r) restrict(amp, cpu);
	float		Length(const float_2& r) restrict(amp, cpu);
	float_3		CrossProduct(float_3 A, float_3 B) restrict(amp);
	float		DotProduct(float_3 A, float_3 B) restrict(amp);
	void		RotateVector2D(float_3& vect, float ang) restrict(amp);
	void		RotateVector2D(float_2& vect, float ang) restrict(amp);
	void		RotateVector(float_3& vect, float_3& ax, float ang) restrict(amp);
	void		NormalizeVector(float_4& vect) restrict(amp);
	void		NormalizeVector(float_3& vect) restrict(amp);
	void		NormalizeVector(float_2& vect) restrict(amp);
	float_3		CountAverageVector(std::vector<float_3>& vectors);
	float_3		CountAverageVector(array<float_3>& src, uint count);
	float_3		AccumulateValue(array<float_3>& src, uint count);
	float_2		CountAverageVector(std::vector<float_2>& vectors);
	float_2		CountAverageVector(array<float_2>& src, uint count);
	float_2		AccumulateValue(array<float_2>& src, uint count);
	float		CountAverageVector(std::vector<float>& vectors);
	float		CountAverageVector(array<float>& src, uint count);
	float		AccumulateValue(array<float>& src, uint count);
	float		Dispercion(std::vector<float>& src);
}