#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "vbitmap.h"

int Ymagine_rotate(Vbitmap *outbitmap, Vbitmap *vbitmap,
			   int centerx, int centery, float angle);


#ifdef __cplusplus
};
#endif

