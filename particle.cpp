 // SAMPLE_SOLUTION

#include "particle.h"
#include <FL/gl.h>
#include <stdlib.h>
#include "modelerdraw.h"

/** Constructor **/
Particle::Particle() 
{
    // TODO
}

/** Destructor **/
Particle::~Particle() 
{
    // TODO
}

void Particle::draw() {
	setDiffuseColor(1.0f, 0.0f, 1.0f);
    drawSphere(0.1);
}

