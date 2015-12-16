/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "sprite.h"
#include "body.h"
#include "vector.h"
#include "entity.h"
#include <math.h>
#include <stdio.h>

void set_camera(Vec3D position, Vec3D rotation);
void set_camera_toplayer(Vec3D player, Vec3D cameraPos);
void update_BB(Body *body);
void update_stageBB(Body *body);
//position update
void update_BB(Body *body)
{
	body->bounds.x = body->position.x - (body->obj->size.x/2);
	body->bounds.y = body->position.y - (body->obj->size.y/2);
	body->bounds.z = body->position.z - (body->obj->size.z/2);
}
void update_powerBB(Body *body, Vec3D vector)
{
	body->bounds.x = body->position.x - (body->obj->size.x/2);
	body->bounds.y = body->position.y - (body->obj->size.y/2);
	body->bounds.z = body->position.z - (body->obj->size.z/2);

	vec3d_cpy(body->position,vector);
}
void update_stageBB(Body *body)
{
	//body->bounds.y = body->position.y - (body->obj->size.y);
	//cube_set(body->bounds,1,1,1,1,1,1);
}

void touch_callback(void *data, void *context)
{
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        slog("%s is ",other->name);
    }
    slog("touching me.... touching youuuuuuuu");
}

void think(Entity *self)
{
    if (!self)return;
    switch(self->state)
    {
        case 0:
            self->frame = 0;
            break;
        case 1:
            self->frame += 0.3;
            if (self->frame >= 24)self->frame = 0;
            break;
        case 2:
            self->frame -= 0.3;
            if (self->frame < 0)self->frame = 23;
            break;
    }
    self->objModel = self->objAnimation[(int)self->frame];
}

Entity *newCube(Vec3D position, char *name)
{
    Entity * ent;
    char buffer[255];
   int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    //for (i = 0; i < 24;i++)
   // {
        sprintf(buffer,"models/cube.obj");
        ent->objAnimation[0] = obj_load(buffer);
   // }
    ent->objModel = ent->objAnimation[0];
    ent->texture = LoadSprite("models/white.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
	ent->scale = vec3d(1,1,1);
	ent->color = vec4d(1,1,1,1);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 0;
    sprintf(ent->name,"%s",name);
    ent->think = think;
    ent->state = 0;
    mgl_callback_set(&ent->body.touch,touch_callback,ent);
    return ent;
}

int main(int argc, char *argv[])
{
    GLuint vao;
    float r = 0;
	int i;
    GLuint triangleBufferObject;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,-10,15};
    Vec3D cameraRotation = {73,0,0};
	Vec3D boxPosition = {0,0,0};
	
	Vec3D pupPosition1 = {0,10,10};
	Vec3D pupPosition2 = {0,10,10};
	Vec3D pupPosition3 = {0,15,10};

    Vec3D boxRotation = {0,0,0};

	Vec3D boxSize = {1,1,1};
	Vec3D stageSize = {1,1,1};
	Vec3D stagePosition = {0,0,0};
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	int diag;
	unsigned int now;
	unsigned int last;
	unsigned int time;
	float deltaTime;

	Body mainBody;
	Body powerBody1;
	Body powerBody2;
	Body powerBody3;
	Body stageBody;

	SDL_Event e;
    Obj *obj,*bgobj, *obj2;
    Sprite *texture,*bgtext;
    const float triangleVertices[] = {
        0.0f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.366f, 0.0f, 1.0f,
        -0.5f, -0.366f, 0.0f, 1.0f,
        //next part contains vertex colors
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f  
    }; //we love you vertices!
    
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
	InitSpriteList();
    model_init();
    obj_init();
    entity_init(255);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); //make our vertex array object, we need it to restore state we set after binding it. Re-binding reloads the state associated with it.
    
    glGenBuffers(1, &triangleBufferObject); //create the buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangleBufferObject); //we're "using" this one now
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //formatting the data for the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind any buffers
    
    obj = obj_load("models/cube.obj");
    texture = LoadSprite("models/cube_text.png",1024,1024);

    bgobj = obj_load("models/level.obj"); 
    bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
	obj2 = obj_load("models/cube.obj");
	
	set_body(&mainBody, boxPosition, obj, boxRotation, boxSize);
	set_body(&stageBody,stagePosition,bgobj,boxRotation, stageSize);
	set_body(&powerBody1,boxPosition, obj, boxRotation, boxSize);
	set_body(&powerBody2,boxPosition, obj, boxRotation, boxSize);
	set_body(&powerBody3,boxPosition, obj, boxRotation, boxSize);
	
	diag = 0;
	now = 0;
	last = 0;
	time = 0;


    while (bGameLoopRunning)
    {
		//entity_think_all();

		update_BB(&mainBody);
		update_stageBB(&stageBody);
		update_powerBB(&powerBody1, pupPosition1);
		update_BB(&powerBody2);
		update_BB(&powerBody3);
		
		now = SDL_GetTicks();
		time = now/1000;
		if(now>last+1000)
		{
			deltaTime = ((float)(now-last)/1000);
			last = now;
			//slog("%d",time);
		}

        diag = 0;
		
		while ( SDL_PollEvent(&e) ) 
		{
			if (state[SDL_SCANCODE_ESCAPE])
            {
                bGameLoopRunning = 0;
            }
			if(state[SDL_SCANCODE_1])
			{
				mainBody.awake = 1;
			}
			if(state[SDL_SCANCODE_W])
			{
				SET_FLAG(diag,MOVE_FORWARD);
				/*vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            0
                        ));*/
			}
			if(state[SDL_SCANCODE_S])
			{
				SET_FLAG(diag,MOVE_BACKWARDS);
				/*vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            sin(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.z * DEGTORAD),
                            0
                        ));*/
			}
			if(state[SDL_SCANCODE_A])
			{
				/*vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));*/
				SET_FLAG(diag,MOVE_LEFT);
			}
			if(state[SDL_SCANCODE_D])
			{
				/*vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));*/
				SET_FLAG(diag,MOVE_RIGHT);
			}
			if(state[SDL_SCANCODE_SPACE])
			{
				SET_FLAG(diag,MOVE_JUMP);
				//cameraPosition.z++;
			}
			if(state[SDL_SCANCODE_RIGHT])
			{
				mainBody.rotation.z -= 3;
				//cameraRotation.z -= 1;
			}
			if(state[SDL_SCANCODE_LEFT])
			{
				mainBody.rotation.z += 3;
				//cameraRotation.z += 1;
			}
			if(state[SDL_SCANCODE_UP])
			{
				cameraRotation.x += 1;
			}
			if(state[SDL_SCANCODE_DOWN])
			{
				 cameraRotation.x -= 1;
			}
			if(state[SDL_SCANCODE_Z])
			{
				mainBody.position.z--;
				//cameraPosition.z--;
			}
			if(state[SDL_SCANCODE_SPACE])
			{
				SET_FLAG(diag,MOVE_JUMP);
			}

			if(diag==MOVE_FORWARD)
				{
					vec3d_add(
                        mainBody.position,
                        mainBody.position,
                        vec3d(
                            0,
							mainBody.velocity.y,
                            0
                        ));
				}
			else if(diag==(MOVE_FORWARD | MOVE_RIGHT))
				{
					vec3d_add(
                        mainBody.position,
                        mainBody.position,
                        vec3d(
							mainBody.velocity.x,
							mainBody.velocity.y,
                            0
                        ));
				}
			else if(diag==(MOVE_RIGHT))
				{
					vec3d_add(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            mainBody.velocity.x,
							0,
                            0
                        ));
				}
			else if(diag==(MOVE_RIGHT | MOVE_BACKWARDS))
				{
					vec3d_add(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            mainBody.velocity.x,
							0,
                            0
                        ));
					vec3d_sub(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            0,
							mainBody.velocity.y,
                            0
                        ));	
			}
			else if(diag==(MOVE_BACKWARDS))
				{
					vec3d_sub(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            0,
							mainBody.velocity.y,
                            0
                        ));
				}
			else if(diag==(MOVE_LEFT))
				{
					vec3d_sub(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            mainBody.velocity.x,
							0,
                            0
                        ));
				}
			else if(diag==(MOVE_LEFT | MOVE_FORWARD))
				{
					vec3d_sub(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            mainBody.velocity.x,
							0,
                            0
                        ));
					vec3d_add(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            0,
							mainBody.velocity.y,
                            0
                        ));
				}
			else if(diag==(MOVE_LEFT | MOVE_BACKWARDS))
				{
					vec3d_sub(
						mainBody.position,
                        mainBody.position,
                        vec3d(
                            mainBody.velocity.x,
							mainBody.velocity.y,
                            0
                        ));
				}
			else if(diag==(MOVE_JUMP))
			{
 				mainBody._airborne = 1;
			}
		}
		//Some checks 1.power ups
		if(mainBody.hasPower && mainBody.awake)
		{
			mainBody.powertime -= deltaTime;
			slog("%f", mainBody.powertime);
			if(mainBody.powertime <= 0)
			{
				mainBody.hasPower = 0;
				vec3d_set(mainBody.velocity, 0.1, 0.1, 0.1);
			}
		}else
		{
			mainBody.powertime = 4000;
		}
		//collisions
		if(mainBody._airborne)
		{
			if(mainBody.jumptime >0)
			{
				vec3d_add(
						mainBody.position,
						mainBody.position,
						vec3d(
							0,
							0,
							mainBody.velocity.z
							));
				mainBody.jumptime -= deltaTime;
			}
			else if(mainBody.jumptime <= 0)
			{
				mainBody.hasGravity = 1;
			}
			if(mainBody.hasGravity && mainBody._airborne)
				{
					if(mainBody.jumptime <= 0)
					{
						vec3d_sub(
								mainBody.position,
								mainBody.position,
								vec3d(
									0,
									0,
									mainBody.velocity.z
									));
					}
				}
		}
		if(mainBody.awake)
		{
			if(cube_cube_intersection2(mainBody.bounds,stageBody.bounds))
			{
				REMOVE_FLAG(diag,MOVE_JUMP);
				mainBody.hasGravity = 0;
				mainBody._airborne = 0;
				mainBody.jumptime = 40;
				mainBody._needsBackoff = 1;
				body_process(&mainBody);
				//slog("player in stage %d", time);	
			}
								/*if(cube_cube_intersection2(powerBody1.bounds,stageBody.bounds))
		{
			mainBody._needsBackoff = 1;
			body_process(&powerBody1);
				
		}*/
			if(cube_cube_intersection2(mainBody.bounds,powerBody1.bounds))
			{
				//body_process(&mainBody);
				if(powerBody1.inuse)
				{
					slog("mystical power of Speed");
					mainBody.hasPower = 1;
					mainBody.powertime = 4000;
					mainBody.velocity.x *= 3;
					mainBody.velocity.y *= 3;
				}
				powerBody1.inuse = 0;
			}
		}
       
		graphics3d_frame_begin();
	   //begin drawing
		glPushMatrix();
	
        set_camera(
            cameraPosition,
            cameraRotation);
		
		
   //     obj_draw(          //stage ground
			//stageBody.obj,
			//stageBody.position,
   //         vec3d(0,90,0),
			//stageBody.scale, //scale
   //         vec4d(1,1,1,1),
   //         bgtext
   //     );
        
		//drawBB(&mainBody);
		//drawBB(&stageBody);
		drawBB(&powerBody1);

        obj_draw(          //player draw
			mainBody.obj,
			mainBody.position,
			mainBody.rotation,
			mainBody.scale,
            vec4d(1,1,1,1),
            texture
        );
		if(powerBody1.inuse){
			obj_draw(          //powerup1 draw
				powerBody1.obj,
				powerBody1.position,
			    vec3d(90,r++,0),
				vec3d(1,1,1),
			    vec4d(1,1,1,1),
			    texture
			);
		}
			//obj_draw(          //powerup2 draw
			//    obj2,
			//	pupPosition2,
			//    vec3d(r++,0,0),
			//	vec3d(1,1,1),
			//    vec4d(1,1,1,1),
			//    texture
			//);
		
			//obj_draw(          //powerup3 draw
			//    obj2,
			//	pupPosition3,
			//    vec3d(90,0,r++),
			//	vec3d(1,1,1),
			//    vec4d(1,1,1,1),
			//    texture
			//);
		
        if (r > 360)r -= 360;
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}
void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

/*eol@eof*/