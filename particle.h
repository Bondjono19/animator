 // SAMPLE_SOLUTION
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vec.h"



class Particle {

public:
	/** Constructor **/
	Particle();


	/** Destructor **/
	virtual ~Particle();

    virtual void draw();

    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;

protected:

};


#endif	// __PARTICLE_H__

