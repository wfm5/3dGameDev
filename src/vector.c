#include "vector.h"


Vec2D vec2d(float x,float y)
{
    Vec2D v = {x,y};
    return v;
}

Vec3D vec3d(float x,float y, float z)
{
    Vec3D v = {x,y,z};
    return v;
}

Vec4D vec4d(float x,float y, float z, float w)
{
    Vec4D v = {x,y,z,w};
    return v;
}
Vec3D DifferenceVector(Vec3D aPos, Vec3D bPos)
{
	vec3d_sub(aPos,aPos,bPos);
	return aPos;
}


/*eol@eof*/
