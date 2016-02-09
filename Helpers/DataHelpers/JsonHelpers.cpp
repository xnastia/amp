//
// Created by mpolovyi on 14/10/15.
//

#include "../../SimulationRoutines/CSimulationController.h"
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include "JsonHelpers.h"
#include <sstream>

struct SaveData {
	std::vector<float> CoorsX;
	std::vector<float> CoorsY;
	std::vector<float> VelocX;
	std::vector<float> VelocY;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CoorsX, CoorsY, VelocX, VelocY); // serialize things by passing them to the archive
	}
};

void StartDataFlow(cereal::JSONOutputArchive &writer){
	writer.makeArray();
}

void WriteData(cereal::JSONOutputArchive &writer, CSimulationController& simContr){
    writer.startNode();
    {
        WriteSimParams(writer, simContr);
		writer(cereal::make_nvp("Noise", simContr.Noise));
		writer(cereal::make_nvp("Steps", simContr.GetSteps()));
		writer(cereal::make_nvp("AverageVelocityDistribution", simContr.GetVelocityDistribution()));
		writer(cereal::make_nvp("AverageDencityDistribution", simContr.GetDensityDistribution()));

		writer(cereal::make_nvp("ParticleCoordinatesX", simContr.GetParticleCoordinatesX()));
		writer(cereal::make_nvp("ParticleCoordinatesY", simContr.GetParticleCoordinatesY()));
		writer(cereal::make_nvp("ParticleVelocitiesX", simContr.GetParticleVelocitiesX()));
		writer(cereal::make_nvp("ParticleVelocitiesY", simContr.GetParticleVelocitiesY()));
		/*
		{
			const float* tmpVal = &(simContr.GetParticleCoordinatesX())[0];
			writer.saveBinaryValue(tmpVal, sizeof(float)*simContr.GetParticleCount(), "ParticleCoordinatesX");
		}
		{
			const float* tmpVal = &(simContr.GetParticleCoordinatesY())[0];
			writer.saveBinaryValue(tmpVal, sizeof(float)*simContr.GetParticleCount(), "ParticleCoordinatesY");
		}
		{
			const float* tmpVal = &(simContr.GetParticleVelocitiesX())[0];
			writer.saveBinaryValue(tmpVal, sizeof(float)*simContr.GetParticleCount(), "ParticleVelocitiesX");
		}
		{
			const float* tmpVal = &(simContr.GetParticleVelocitiesY())[0];
			writer.saveBinaryValue(tmpVal, sizeof(float)*simContr.GetParticleCount(), "ParticleVelocitiesY");
		}*/
    }
    writer.finishNode();
}

void WriteSimParams(cereal::JSONOutputArchive &writer, CSimulationController&simContr) {
	writer(cereal::make_nvp("ParticleCount", simContr.GetParticleCount()));
	writer(cereal::make_nvp("MaxSteps", simContr.GetMaxSteps()));
	writer(cereal::make_nvp("FirstTestSteps", simContr.GetFirstTestSteps()));
	writer(cereal::make_nvp("Density", simContr.GetDensity()));
	writer(cereal::make_nvp("BorderVelocity", simContr.GetBorderVelocity()));
	writer(cereal::make_nvp("SystemSizeX", simContr.GetSystemSizeX()));
	writer(cereal::make_nvp("SystemSizeY", simContr.GetSystemSizeY()));
	writer(cereal::make_nvp("MinNoise", simContr.GetMinNoise()));
	writer(cereal::make_nvp("MaxNoise", simContr.GetMaxNoise()));
	writer(cereal::make_nvp("StepNoise", simContr.GetStepNoise()));
}

void EndDataFlow(cereal::JSONOutputArchive &writer){
	// writer.finishNode();
}