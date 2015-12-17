#ifndef __UI__
#define __UI__
#include <SDL_ttf.h>

typedef struct
{
	TTF_Font *font;
	Vec3D position;
	Vec3D rotation;
	Vec3D scale;
	int point;
	SDL_Color color;
}UI;
void load_ui(UI *powerUpInfo, int point);
void draw_ui( Vec3D camera);

/*SDL_Surface *text,TTF_Font *font, SDL_Color clrFg,*/

#endif