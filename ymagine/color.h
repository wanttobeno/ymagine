/**
 * Copyright 2013 Yahoo! Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may
 * obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License. See accompanying LICENSE file.
 */

#ifndef _YMAGINE_GRAPHICS_COLOR_H
#define _YMAGINE_GRAPHICS_COLOR_H 1


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#  define YOPTIMIZE_SPEED
#  define YINLINE
#define ALOGE(x) printf(x)
#define ALOGD(x,...)

#define YMAGINE_OK    ((int)  0)
#define YMAGINE_ERROR ((int) -1)

#ifdef MAX
#undef MAX
#endif
#define MAX(a,b) ((a)>(b)?(a):(b))

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) ((a)<(b)?(a):(b))

#define VBITMAP_ENABLE_GLOBAL_REF 0
#define VBITMAP_MAX_PSNR 100
#define LOG10 ((double) 2.302585092994046)


/**
 * scale the image to half its width or height, ignoring 3 out of 4 pixels
 */
#define YMAGINE_SCALE_HALF_QUICK   11

/**
 * scale the image to half its width or height, averaging 4 pixels into 1 pixel
 */
#define YMAGINE_SCALE_HALF_AVERAGE 12

typedef enum 
{
	VBITMAP_COLOR_RGBA=0,
	VBITMAP_COLOR_RGB,
	VBITMAP_COLOR_GRAYSCALE,
	VBITMAP_COLOR_YUV,
	VBITMAP_COLOR_CMYK,
	VBITMAP_COLOR_rgbA,
	VBITMAP_COLOR_YCbCr,
	VBITMAP_COLOR_ARGB,
	VBITMAP_COLOR_Argb
}EColorMode;

//#define VBITMAP_COLOR_RGBA          0
//#define VBITMAP_COLOR_RGB           1
//#define VBITMAP_COLOR_GRAYSCALE     2
//#define VBITMAP_COLOR_YUV           3
//#define VBITMAP_COLOR_CMYK          4
//#define VBITMAP_COLOR_rgbA          5
//#define VBITMAP_COLOR_YCbCr         6
//#define VBITMAP_COLOR_ARGB          7
//#define VBITMAP_COLOR_Argb          8
//

#define  Ymem_malloc(x) (unsigned char*)malloc(x)
#define  Ymem_free(x) free(x)


#define YMAGINE_THEME_DEFAULT   -1
#define YMAGINE_THEME_NONE       0
#define YMAGINE_THEME_SATURATION 1



#ifdef __cplusplus
extern "C" {
#endif


/**
 * Prepare YUV to RGB conversion table, call at least once before any
 * YUV to RGB transformations.
 *
 * @return YMAGINE_OK on success
 */
int ycolor_yuv_prepare();

/**
 * @brief Convert NV21 pixel buffer to RGB(A)888 pixel buffer
 *
 * Take the given NV21 pixel buffer and convert it to an RGB(A)888 pixel buffer.
 * The output buffer of minimum size width*height*3 must be allocated by the
 * caller. If downscaleis true, the resulting image has half the width and height
 * of the original NV21 image. Downscaling simply ignores 3 out of 4 pixels in the Y
 * plane.
 *
 * @param width of the image in indata
 * @param height of the image in outdata
 * @param colormode of the output buffer (VBITMAP_COLOR_RGB or VBITMAP_COLOR_RGBA)
 * @param downscale set to YMAGINE_SCALE_HALF_AVERAGE or YMAGINE_SCALE_HALF_QUICK to obtain an output image of width/2 and height/2
 *
 * @return YMAGINE_OK on success
 */
int ycolor_nv21torgb(int width, int height, const unsigned char* indata, unsigned char* outdata, int colormode, int downscale);


uint32_t YcolorRGBA(int r, int g, int b, int a);
uint32_t YcolorRGB(int r, int g, int b);
uint32_t YcolorHSVA(int h, int s, int v, int a);

uint32_t YcolorHSV(int h, int s, int v);


int YcolorRGBtoRed(uint32_t rgb);
int YcolorRGBtoGreen(uint32_t rgb);
int YcolorRGBtoBlue(uint32_t rgb);
int YcolorRGBtoAlpha(uint32_t rgb);

int YcolorRGBtoHSV(uint32_t rgb);
int YcolorHSVtoHue(uint32_t hsv);
int YcolorHSVtoRGB(uint32_t hsv);




#ifdef __cplusplus
};
#endif

#endif /* _YMAGINE_GRAPHICS_COLOR_H */
