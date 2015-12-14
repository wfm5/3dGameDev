#include "obj.h"
#include "body.h"
#include "vector.h"
#include "simple_logger.h" // this may cause problems

void body_reset(Body *body)
{
    if (!body)return;
    body->_needsBackoff = 0;
    body->_done = 0;    
}
void set_body(Body *player, Vec3D position, Obj *obj, Vec3D rotation, Vec3D scale)
{
	player->position = position;
	
	player->rotation = rotation;

	player->velocity.x = .1;
	player->velocity.y = .1;
	player->velocity.z = .2;
	
	player->obj = obj;

	player->_airborne = 0;

	player->bounds.x = position.x;// - (obj->size.x/2);
	player->bounds.y = position.y;// - (obj->size.y/2);
	player->bounds.z = position.z;// - (obj->size.z/2);
	player->bounds.w = obj->size.x;
	player->bounds.h = obj->size.y;
	player->bounds.d = obj->size.z;

	player->_needsBackoff = 0;
	player->_stepOffVector.x = 0;
	player->_stepOffVector.y = 0;
	player->_stepOffVector.z = .1;
	
}
void body_process(Body *body)
{
    if (!body)return;
    if (body->_needsBackoff) //either true or false via 1 or 0
    {
        vec3d_add(body->position,body->position,body->_stepOffVector);
		//vec3d_add(body->bounds,body->bounds,body->_stepOffVector);
		body->_needsBackoff = 0;
	}
}


/*eol@eof*/