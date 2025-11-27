#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include "particle.h"
#include <array>

#include <unordered_map>
#include <FL/gl.h>

// particles
std::array<Particle, 100> particles;

//baked particles data structure
std::unordered_map<float, std::array<Particle, 100>> bakedParticles;

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem()
{

	// fill particles vector with initial particles
	sources = new std::vector<Vec3f>();

	bake_fps = 30.0f;
	bake_start_time = 0.0f;
	bake_end_time = 0.0f;
	simulate = false;
	dirty = false;
}


void ParticleSystem::addParticleStartingAt( Vec3f WorldPoint ) {
	sources->push_back( WorldPoint );
	//std::cout << "Number of sources: " << sources->size() << std::endl;
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	int i = 0;
	std::cout << "num sources: " << sources->size() << std::endl;

	for (int i = 0; i < particles.size(); ++i) {
		Particle& p = particles[i];
		Vec3f s = (*sources)[i % sources->size()];
		p.x = s[0];
		p.y = s[1];
		p.z = s[2];
		std::cout << "source: " << s[0] << ", " << s[1] << ", " << s[2] << std::endl;

		p.vx = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
		p.vy = ((float)rand() / RAND_MAX) * 5.0f + 5.0f;
		p.vz = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
	}


	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;
	bake_start_time = t;
	//clearBaked();

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;
	bake_end_time = t;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	int i = 0;

	for (int i = 0; i < particles.size(); ++i) {
		Particle& p = particles[i];
		Vec3f s = (*sources)[i % sources->size()];
		p.x = s[0];
		p.y = s[1];
		p.z = s[2];
		p.vx = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
		p.vy = ((float)rand() / RAND_MAX) * 5.0f + 5.0f;
		p.vz = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
	}
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if(simulate && bakedParticles.find(t) == bakedParticles.end()){
		float timestep = 1 / bake_fps;
		for (int i = 0; i < 100; ++i)
		{
			Particle &p = particles[i];

			//drag
			/*
			p.vx -= p.vx * p.vx * 0.1f * timestep ? p.vx >= 0 : p.vx * p.vx * -0.1f * timestep;
			p.vy -= p.vy * p.vy * 0.1f * timestep ? p.vy >= 0 : p.vy * p.vy * -0.1f * timestep;
			p.vz -= p.vz * p.vz * 0.1f * timestep ? p.vz >= 0 : p.vz * p.vz * -0.1f * timestep;
			*/

			p.vy -= 9.81f * timestep; // gravity

			p.x += p.vx * timestep;
			
			if (p.y < 0.0f) {
				p.y = 0.0f;
				p.vy = -p.vy * 0.6f; // some energy loss on bounce
			}
			
			p.y += p.vy * timestep;
			p.z += p.vz * timestep;
		}
		bakeParticles(t);
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	for (int i = 0; i < 100; ++i) {
		Particle& p = bakedParticles[t][i];
		glPushMatrix();
		glTranslated(p.x, p.y, p.z);
		p.draw();
		glPopMatrix();
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	bakedParticles[t] = particles;

}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	resetSimulation(0.0f);
	bakedParticles.clear();
}





