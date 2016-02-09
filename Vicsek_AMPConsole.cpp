// Vicsek_AMPConsole.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Helpers/DataHelpers/DataStructures.h"
#include "SimulationRoutines/CSimulationController.h"

void Simulate(std::string fname){
    SimulationData data = SimulationData(fname);
	std::string::size_type pos = fname.find_last_of("\\/");
    std::string name = "Results";
    int i = 1;
	while (std::ifstream(fname.substr(0, pos + 1) + name + std::to_string(i) + "_" + fname.substr(pos+1, fname.length() - 1))){ i++; }

    std::stringstream s;
	s << fname.substr(0,pos+1) << name << std::to_string(i) << "_" << fname.substr(pos+1, fname.length() - 1);
	std::fstream stream(s.str().c_str(), std::fstream::out);
	cereal::JSONOutputArchive writer(stream);

    CSimulationController::InitAndRun(writer, data);
}

int main(int argc, char** argv)
{
	std::wcout << accelerator(accelerator::default_accelerator).description << std::endl;

	if(argc > 1) {
		Simulate(argv[1]);
	}
	else
	{
		Simulate("C:\\simulation_json\\Data0.json");
	}

	return 0;
}