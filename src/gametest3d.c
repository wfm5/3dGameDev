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
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "sprite.h"
#include "body.h"
#include "vector.h"
#include <math.h>

void set_camera(Vec3D position, Vec3D rotation);
void set_camera_toplayer(Vec3D player, Vec3D cameraPos);

int main(int argc, char *argv[])
{
    GLuint vao;
    float r = 0;
    GLuint triangleBufferObject;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,-10,15};
    Vec3D cameraRotation = {73,0,0};
	Vec3D boxPosition = {0,0,10};
    Vec3D boxRotation = {0,0,0};
	Vec3D stageSize = {40,10,10};
	Vec3D stagePosition = {0,0,0};
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
    model_init();
    obj_init();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); //make our vertex array object, we need it to restore state we set after binding it. Re-binding reloads the state associated with it.
    
    glGenBuffers(1, &triangleBufferObject); //create the buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangleBufferObject); //we're "using" this one now
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //formatting the data for the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind any buffers
    
    obj = obj_load("models/cube.obj");
    texture = LoadSprite("models/cube_text.png",1024,1024);

    bgobj = obj_load("models/cube.obj");//obj_load("models/mountainvillage.obj");
    bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
	obj2 = obj_load("models/cube.obj");

    set_body(&mainBody, boxPosition, obj, vec3d(1,1,1));
    set_body(&powerBody1, vec3d(15,0,10), obj2, vec3d(1,1,1));
	set_body(&powerBody2, vec3d(10,0,10), obj2, vec3d(1,1,1));
	set_body(&powerBody3, vec3d(5,0,10), obj2, vec3d(1,1,1));
	set_body(&stageBody, stagePosition, bgobj, stageSize);
	//set_body_size(&stageBody, stageSize);
	

    while (bGameLoopRunning)
    {
        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    cameraPosition.z++;
					//mainBody.position.z++;

                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                    cameraPosition.z--;
					mainBody.position.z--;
                }
                else if (e.key.keysym.sym == SDLK_w)
                {
					//camera should move to a place reletive to mainBody
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
					//move box as well
					mainBody.position.y++;
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            sin(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
					mainBody.position.y--;
                }
                else if (e.key.keysym.sym == SDLK_d)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
					mainBody.position.x++;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
					mainBody.position.x--;
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    cameraRotation.z += 1;
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraRotation.z -= 1;
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    cameraRotation.x += 1;
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    cameraRotation.x -= 1;
                }
            }
        }
		//i plan to use cube cube intersection once i fix it to stop colliding everywhere
		if(point_cube_intersection(mainBody.position,powerBody1.position,vec3d(1,1,1))) //power up 1
		{
			slog("you now have some power");
			powerBody1.used = 0;
		}
//		if(cube_cube_intersection(mainBody.bounds,powerBody1.bounds)) //power up 1
	//	{
		//	slog("you now have some powerAYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYE");
			
	//	}
		if(point_cube_intersection(mainBody.position,powerBody2.position,vec3d(1,1,1))) //power up 2
		{
			slog("you now have some other power");
			powerBody2.used = 0;

		}
		if(point_cube_intersection(mainBody.position,powerBody3.position,vec3d(1,1,1))) //power up 3
		{
			slog("you now have some mystical power");
			powerBody3.used = 0;
		}
		if(point_cube_intersection(mainBody.position,stageBody.position,stageSize)) //get out the stage please
		{
			//slog("you are in the ground");
			mainBody._airborne = 0;
			if(!mainBody._needsBackoff)
				mainBody._needsBackoff = 1;
			mainBody._stepOffVector = vec3d(0,0,0.02); //DifferenceVector(mainBody.position,stageBody.position);
			body_process(&mainBody);
			body_reset(&mainBody);
		}else{
			mainBody._airborne = 1;
			//slog("you are in the air");
		}
		if(mainBody._airborne)
		{	
			mainBody.position.z -= 0.02;
		}
        graphics3d_frame_begin();


		//begin drawing
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
	//	set_camera_toplayer(mainBody.position, cameraPosition);
		
        obj_draw(          //stage ground
            bgobj,
            stagePosition,
            vec3d(90,90,0),
            stageSize, //scale
            vec4d(1,1,1,1),
            bgtext
        );
        
        obj_draw(          //player draw
            obj,
			mainBody.position,
            boxRotation,
            vec3d(0.5,0.5,0.5),
            vec4d(1,1,1,1),
            texture
        );
		if(powerBody1.used){
			obj_draw(          //powerup1 draw
			    obj2,
				powerBody1.position,
			    vec3d(90,r++,0),
			    vec3d(0.5,0.5,0.5),
			    vec4d(1,1,1,1),
			    texture
			);
		}
		if(powerBody2.used)
		{
			obj_draw(          //powerup2 draw
			    obj2,
				powerBody2.position,
			    vec3d(90,r--,0),
			    vec3d(0.5,0.5,0.5),
			    vec4d(1,1,1,1),
			    texture
			);
		}
		if(powerBody3.used){
			obj_draw(          //powerup3 draw
			    obj2,
				powerBody3.position,
			    vec3d(90,0,r++),
			    vec3d(0.5,0.5,0.5),
			    vec4d(1,1,1,1),
			    texture
			);
		}
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

void set_camera_toplayer(Vec3D player, Vec3D cameraPos) //set camera to this position{0,-10,2} relative to the boxposition
{
	vec3d_add(
		cameraPos,
		player,
		vec3d(0,-10,2)
		);
}

/*eol@eof*/