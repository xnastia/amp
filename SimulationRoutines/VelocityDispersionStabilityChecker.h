//
// Created by mpolovyi on 15/10/15.
//

#ifndef VICSEK_AMPCONSOLE_VELOCITYDISPERSIONSTABILITYCHECKER_H
#define VICSEK_AMPCONSOLE_VELOCITYDISPERSIONSTABILITYCHECKER_H

#include "VelocityDistributionStabilityChecker.h"

class VelocityDispersionStabilityChecker :
        public VelocityDistributionStabilityChecker {
public:
    virtual bool Check(CIntegrator2D& integrator, SimulationData& data, StabilityCheckData& stData);

};


#endif //VICSEK_AMPCONSOLE_VELOCITYDISPERSIONSTABILITYCHECKER_H
