// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"
#include "particleSystem.h"
#include "mat.h"
#include "vec.h"
#include <vector>


#include <GL/gl.h>
#include <FL/gl.h>
#include <stdlib.h>


#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1


// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView
{
public:
	RobotArm(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) {
	}
	virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char* label)
{
	return new RobotArm(x, y, w, h, label);
}

ParticleSystem *ps = new ParticleSystem();

Mat4f getModelViewMatrix()
{
        GLfloat m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        Mat4f matMV(m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15] );

        return matMV.transpose(); // convert to row major
}

void SpawnParticles( Mat4f CameraTransforms )
{
	Mat4f WorldMatrix =  CameraTransforms.inverse() * getModelViewMatrix();
	Vec3f WorldPoint = Vec3f(WorldMatrix[0][3], WorldMatrix[1][3], WorldMatrix[2][3]);
	ps->addParticleStartingAt(WorldPoint);
	//std::cout << "Spawn at: " << WorldPoint[0] << ", " << WorldPoint[1] << ", " << WorldPoint[2] << ", " << WorldPoint[3] << std::endl;
}

void RobotArm::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	Mat4f CameraTransforms = getModelViewMatrix();
	ps->clearSources();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5,0,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);

	glPushMatrix();
	glScaled(1, VAL(HEIGHT), 1);
	glTranslated(VAL(XPOS), VAL(YPOS)+1.5, VAL(ZPOS));
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(VAL(ROTATE), 0.0, 0.0, 1.0);

		glPushMatrix();
		glScaled(1,0.5,1);
		drawCylinder(1, 0.25, 0.425);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, 1.25);
		drawSphere(0.25);
	
			glPushMatrix();
			glRotated(90, 0.0, 1.0, 0.0);
			glTranslated(0, 0, 0.25);
			std::string lsystem = "-X";
			for (int i = 0; i < VAL(LSYSTEM_DEPTH); ++i) {
				lsystem = LSystemStep(lsystem);
			}
			drawLSystem(lsystem, VAL(LSYSTEM_ANGLE));

			glRotated(180, 1.0, 0.0, 0.0);
			glTranslated(0, 0, 0.5);

			drawLSystem(lsystem, VAL(LSYSTEM_ANGLE));
			glPopMatrix();

		glPopMatrix();

		//right arm
		glPushMatrix();
		glTranslated(0, 0, 1);
		glRotated(-90, 0.0, 1.0, 0.0);
		glTranslated(0, 0, 0.25);
		glRotated(VAL(ROTATE_Z_RIGHT_SHOULDER), 0.0, 1.0, 0.0);
		glRotated(VAL(ROTATE_X_RIGHT_SHOULDER), 1.0, 0.0, 0.0);
		glRotated(VAL(ROTATE_Y_RIGHT_SHOULDER), 0.0, 0.0, 1.0);
		drawCylinder(0.5, 0.05, 0.025);
		
			glPushMatrix();
				glTranslated(0, 0, 0.5);
				glRotated(VAL(ROTATE_RIGHT_LOW_ARM), 1.0, 0.0, 0.0);
				drawCylinder(0.5, 0.025, 0.05);

				glPushMatrix();
				//draw satr (bell)
				glTranslated(0, 0, 0.5);
				glScaled(0.4,0.4,0.4);
				drawStar(1,0.3);

				SpawnParticles(CameraTransforms);

				glPopMatrix();

			glPopMatrix();
		glPopMatrix();

		//left arm
		glPushMatrix();
		glTranslated(0, 0, 1);
		glRotated(90, 0.0, 1.0, 0.0);
		glTranslated(0, 0, 0.25);
		if (VAL(WAVE) != 0) {

			glRotated(VAL(WAVE), 0, -1, 0);
		}
		else {
			glRotated(VAL(ROTATE_Z_LEFT_SHOULDER), 0.0, -1.0, 0.0);
			glRotated(VAL(ROTATE_X_LEFT_SHOULDER), 1.0, 0.0, 0.0);
			glRotated(VAL(ROTATE_Y_LEFT_SHOULDER), 0.0, 0.0, 1.0);
		}
		drawCylinder(0.5, 0.05, 0.025);
		
			glPushMatrix();
				glTranslated(0, 0, 0.5);
				if (VAL(WAVE) != 0) {
					glRotated(VAL(WAVE), 0, -1, 0);
				}
				else {
					glRotated(VAL(ROTATE_LEFT_LOW_ARM), 1.0, 0.0, 0.0);
				}
				drawCylinder(0.5, 0.025, 0.05);
					glPushMatrix();
						//draw hand
						glRotated(-90, 0, 0, 1);
						glRotated(-90, 0, 1, 0);
						glTranslated(0.5, 0, 0);
						drawBox(0.2, 0.1, 0.05);
						glTranslated(0, 0, 0.05);
						drawBox(0.2, 0.1, 0.05);
						glTranslated(0, -0.1, 0);
						drawBox(0.2, 0.1, 0.05);
						glTranslated(0, 0, -0.05);
						drawBox(0.2, 0.1, 0.05);

						SpawnParticles(CameraTransforms);

						glPushMatrix();
							//draw fingers
							if(VAL(WAVE)!=0){
								glTranslated(0.2, 0, 0);
								glRotated(VAL(WAVE), 0, 1, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
							}
							else {
								glTranslated(0.2, 0, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
								glTranslated(0, 0.05, 0);
								drawBox(0.15, 0.03, 0.03);
							}
							glPopMatrix();
						glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		//right leg
		glPushMatrix();
		glRotated(VAL(ROTATE_RIGHT_LEG_Y), 0.0, 0.0, 1.0);
		glRotated(VAL(ROTATE_RIGHT_LEG_X), -1.0, 0.0, 0.0);
		glTranslated(-0.3, -0.125, 0);
		drawBox(0.25, 0.25, -0.75);
			glPushMatrix();
				glTranslated(0.0625, 0.0625, -0.75);
				glRotated(VAL(ROTATE_RIGHT_LOW_LEG), 1.0, 0.0, 0.0);
				drawBox(0.125, 0.125, -0.75);
			glPopMatrix();
		glPopMatrix();

		//left leg
		glPushMatrix();
		glRotated(VAL(ROTATE_LEFT_LEG_Y), 0.0, 0.0, 1.0);
		glRotated(VAL(ROTATE_LEFT_LEG_X), -1.0, 0.0, 0.0);
		glTranslated(0.05, -0.125, 0);
		drawBox(0.25, 0.25, -0.75);
			glPushMatrix();
				glTranslated(0.0625, 0.0625, -0.75);
				glRotated(VAL(ROTATEA_LEFT_LOW_LEG), 1.0, 0.0, 0.0);
				drawBox(0.125, 0.125, -0.75);
			glPopMatrix();
		glPopMatrix();
		
	glPopMatrix();

}

int main()
{
	ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[ROTATE_Z_LEFT_SHOULDER] = ModelerControl("Left Shoulder Z", -90, 90, 1, 0);
	controls[ROTATE_Z_RIGHT_SHOULDER] = ModelerControl("Right Shoulder Z", -90, 90, 1, 0);
	controls[ROTATE_X_LEFT_SHOULDER] = ModelerControl("Left Shoulder X", -90, 90, 1, 0);
	controls[ROTATE_X_RIGHT_SHOULDER] = ModelerControl("Right Shoulder X", -90, 90, 1, 0);
	controls[ROTATE_Y_LEFT_SHOULDER] = ModelerControl("Left Shoulder Y", -90, 90, 1, 0);
	controls[ROTATE_Y_RIGHT_SHOULDER] = ModelerControl("Right Shoulder Y", -90, 90, 1, 0);
	controls[ROTATE_LEFT_LOW_ARM] = ModelerControl("Left Low Arm", 0, 90, 1, 0);
	controls[ROTATE_RIGHT_LOW_ARM] = ModelerControl("Right Low Arm", 0, 90, 1, 0);
	controls[ROTATE_LEFT_LEG_X] = ModelerControl("Left Leg X", -90, 90, 1, 0);
	controls[ROTATE_RIGHT_LEG_X] = ModelerControl("Right Leg X", -90, 90, 1, 0);
	controls[ROTATE_LEFT_LEG_Y] = ModelerControl("Left Leg Y",	 -70, 70, 1, 0);
	controls[ROTATE_RIGHT_LEG_Y] = ModelerControl("Right Leg Y", -70, 70, 1, 0);
	controls[ROTATEA_LEFT_LOW_LEG] = ModelerControl("Left Low Leg", 0, 90, 1, 0);
	controls[ROTATE_RIGHT_LOW_LEG] = ModelerControl("Right Low Leg", 0, 90, 1, 0);
	controls[WAVE] = ModelerControl("Wave", 0, 100, 1, 0);
	controls[LSYSTEM_DEPTH] = ModelerControl("Horn Depth", 0, 5, 1, 3);
	controls[LSYSTEM_ANGLE] = ModelerControl("Horn Angle", 0, 90, 1, 25);


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.
	ModelerApplication::Instance()->SetParticleSystem(ps);
	ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);

	return ModelerApplication::Instance()->Run();
}
