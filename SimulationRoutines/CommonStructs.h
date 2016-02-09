#pragma once
#include <amp.h>
#include <amprt.h>
#include <amp_graphics.h>
#include <memory>

using namespace concurrency;
using namespace concurrency::graphics;

//  Data structure for storing particles on the C++ AMP accelerator.
//
//  This is an struct of arrays, rather than the more conventional array of structs used by
//  the n-body CPU example. In general structs of arrays are more efficient for GPU programming.
struct ParticlesAmp2D
{
	array<float_2, 1>& pos;
	array<float_2, 1>& vel;

public:
	ParticlesAmp2D(array<float_2, 1>& pos, array<float_2, 1>& vel) : pos(pos), vel(vel) {}

	inline int size() const { return pos.extent.size(); }
};

//  Structure storing all the data associated with processing a subset of 
//  particles on a single C++ AMP accelerator.

struct TaskData2D
{
public:
	accelerator Accelerator;
	std::shared_ptr<ParticlesAmp2D> DataOld;      // These hold references to the data
	std::shared_ptr<ParticlesAmp2D> DataNew;

private:
	array<float_2, 1> m_posOld;                 // These hold the actual data.
	array<float_2, 1> m_posNew;
	array<float_2, 1> m_velOld;
	array<float_2, 1> m_velNew;

public:
	TaskData2D(int size, accelerator_view view, accelerator acc) :
		Accelerator(acc),
		m_posOld(size, view),
		m_velOld(size, view),
		m_posNew(size, view),
		m_velNew(size, view),
		DataOld(new ParticlesAmp2D(m_posOld, m_velOld)),
		DataNew(new ParticlesAmp2D(m_posNew, m_velNew))
	{}

	void Swap()
	{
		std::swap(DataOld, DataNew);
	}
};