#include "VicsekChepizhko2DIntegrator.h"
#include "../Rand/amp_tinymt_rng.h"
#include "../Helpers/MathHelpers/Vicsek2DMath.h"

bool CVicsekChepizhko2DIntegrator::RealIntegrate(float noise)
{
	int numParticles = m_Task->DataNew->size();
	extent<1> computeDomain(numParticles);
	const int numTiles = numParticles / s_TileSize;
	const float deltaTime = m_ParticleVelocity;
	const float borderVel = m_BorderVelocity;
	const float doubleIntR = 2* m_IntR;

	const float_2 domainSize = m_DomainSize;
	const float intR2 = m_IntR*m_IntR;
	//initialization of random generator;

	tinymt_collection<1> rnd(computeDomain, std::rand());

	const ParticlesAmp2D& particlesIn = *m_Task->DataOld;
	const ParticlesAmp2D& particlesOut = *m_Task->DataNew;

	concurrency::parallel_for_each(computeDomain.tile<s_TileSize>(), [=](tiled_index<s_TileSize> ti) restrict(amp) {

		tile_static float_2 tilePosMemory[s_TileSize];
		tile_static float_2 tileVelMemory[s_TileSize];

		const int idxLocal = ti.local[0];
		int idxGlobal = ti.global[0];

		float_2 pos = particlesIn.pos[idxGlobal];
		float_2 vel = particlesIn.vel[idxGlobal];
		float_2 acc = 0.0f;

		// Update current Particle using all other particles
		int particleIdx = idxLocal;
		for (int tile = 0; tile < numTiles; tile++, particleIdx += s_TileSize)
		{
			// Cache current particle into shared memory to increase IO efficiency
			tilePosMemory[idxLocal] = particlesIn.pos[particleIdx];
			tileVelMemory[idxLocal] = particlesIn.vel[particleIdx];
			// Wait for caching on all threads in the tile to complete before calculation uses the data.
			ti.barrier.wait();

			// Unroll size should be multile of m_tileSize
			// Unrolling 4 helps improve perf on both ATI and nVidia cards
			// 4 is the sweet spot - increasing further adds no perf improvement while decreasing reduces perf
			for (int j = 0; j < s_TileSize;)
			{
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++], pos, tilePosMemory[j++], doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++], pos, tilePosMemory[j++], doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++], pos, tilePosMemory[j++], doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++], pos, tilePosMemory[j++], doubleIntR, intR2, domainSize);
			}

			// Wait for all threads to finish reading tile memory before allowing a new tile to start.
			ti.barrier.wait();
		}

		MathHelpers::RotateVector2D(vel, noise * (0.5 - rnd[ti.local].next_single()));

		MathHelpers::NormalizeVector(vel);

		pos += vel * deltaTime;
		Vicsek2DMath::BorderCheckMoveTopMoveBottom(pos, vel, domainSize, borderVel);

		particlesOut.pos[idxGlobal] = pos;
		particlesOut.vel[idxGlobal] = vel;
	});
	m_Task->Swap();
	return true;
}
