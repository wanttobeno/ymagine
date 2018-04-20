#ifndef _YMAGINE_GRAPHICS_REGION_H
#define _YMAGINE_GRAPHICS_REGION_H 1


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _VrectStruct 
{
	int x;
	int y;
	int width;
	int height;
}Vrect,*PVrect;

// º∆À„Ωªµ„
int VrectComputeIntersection(const Vrect *rect1, const Vrect *rect2, Vrect *output);


#ifdef __cplusplus
};
#endif

#endif /* _YMAGINE_GRAPHICS_REGION_H */

