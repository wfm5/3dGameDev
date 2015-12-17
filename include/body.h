#ifndef __BODY_H__
#define __BODY_H__

#include "vector.h"
#include "collisions.h"
#include "mgl_callback.h"

typedef struct
{
	int inuse;
	int _airborne;
	int awake;
	int hasGravity;
	int hasPower;
	int moved;
	float powertime;
	float jumptime;
    Vec3D position;
    Vec3D velocity;
	Vec3D rotation;
    Cube bounds;
	Vec3D scale;
	Obj *obj;
    MglCallback touch;
    int  _needsBackoff;
    int  _done;
    Vec3D _stepOffVector;
}Body;

/**
 * @brief resets a body pre-physics calculation
 * @param body the body to reset
 */
void body_reset(Body *body);

/**
 * @brief applies any post physics processing needed for the body
 * @param body the body to process
 */
void body_process(Body *body);

void set_body(Body *player, Vec3D position, Obj *obj, Vec3D rotation, Vec3D scale);
#endif