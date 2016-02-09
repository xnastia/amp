//
// Created by mpolovyi on 14/10/15.
//
#ifndef VICSEK_AMPCONSOLE_DATASTRUCTURE_H
#define VICSEK_AMPCONSOLE_DATASTRUCTURE_H

#include <string>
#include <stdio.h>
#include <fstream>
#include <cereal\archives\json.hpp>
#include <cereal\types\string.hpp>

enum BorderConditions{
    Transitional,
    Reflective,
    Kuette,
    DoubleKuette
};

struct SimulationData{
    BorderConditions BCond;
    int ParticleCount;
    int MaxSteps;
    int FirstTestSteps;
    float BorderVelocity;
    float SystemSizeX;
    float SystemSizeY;
    float MinNoise;
    float MaxNoise;
    float StepNoise;
    int Slices;

    SimulationData(std::string fName){
		
		std::ifstream istream(fName);
		cereal::JSONInputArchive iarchieve(istream);

		if (strcmp(iarchieve.getNodeName(), "ParticleCount") == 0)
			iarchieve.loadValue(ParticleCount);

		if (strcmp(iarchieve.getNodeName(), "MaxSteps") == 0)
			iarchieve.loadValue(MaxSteps);

		if (strcmp(iarchieve.getNodeName(), "FirstTestSteps") == 0)
			iarchieve.loadValue(FirstTestSteps);

		if (strcmp(iarchieve.getNodeName(), "BorderVelocity") == 0)
			iarchieve.loadValue(BorderVelocity);

		if (strcmp(iarchieve.getNodeName(), "SystemSizeX") == 0)
			iarchieve.loadValue(SystemSizeX);

		if (strcmp(iarchieve.getNodeName(), "SystemSizeY") == 0)
			iarchieve.loadValue(SystemSizeY);

		if (strcmp(iarchieve.getNodeName(), "MinNoise") == 0)
			iarchieve.loadValue(MinNoise);

		if (strcmp(iarchieve.getNodeName(), "MaxNoise") == 0)
			iarchieve.loadValue(MaxNoise);

		if (strcmp(iarchieve.getNodeName(), "StepNoise") == 0)
			iarchieve.loadValue(StepNoise);

		if (strcmp(iarchieve.getNodeName(), "Slices") == 0)
			iarchieve.loadValue(Slices);

		std::string bc;
		if (strcmp(iarchieve.getNodeName(), "BorderConditions") == 0)
			iarchieve.loadValue(bc);
		
		if (bc == "Transitional"){
            BCond = BorderConditions::Transitional;
        }
        if (bc == "Reflective"){
            BCond = BorderConditions::Reflective;
        }
        if (bc == "Kuette"){
            BCond = BorderConditions::Kuette;
        }
        if (bc == "DoubleKuette"){
            BCond = BorderConditions::DoubleKuette;
        }
    }
};

struct StabilityCheckData{
    float Noise;
    float dispTest;
    int testStepsCount;
};

#endif //VICSEK_AMPCONSOLE_DATASTRUCTURE_H
