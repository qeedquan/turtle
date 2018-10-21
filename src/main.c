#include "dat.h"
#include "fns.h"

int tflag;

int main(int argc, char *argv[])
{
	int i;

	while (argc > 2) {
		if (argv[1][0] != '-')
			break;
		for (i = 1; argv[1][i] != '\0'; i++) {
			switch (argv[1][i]) {
			case 't':
				tflag = 1;
				break;
			}
		}

		argc--;
		argv++;
	}
	if (argc != 2)
		usage();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		die("%s", SDL_GetError());

	if (SDL_CreateWindowAndRenderer(W, H, SDL_WINDOW_RESIZABLE, &window, &render) < 0)
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
	fprintf(stderr, "usage: [options] file\n");
	fprintf(stderr, "  -t: turn on tracing\n");
	exit(2);
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

	if (SDL_SetRenderTarget(render, NULL) < 0)
		die("%s", SDL_GetError());

	for (;;) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				return;

			if (ev.type == SDL_KEYDOWN) {
				switch (ev.key.keysym.sym) {
				case SDLK_ESCAPE:
					return;
				}
			}
		}

		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		SDL_RenderCopy(render, texture, NULL, NULL);
		SDL_RenderPresent(render);
	}
}
