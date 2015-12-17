#include "graphics3d.h"
#include "vector.h"
#include "ui.h"


void load_ui(UI *powerUpInfo, int point)
{
	powerUpInfo->point = point;
	powerUpInfo->font = TTF_OpenFont("models/wellbutrin.regualr.ttf",powerUpInfo->point);
	powerUpInfo->color.a = 0;
	powerUpInfo->color.b = 0;
	powerUpInfo->color.g = 255;
	powerUpInfo->color.r = 0;
	//powerUpInfo.position =  ;
	
}
void draw_ui(/*SDL_Surface *text,TTF_Font *font, SDL_Color clrFg,*/ Vec3D camera)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0,15,14);
	glRotatef(90.0,1.0,0.0,0.0);
	
	glBegin(GL_QUADS);
		glVertex3f( -0.5f, -0.5f,15.0f ); 
		glVertex3f( 0.5f, -0.5f, 15.0f ); 
		glVertex3f( 0.5f, 0.5f, 15.0f ); 
		glVertex3f( -0.5f, 0.5f, 15.0f );
	glEnd();
	/*SDL_Rect rcDest = {0,0,0,0};
	text = TTF_RenderText_Solid(font, "Power Time Left:",clrFg);

	SDL_BlitSurface(text,NULL,text,&rcDest);
	SDL_FreeSurface(text);*/
	glPopMatrix();
}