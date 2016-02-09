#pragma once
#include "Integrator2D.h"
#include "../Rand/amp_tinymt_rng.h"
#include <string>
#include "CommonStructs.h"
#include "../Helpers/DataHelpers/DataStructures.h"

class CVicsek2DIntegrator :
	public CIntegrator2D
{
public:
	CVicsek2DIntegrator(TaskData2D& td, SimulationData data) : CIntegrator2D(td, data)
	{
		PopulateTaskData(td, float_2(data.SystemSizeX, data.SystemSizeY), td.DataNew->size());
		m_ParticleVelocity = 0.1;
	};
	~CVicsek2DIntegrator() {};
	
protected:
	float m_ParticleVelocity;
	virtual bool RealIntegrate(float noise) override;
	virtual void PopulateTaskData(TaskData2D& td, float_2 domain, int partCount);

	tinymt_collection<1> m_Rnd = tinymt_collection<1>(extent<1>(1), std::rand());
};

