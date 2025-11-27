#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS, HEIGHT, ROTATE, ROTATE_Z_LEFT_SHOULDER, ROTATE_Z_RIGHT_SHOULDER, ROTATE_X_LEFT_SHOULDER, ROTATE_X_RIGHT_SHOULDER, ROTATE_Y_LEFT_SHOULDER, ROTATE_Y_RIGHT_SHOULDER,
	ROTATE_LEFT_LOW_ARM, ROTATE_RIGHT_LOW_ARM, ROTATE_LEFT_LEG_X, ROTATE_RIGHT_LEG_X, ROTATE_LEFT_LEG_Y, ROTATE_RIGHT_LEG_Y, 
	ROTATEA_LEFT_LOW_LEG, ROTATE_RIGHT_LOW_LEG, LSYSTEM_DEPTH, LSYSTEM_ANGLE, WAVE, NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif