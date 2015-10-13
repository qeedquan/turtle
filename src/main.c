#include "dat.h"
#include "fns.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        usage();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        die("%s", SDL_GetError());

    if (SDL_CreateWindowAndRenderer(W, H, SDL_WINDOW_OPENGL, &window, &render) < 0)
        die("%s", SDL_GetError());

    SDL_SetWindowTitle(window, "Turtle");

    initgfx();
    initgins();
    compile(argv[1]);
    resetvm();
    execute();
    display();
    return 0;
}

void usage(void)
{
    die("usage: file");
}

void die(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

void compile(char *file)
{
    newfile(file);
    yyparse();
    emit(1, OHLT);
    linkcode();
}

void display(void)
{
    SDL_Event ev;

    SDL_SetRenderTarget(render, NULL);
    for (;;)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                return;

            if (ev.type == SDL_KEYDOWN)
                return;
        }

        SDL_RenderCopy(render, texture, NULL, NULL);
        SDL_RenderPresent(render);
    }
}
