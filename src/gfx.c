#include "dat.h"
#include "fns.h"

SDL_Window   *window;
SDL_Renderer *render;
SDL_Texture  *texture;

int px, py;
int angle;
int color;

void initgfx(void)
{
    color = lookupcolor("Black");
    angle = 90;
    px = 0;
    py = 0;

    if (!SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, W, H))
        die("%s", SDL_GetError());

    if (SDL_SetRenderTarget(render, texture) < 0)
        die("%s", SDL_GetError());

    SDL_SetRenderDrawColor(render, 100, 120, 100, 255);
    SDL_RenderClear(render);
}
