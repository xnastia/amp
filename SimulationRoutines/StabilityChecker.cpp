//
// Created by mpolovyi on 15/10/15.
//

#include "StabilityChecker.h"
#include "Integrator2D.h"
#include "../Helpers/DataHelpers/DataStructures.h"

bool StabilityChecker::Check(CIntegrator2D &integrator, SimulationData &data, StabilityCheckData &stData) {
	firstCall = false;
    return integrator.Steps >= data.MaxSteps;
}
