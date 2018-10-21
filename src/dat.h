#include <SDL.h>

/* must be to the power of 2 */
#define INS (4 * 1024 * 1024)
#define STK (4 * 1024 * 1024)

#undef BUFSIZ
#define BUFSIZ 8192

#define W 1024
#define H 600

typedef struct Ins Ins;
typedef struct Func Func;

struct Ins {
	int *buf;
	size_t pos;
	size_t len;
};

struct Func {
	char name[BUFSIZ];
	size_t adr;
	Func *link;
};

enum { OHLT,
       OPUSH,
       OPOP,
       OSDECBRA,
       OSBGTR,

       OCALL,
       ORET,

       OROTATE,
       ODRAW,
       OCOLOR,
       OMOVE,
       OMOVETO,
};

extern int tflag;

extern SDL_Window *window;
extern SDL_Renderer *render;
extern SDL_Texture *texture;

extern int px, py;
extern int angle;
extern int color;
