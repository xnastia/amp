#include "Vicsek2DMath.h"

namespace Vicsek2DMath
{
	void BodyBodyInteraction(float_2& vel, const float_2 otherParticleVel,
							 const float_2 particlePosition, const float_2 otherParticlePosition,
							 float doubleIntR, float intR2, const float_2 domainSize) restrict(amp)
	{
		float_2 r = otherParticlePosition - particlePosition;

		if (r.x > (domainSize.x - doubleIntR))
		{
			r.x -= domainSize.x;
		}
		if (r.x < -(domainSize.x - doubleIntR))
		{
			r.x += domainSize.x;
		}
		float distSqr = r.x*r.x + r.y*r.y;

		if (distSqr > intR2)
		{
			return;
		}

		vel += otherParticleVel;
	}

	void BorderCheckTransitional(float_2& pos, float_2& vel, const float_2 domainSize) restrict(amp)
	{
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}
		if (concurrency::direct3d::step(domainSize.y, pos.y))
		{
			pos.y -= domainSize.y;
		}
		if (concurrency::direct3d::step(pos.y, 0))
		{
			pos.y += domainSize.y;
		}
	}

	void BorderCheckMovingTopY(float_2 &pos, float_2 &vel, const float_2 domainSize, const float borderVel) restrict(amp)
	{
		//domainSize.x < pos.x
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		//pos.x < 0
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}

		//check. possible interract simulatenously with particle-particle interraction, and not on touch but being in interraction radius
		//domainSize.y < pos.y
		if (concurrency::direct3d::step(domainSize.y-1, pos.y))
		{
			vel += float_2(borderVel, 0);
			MathHelpers::NormalizeVector(vel);
			float dist = pos.y + vel.y - domainSize.y;
			if (concurrency::direct3d::step(0, dist))
			{
				pos.y -= 2 * dist;
				vel.y = -vel.y;
			}
		}
		//pos.y < 0
		if (concurrency::direct3d::step(pos.y, 0))
		{
			pos.y = -pos.y;
			vel.y = -vel.y;
		}
	}

	void BorderCheckMoveTopMoveBottom(float_2& pos, float_2& vel, const float_2 domainSize, const float borderVel) restrict(amp)
	{
		//domainSize.x < pos.x
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		//pos.x < 0
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}

		//check. possible interract simulatenously with particle-particle interraction, and not on touch but being in interraction radius
		//domainSize.y < pos.y
		if (concurrency::direct3d::step(domainSize.y - 1, pos.y))
		{
			vel += float_2(borderVel, 0);
			MathHelpers::NormalizeVector(vel);
			float dist = pos.y + vel.y - domainSize.y;
			if (concurrency::direct3d::step(0, dist))
			{
				pos.y -= 2 * dist;
				vel.y = -vel.y;
			}
		}
		//pos.y < 1
		if (concurrency::direct3d::step(pos.y, 1))
		{
			vel += float_2(-borderVel, 0);
			MathHelpers::NormalizeVector(vel);
			float dist = pos.y + vel.y;
			if (concurrency::direct3d::step(dist, 0))
			{
				pos.y -= 2 * dist;
				vel.y = -vel.y;
			}
		}
	}
}