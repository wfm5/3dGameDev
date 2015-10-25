#include "obj.h"
#include "body.h"
#include "simple_logger.h" // this may cause problems

void body_reset(Body *body)
{
    if (!body)return;
    body->_needsBackoff = 0;
    body->_done = 0;    
}

void body_process(Body *body)
{
    if (!body)return;
    if (body->_needsBackoff) //either true or false via 1 or 0
    {
        vec3d_add(body->position,body->position,body->_stepOffVector);
		//vec3d_add(body->bounds,body->bounds,body->_stepOffVector);
    }
}

void set_body(Body *target, Vec3D pos, Obj *targetObj, Vec3D scale)
{
	target->position = pos;

	target->bounds.x = pos.x;
	target->bounds.y = pos.y;
	target->bounds.z = pos.z;
	target->bounds.w = targetObj->size.x*scale.x;
	target->bounds.h = targetObj->size.y*scale.y;
	target->bounds.d = targetObj->size.z*scale.z;
	
	target->_needsBackoff = 0;
	target->used = 1;
	target->_airborne = 1;
}
void set_body_size(Body *target, Vec3D size)
{
	//target->bounds.w = 100;
	//target->bounds.h = 100;
	//target->bounds.d = 100;
	slog("width of target is: %d",target->bounds.w = size.x);
}

/*eol@eof*/