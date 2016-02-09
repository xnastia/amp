//
// Created by mpolovyi on 15/10/15.
//

#include "VelocityDispersionStabilityChecker.h"
#include "Integrator2D.h"
#include <math.h>
#include "../Helpers/DataHelpers/DataStructures.h"

bool VelocityDispersionStabilityChecker::Check(CIntegrator2D &integrator, SimulationData &data,
                                               StabilityCheckData &stData) {
	if (firstCall) {
		integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise, data.Slices);
		old_velocity_distribution = integrator.AverVelocityModuleDistribution;
		return false;
	}

	integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise, data.Slices);
	velocity_distribution = integrator.AverVelocityModuleDistribution;

    float aver_old = 0;
    float aver_new = 0;
    float averSq_old = 0;
    float averSq_new = 0;
    for (int i = 0; i < velocity_distribution.size(); i++){
        aver_old += old_velocity_distribution[i];
        aver_new += velocity_distribution[i];

        averSq_old += old_velocity_distribution[i]*old_velocity_distribution[i];
        averSq_new += velocity_distribution[i]*velocity_distribution[i];
		old_velocity_distribution[i] = velocity_distribution[i];
    }

	aver_old /= data.ParticleCount;
	aver_new /= data.ParticleCount;
	averSq_old /= data.ParticleCount;
	averSq_new /= data.ParticleCount;

    return std::abs(sqrt(aver_old*aver_old - averSq_old) - sqrt(aver_new*aver_new - averSq_new)) <= 1/sqrt(data.ParticleCount);
}
