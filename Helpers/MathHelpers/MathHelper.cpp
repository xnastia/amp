#include "MathHelper.h"

namespace MathHelpers
{
	float_3 CrossProduct(float_3 A, float_3 B) restrict(amp)
	{
		return float_3(A.y * B.z - A.z * B.y, A.z*B.x - A.x * B.z, A.x*B.y - A.y*B.x);
	}

	float DotProduct(float_3 A, float_3 B) restrict(amp)
	{
		return (A.x*B.x + A.y*B.y + A.z*B.z);
	}

	void RandRotation2D(float_3& vel, float_3 rndRot, int noize) restrict(amp)
	{
		RotateVector2D(vel, noize);
	}

	void RotateVector2D(float_3& vect, float ang) restrict(amp)
	{
		ang = concurrency::direct3d::radians(ang);
		float_3 rot(
			vect.x * fast_math::cos(ang) - vect.y*fast_math::sin(ang),
			vect.x * fast_math::sin(ang) + vect.y*fast_math::cos(ang),
			0);

		vect = rot;
	}

	void RotateVector2D(float_2& vect, float ang) restrict(amp)
	{
		ang = concurrency::direct3d::radians(ang);
		float_2 rot(
			vect.x * fast_math::cos(ang) - vect.y*fast_math::sin(ang),
			vect.x * fast_math::sin(ang) + vect.y*fast_math::cos(ang));
		vect = rot;
	}

	void RotateVector(float_3& vect, float_3& ax, float ang) restrict(amp)
	{
		ang = concurrency::direct3d::radians(ang);

		auto c = concurrency::fast_math::cos(ang);
		auto s = concurrency::fast_math::sin(ang);
		auto c1 = 1 - c;

		auto c1xy = c1*ax.x*ax.y;
		auto c1xz = c1*ax.x*ax.z;
		auto c1yz = c1*ax.y*ax.z;

		auto sx = s*ax.x;
		auto sy = s*ax.y;
		auto sz = s*ax.z;

		//Rotation matrix
		float R11(c + c1 * ax.x*ax.x), R12(c1xy + sz), R13(c1xz - sy);
		float R21(c1xy - sz), R22(c + c1*ax.y*ax.y), R23(c1yz + sx);
		float R31(c1xz + sy), R32(c1yz - sx), R33(c + c1*ax.z*ax.z);

		float_3 ans(R11*vect.x + R12*vect.y + R13*vect.z, R21*vect.x + R22*vect.y + R23*vect.z, R31*vect.x + R32*vect.y + R33*vect.z);
		vect = ans;
	}

	void NormalizeVector(float_4& vect) restrict(amp)
	{
		auto a = Length(vect);
		if (a > 0.00001 && (a-1) <= 0.000001)
		{
			vect *= concurrency::fast_math::rsqrt(SqrLength(vect));
		}
	}

	void NormalizeVector(float_3& vect) restrict(amp)
	{
		auto a = Length(vect);
		if (a > 0.00001 && (a-1) <= 0.000001)
		{
			vect *= concurrency::fast_math::rsqrt(SqrLength(vect));
		}
	}

	void NormalizeVector(float_2& vect) restrict(amp)
	{
		auto a = SqrLength(vect);
		if (a > 0.00001)
		{
			vect *= concurrency::fast_math::rsqrt(SqrLength(vect));
		}
	}

	float SqrLength(const float_4& r) restrict(amp, cpu)
	{
		return r.x * r.x + r.y * r.y + r.z * r.z + r.w * r.w;
	}
	float SqrLength(const float_3& r) restrict(amp, cpu)
	{
		return r.x * r.x + r.y * r.y + r.z * r.z;
	}
	float SqrLength(const float_2& r) restrict(amp, cpu)
	{
		return r.x * r.x + r.y * r.y;
	}

	float Length(const float_4& r) restrict(amp, cpu)
	{
		return fast_math::sqrt(SqrLength(r));
	}
	float Length(const float_3& r) restrict(amp, cpu)
	{
		return fast_math::sqrt(SqrLength(r));
	}
	float Length(const float_2& r) restrict(amp, cpu)
	{
		return fast_math::sqrt(SqrLength(r));
	}

	float_3 CountAverageVector(std::vector<float_3>& vectors)
	{
		array<float_3, 1> arr(vectors.size());
		copy(vectors.begin(), vectors.end(), arr);

		return CountAverageVector(arr, vectors.size());
	}

	float_3 CountAverageVector(array<float_3>& src, uint count)
	{
		return CReduction::reduction_tiled_4<512>(src, count) / count;
	}

	float_3 AccumulateValue(array<float_3>& src, uint count)
	{
		const unsigned window_width = 8;

		// Using array as temporary memory.
		auto element_count = count;
		// Takes care of the sum of tail elements.
		float_3 tail_sum(0.f);
		//if ((element_count % window_width) != 0 && element_count > window_width)
		//{
		//	tail_sum = std::accumulate(src.begin() + ((element_count - 1) / window_width) * window_width, src.end(), 0.f);
		//}
		array_view<float_3, 1> av_tail_sum(1, &tail_sum);

		// Each thread reduces window_width elements.
		unsigned prev_s = element_count;
		for (unsigned s = element_count / window_width; s > 0; s /= window_width)
		{
			parallel_for_each(extent<1>(s), [=, &src](index<1> idx) restrict(amp) {
				float_3 sum(0.f);
				for (unsigned i = 0; i < window_width; i++)
				{
					sum += src[idx + i * s];
				}
				src[idx] = sum;

				// Reduce the tail in cases where the number of elements is not divisible.
				// Note: execution of this section may negatively affect the performance.
				// In production code the problem size passed to the reduction should
				// be a power of the window_width. Please refer to the blog post for more
				// information.
				if ((idx[0] == s - 1) && ((s % window_width) != 0) && (s > window_width))
				{
					for (unsigned i = ((s - 1) / window_width) * window_width; i < s; i++)
					{
						av_tail_sum[0] += src[i];
					}
				}
			});
			
			prev_s = s;
		}

		// Perform any remaining reduction on the CPU.
		std::vector<float_3> result(prev_s);
		copy(src.section(0, prev_s), result.begin());
		av_tail_sum.synchronize();
		return std::accumulate(result.begin(), result.end(), tail_sum);
	}

	float_2 CountAverageVector(std::vector<float_2>& vectors)
	{
		array<float_2, 1> arr(vectors.size());
		copy(vectors.begin(), vectors.end(), arr);

		return CountAverageVector(arr, vectors.size());
	}

	float_2 CountAverageVector(array<float_2>& src, uint count)
	{
		return AccumulateValue(src, count) / count;
	}

	float_2 AccumulateValue(array<float_2>& src, uint count)
	{
		const unsigned window_width = 8;

		// Using array as temporary memory.
		auto element_count = count;
		// Takes care of the sum of tail elements.
		float_2 tail_sum(0.f);
		//if ((element_count % window_width) != 0 && element_count > window_width)
		//{
		//	tail_sum = std::accumulate(src.begin() + ((element_count - 1) / window_width) * window_width, src.end(), 0.f);
		//}
		array_view<float_2, 1> av_tail_sum(1, &tail_sum);

		// Each thread reduces window_width elements.
		unsigned prev_s = element_count;
		for (unsigned s = element_count / window_width; s > 0; s /= window_width)
		{
			parallel_for_each(extent<1>(s), [=, &src](index<1> idx) restrict(amp) {
				float_2 sum(0.f);
				for (unsigned i = 0; i < window_width; i++)
				{
					sum += src[idx + i * s];
				}
				src[idx] = sum;

				// Reduce the tail in cases where the number of elements is not divisible.
				// Note: execution of this section may negatively affect the performance.
				// In production code the problem size passed to the reduction should
				// be a power of the window_width. Please refer to the blog post for more
				// information.
				if ((idx[0] == s - 1) && ((s % window_width) != 0) && (s > window_width))
				{
					for (unsigned i = ((s - 1) / window_width) * window_width; i < s; i++)
					{
						av_tail_sum[0] += src[i];
					}
				}
			});
			prev_s = s;
		}

		// Perform any remaining reduction on the CPU.
		std::vector<float_2> result(prev_s);
		copy(src.section(0, prev_s), result.begin());
		av_tail_sum.synchronize();

		return std::accumulate(result.begin(), result.end(), tail_sum);
	}

	float CountAverageVector(std::vector<float>& vectors)
	{
		array<float, 1> arr(vectors.size());
		copy(vectors.begin(), vectors.end(), arr);

		return CountAverageVector(arr, vectors.size());
	}

	float CountAverageVector(array<float>& src, uint count)
	{

		return AccumulateValue(src, count) / count;
	}

	float AccumulateValue(array<float>& src, uint count)
	{
		const unsigned window_width = 8;

		// Using array as temporary memory.
		auto element_count = count;
		// Takes care of the sum of tail elements.
		float tail_sum(0.f);
		//if ((element_count % window_width) != 0 && element_count > window_width)
		//{
		//	tail_sum = std::accumulate(src.begin() + ((element_count - 1) / window_width) * window_width, src.end(), 0.f);
		//}
		array_view<float, 1> av_tail_sum(1, &tail_sum);

		// Each thread reduces window_width elements.
		unsigned prev_s = element_count;
		for (unsigned s = element_count / window_width; s > 0; s /= window_width)
		{
			parallel_for_each(extent<1>(s), [=, &src](index<1> idx) restrict(amp) {
				float sum(0.f);
				for (unsigned i = 0; i < window_width; i++)
				{
					sum += src[idx + i * s];
				}
				src[idx] = sum;

				// Reduce the tail in cases where the number of elements is not divisible.
				// Note: execution of this section may negatively affect the performance.
				// In production code the problem size passed to the reduction should
				// be a power of the window_width. Please refer to the blog post for more
				// information.
				if ((idx[0] == s - 1) && ((s % window_width) != 0) && (s > window_width))
				{
					for (unsigned i = ((s - 1) / window_width) * window_width; i < s; i++)
					{
						av_tail_sum[0] += src[i];
					}
				}
			});
			prev_s = s;
		}

		// Perform any remaining reduction on the CPU.
		std::vector<float> result(prev_s);
		copy(src.section(0, prev_s), result.begin());
		av_tail_sum.synchronize();

		return std::accumulate(result.begin(), result.end(), tail_sum);
	}

	float Dispercion(std::vector<float>& src)
	{
		float disper = 0;
		float summ = 0;
		for (size_t i = 0; i < src.size(); i++)
		{
			disper += src[i] * src[i];
			summ += src[i];
		}
		disper = (disper + (summ * summ) / src.size()) / src.size();

		return disper;
	}
}