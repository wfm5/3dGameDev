#include "entity.h"
#include "body.h"
#include "simple_logger.h"
#include <stdlib.h>

static Entity *__entity_list = NULL;
static int __entity_max = 0;
static int __entity_initialized = 0;

static void entity_deinitialize();

void entity_init(int maxEntity)
{
    if (__entity_initialized)
    {
        slog("already initialized");
        return;
    }
    __entity_list = (Entity *)malloc(sizeof(Entity)*maxEntity);
    memset(__entity_list,0,sizeof(Entity)*maxEntity);
    __entity_max = maxEntity;
    __entity_initialized = 1;
    slog("initialized %i entities",maxEntity);
    //atexit(entity_deinitialize);
}

static void entity_deinitialize()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
            entity_free(&__entity_list[i]);
        }
    }
    free(__entity_list);
    __entity_max = 0;
    __entity_initialized = 0;
}

void entity_free(Entity *ent)
{
    if (!ent)
    {
        slog("passed a null entity");
        return;
    }
    ent[0].inuse = 0;
    obj_free(ent->objModel);
    FreeSprite(ent->texture);
}

Entity *entity_new()
{
    int i;
    for (i = 0; i < __entity_max;i++)
    {
        if (!__entity_list[i].inuse)
        {
            memset(&__entity_list[i],0,sizeof(Entity));
            __entity_list[i].inuse = 1;
            vec3d_set(__entity_list[i].scale,1,1,1);
            vec4d_set(__entity_list[i].color,1,1,1,1);
            return &__entity_list[i];
        }
    }
    return NULL;
}
void entity_think_all()
{
    int i;
    for (i = 0; i < __entity_max; i++)
    {
        if ((__entity_list[i].inuse) &&
            (__entity_list[i].think != NULL))
        {
            __entity_list[i].think(&__entity_list[i]);
        }
    }
}
void entity_draw_all()
{
    int i;
    for (i = 0;i < __entity_max;i++)
    {
        if (__entity_list[i].inuse)
        {
			entity_draw(&__entity_list[i]);
        }
    }
}
void entity_draw(Entity *ent)
{
    if (!ent)
    {
        return;
    }
	
    obj_draw(
        ent->objModel,
        ent->body.position,
        ent->rotation,
        ent->scale,
        ent->color,
        ent->texture
    );
}


int entity_is_entity(void *data)
{
    if (!data)return 0;
    if (!__entity_initialized)return 0;
    if ((Entity *)data < __entity_list)return 0;
    if ((Entity *)data >= (__entity_list + __entity_max))return 0;
    return 1;
}
void drawBB(Body *body)
{

	int tris[36] =
 
		{1,0,2,
					 
		2,3,1,

		5,4,0,

		0,1,5,
	
		5,1,3,

		3,7,5,
 
		6,7,2,

		7,3,2,

		0,4,2,
					 
		4,6,2,
					 
		4,5,6,
					 
		5,7,6};
	
	int i;
	
	float x = body->bounds.x;//+body->position.x;

	float y = body->bounds.y;//+body->position.y;

	float z = body->bounds.z;//+body->position.z;

	float w = body->bounds.w;

	float h = body->bounds.h;

	float d = body->bounds.d;

	Vec3D verts[8] = 

	{

		{x,y+h,z},

		{x+w,y+h,z},

		{x,y,z},

		{x+w,y,z},

		{x,y+h,z+d},

		{x+w,y+h,z+d},

		{x,y,z+d},

		{x+w,y,z+d}

	};

    glEnable(GL_BLEND);

    glColorMaterial(GL_FRONT,GL_DIFFUSE);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	glRotatef(body->rotation.x, 1.0f, 0.0f, 0.0f);

    glRotatef(body->rotation.y, 0.0f, 1.0f, 0.0f);

    glRotatef(body->rotation.z, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);


		for(i=0;i<12;i++)

		{

			glColor4f(1.0f,0.0f,0.0f,0.25f);

			glVertex3f(verts[tris[3*i]].x,

			verts[tris[3*i]].y,

			verts[tris[3*i]].z);


			glColor4f(1.0f,0.0f,0.0f,0.25f);

			glVertex3f(verts[tris[3*i+1]].x,

			verts[tris[3*i+1]].y,

			verts[tris[3*i+1]].z);


			glColor4f(1.0f,0.0f,0.0f,0.25f);

			glVertex3f(verts[tris[3*i+2]].x,

			verts[tris[3*i+2]].y,

			verts[tris[3*i+2]].z);




		}

		glEnd();

    glBegin(GL_LINES);


		for(i=0;i<12;i++)

		{

			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i]].x,

			verts[tris[3*i]].y,

			verts[tris[3*i]].z);


			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i+1]].x,

			verts[tris[3*i+1]].y,

			verts[tris[3*i+1]].z);


			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i+1]].x,

			verts[tris[3*i+1]].y,

			verts[tris[3*i+1]].z);


			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i+2]].x,

			verts[tris[3*i+2]].y,

			verts[tris[3*i+2]].z);


			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i+2]].x,

			verts[tris[3*i+2]].y,

			verts[tris[3*i+2]].z);


			glColor4f(1.0f,1.0f,0.0f,1.0f);

			glVertex3f(verts[tris[3*i]].x,

			verts[tris[3*i]].y,

			verts[tris[3*i]].z);




		}

		glEnd();

    
    glDisable(GL_BLEND);

    glDisable(GL_COLOR_MATERIAL);

	slog(" bb (%f,%f,%f,%f,%f,%f)\n",x,y,z,body->bounds.w,body->bounds.h,body->bounds.d);

	glPopMatrix();

}