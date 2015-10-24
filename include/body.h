#ifndef __BODY_H__
#define __BODY_H__

#include "vector.h"
#include "collisions.h"
#include "mgl_callback.h"

typedef struct
{
    Vec3D position;
    Vec3D velocity;
    Cube bounds;
    MglCallback touch;
    int  _needsBackoff;
    int  _done;
    Vec3D _stepOffVector;
	int _airborne;
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

/**
*@brief setting up bounding boxes
*@params a target body, 1 Vec3d to set point, 1 object to attach body to
*/
void set_body(Body *target, Vec3D pos, Obj *targetObj, Vec3D scale);
void set_body_size(Body *target, Vec3D size);

#endif