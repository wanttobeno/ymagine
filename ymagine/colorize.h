#ifdef __cplusplus
extern "C" {
#endif

#include "vbitmap.h"


int Ymagine_colorrize( unsigned char *pixels,
					  int width ,int height,int pitch,int bpp, int color);


int Ymagine_colorize(Vbitmap *vbitmap, int color);


#ifdef __cplusplus
};
#endif

