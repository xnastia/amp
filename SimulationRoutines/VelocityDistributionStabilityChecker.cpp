//
// Created by mpolovyi on 15/10/15.
//

#include "Integrator2D.h"
#include "VelocityDistributionStabilityChecker.h"
#include "../Helpers/DataHelpers/DataStructures.h"
#include <math.h>

bool VelocityDistributionStabilityChecker::Check(CIntegrator2D &integrator, SimulationData& data, StabilityCheckData& stData) {
	if (firstCall) {
		integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise, data.Slices);
		old_velocity_distribution = integrator.AverVelocityModuleDistribution;
		return false;
	}

    integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise, data.Slices);
    velocity_distribution = integrator.AverVelocityModuleDistribution;

    bool ret = true;
    for (int i = 0; i < velocity_distribution.size(); i++){
        ret = ret && (std::abs(velocity_distribution[i]-old_velocity_distribution[i]) < stData.dispTest);
		old_velocity_distribution[i] = velocity_distribution[i];
    }
    return ret;
}
