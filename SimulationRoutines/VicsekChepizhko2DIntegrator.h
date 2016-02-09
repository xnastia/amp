#pragma once
#include "Vicsek2DIntegrator.h"

class CVicsekChepizhko2DIntegrator : CVicsek2DIntegrator
{
public:
	CVicsekChepizhko2DIntegrator(TaskData2D& td, SimulationData data) : CVicsek2DIntegrator(td, data) {
		m_BorderVelocity = data.BorderVelocity;
	};

protected:
	double m_BorderVelocity;
	virtual bool RealIntegrate(float noise) override;
};

