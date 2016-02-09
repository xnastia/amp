//
// Created by mpolovyi on 14/10/15.
//

#ifndef VICSEK_AMPCONSOLE_CSIMULATIONCONTROLLER_H
#define VICSEK_AMPCONSOLE_CSIMULATIONCONTROLLER_H

#include "../Helpers/DataHelpers/JsonHelpers.h"
#include "../Helpers/DataHelpers/DataStructures.h"
#include "CommonStructs.h"
#include "Integrator2D.h"
#include "StabilityChecker.h"

//#include <amp_graphics.h>

class CSimulationController {
public:
    static void InitAndRun(cereal::JSONOutputArchive &writer, SimulationData simData);

    float Noise;

    int GetParticleCount(){
        return m_SimData->ParticleCount;
    };
    int GetMaxSteps(){
        return m_SimData->MaxSteps;
    };
    int GetFirstTestSteps(){
        return m_SimData->FirstTestSteps;
    };
    float GetDensity(){
        return GetParticleCount()/(GetSystemSizeX()*GetSystemSizeY());
    };
    float GetBorderVelocity(){
        return m_SimData->BorderVelocity;
    };
    float GetSystemSizeX(){
        return m_SimData->SystemSizeX;
    };
    float GetSystemSizeY(){
        return m_SimData->SystemSizeY;
    };
    float GetMinNoise(){
        return m_SimData->MinNoise;
    };
    float GetMaxNoise(){
        return m_SimData->MaxNoise;
    };
    float GetStepNoise(){
        return m_SimData->StepNoise;
    };
	int GetSteps() {
		return m_Integrator->Steps;
	}

	const std::vector<float> GetVelocityDistribution();

	const std::vector<float> GetDensityDistribution();

	const std::vector<float> GetParticleCoordinatesX();
	const std::vector<float> GetParticleCoordinatesY();
	const std::vector<float> GetParticleVelocitiesX();
	const std::vector<float> GetParticleVelocitiesY();

protected:
    CSimulationController(){
        Noise = 0;
    }
    ~CSimulationController(){
        delete m_Data;
        delete m_Integrator;
		if (m_stCheckerCount == 1)
			delete m_StabilityChecker;
		else if (m_stCheckerCount > 1)
			for (int i = 0; i < m_stCheckerCount; i++)
				delete m_StabilityChecker[i];

            delete [] m_StabilityChecker;
    }
    TaskData2D* m_Data;
    CIntegrator2D* m_Integrator;
    StabilityChecker** m_StabilityChecker;
    SimulationData* m_SimData;

    int m_stCheckerCount = 1;
};


#endif //VICSEK_AMPCONSOLE_CSIMULATIONCONTROLLER_H
