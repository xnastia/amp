//
// Created by mpolovyi on 14/10/15.
//

#ifndef VICSEK_AMPCONSOLE_JSONHELPERS_H
#define VICSEK_AMPCONSOLE_JSONHELPERS_H

#include <cereal\archives\json.hpp>

class CSimulationController;

void StartDataFlow(cereal::JSONOutputArchive &writer);

void WriteData(cereal::JSONOutputArchive &writer, CSimulationController& simContr);

void WriteSimParams(cereal::JSONOutputArchive &writer, CSimulationController&simContr);

void EndDataFlow(cereal::JSONOutputArchive &writer);

#endif //VICSEK_AMPCONSOLE_JSONHELPERS_H
