//
// Created by mpolovyi on 14/10/15.
//

#include <math.h>
#include "../Helpers/MathHelpers/MathHelper.h"
#include "CSimulationController.h"
#include "Vicsek2DIntegrator.h"
#include "VicsekKulinsky2DIntegrator.h"
#include "VicsekChepizhko2DIntegrator.h"
#include "StabilityChecker.h"
#include "VelocityDispersionStabilityChecker.h"
#include "VelocityDistributionStabilityChecker.h"

#include <amp_graphics.h>

using concurrency::graphics::float_2;

void CSimulationController::InitAndRun(cereal::JSONOutputArchive &writer, SimulationData simData) {
    CSimulationController* sim = new CSimulationController();
    sim->m_SimData = &simData;
    sim->m_Data = new TaskData2D(simData.ParticleCount, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator));

    switch (simData.BCond) {
        case BorderConditions::Transitional:
            sim->m_Integrator = new CVicsek2DIntegrator(*sim->m_Data, simData);
            break;
        case BorderConditions::Kuette:
            sim->m_Integrator = (CVicsek2DIntegrator*) new CVicsekKulinsky2DIntegrator(*sim->m_Data, simData);
            break;
		case BorderConditions::DoubleKuette:
			sim->m_Integrator = (CVicsek2DIntegrator*) new CVicsekChepizhko2DIntegrator(*sim->m_Data, simData);
        default:
            sim->m_Integrator = new CVicsek2DIntegrator(*sim->m_Data, simData);
            break;
    }
    StartDataFlow(writer);

	sim->m_stCheckerCount = 2;
	sim->m_StabilityChecker = new StabilityChecker*[2];

    sim->m_StabilityChecker[0] = new VelocityDispersionStabilityChecker();
	sim->m_StabilityChecker[1] = new StabilityChecker();

	WriteData(writer, *sim);
    for (auto noise = simData.MaxNoise; noise > simData.MinNoise; noise+=simData.StepNoise){
		bool isStable = false;
		std::cout << "Simulating noise " << noise << std::endl;

		int firstSteps = simData.FirstTestSteps;
		StabilityCheckData stCheckData;
		stCheckData.dispTest = 0.0001;
		bool secondTest = false;
		while (!isStable)
		{
			stCheckData.testStepsCount = (int)round(firstSteps*0.05);
			sim->Noise = noise;
			sim->m_Integrator->IntegrateFor(firstSteps, sim->Noise);
			stCheckData.Noise = sim->Noise;
			for (int i = 0; i < sim->m_stCheckerCount; i++) {
				auto tmp = sim->m_StabilityChecker[i]->Check(*sim->m_Integrator, simData, stCheckData);
				isStable = isStable || tmp;
			}
			WriteData(writer, *sim);
			std::cout << "Sim steps = " << (*sim->m_Integrator).Steps << std::endl;
			if (secondTest) {
				firstSteps *= 2;
				secondTest = false;
			}
			else {
				secondTest = true;
			}
		}
		sim->m_Integrator->Steps = 0;
    }
    EndDataFlow(writer);

    delete sim;
}

const std::vector<float> CSimulationController::GetVelocityDistribution() {
	auto tmp = m_Integrator->GetAverVelocityDistributionY(m_SimData->Slices);

    std::vector<float> ret;

    for(auto vel : tmp){
        ret.push_back(MathHelpers::Length(vel));
    }
	
	return ret;
}

const std::vector<float> CSimulationController::GetDensityDistribution() {
	return m_Integrator->GetAverDensityDistributionY(m_SimData->Slices);
}

const std::vector<float> CSimulationController::GetParticleCoordinatesX() {
	std::vector<float_2> tmp(m_Data->DataOld->size());
	std::vector<float> ret;
	concurrency::copy(m_Data->DataOld->pos, tmp.begin());

	for (int i = 0; i < m_Data->DataOld->size(); i++) {
		ret.push_back(tmp[i].x);
	}

	return ret;
}

const std::vector<float> CSimulationController::GetParticleCoordinatesY() {
	std::vector<float_2> tmp(m_Data->DataOld->size());
	std::vector<float> ret;
	concurrency::copy(m_Data->DataOld->pos, tmp.begin());

	for (int i = 0; i < m_Data->DataOld->size(); i++) {
		ret.push_back(tmp[i].y);
	}

	return ret;
}

const std::vector<float> CSimulationController::GetParticleVelocitiesX() {
	std::vector<float_2> tmp(m_Data->DataOld->size());
	std::vector<float> ret;
	concurrency::copy(m_Data->DataOld->vel, tmp.begin());

	for (int i = 0; i < m_Data->DataOld->size(); i++) {
		ret.push_back(tmp[i].x);
	}

	return ret;
}

const std::vector<float> CSimulationController::GetParticleVelocitiesY() {
	std::vector<float_2> tmp(m_Data->DataOld->size());
	std::vector<float> ret;
	concurrency::copy(m_Data->DataOld->vel, tmp.begin());

	for (int i = 0; i < m_Data->DataOld->size(); i++) {
		ret.push_back(tmp[i].y);
	}

	return ret;
}