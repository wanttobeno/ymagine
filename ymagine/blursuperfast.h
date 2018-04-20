#ifndef blursuperfast_h__
#define blursuperfast_h__ 1

#ifdef __cplusplus
extern "C" {
#endif

int Ymagine_blurBuffer(unsigned char *pix,
					   int w, int h, int pitch, int colormode,
					   int radius);

int Ymagine_blurSuperfast(unsigned char *pix,
					  int w, int h, int pitch,
					  int colormode,
					  int radius, int niter);


#ifdef __cplusplus
};
#endif

#endif // blursuperfast_h__