#pragma once
#include <vector>
#include "StabilityChecker.h"

class VelocityDistributionStabilityChecker :
        public StabilityChecker {
public:
    virtual bool Check(CIntegrator2D& integrator, SimulationData& data, StabilityCheckData& stData);

protected:
    std::vector<float> old_velocity_distribution;
    std::vector<float> velocity_distribution;
};
