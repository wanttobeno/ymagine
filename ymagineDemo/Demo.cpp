#include <tchar.h>
#include <windows.h>
#include "Image.h"

#include "../ymagine/ymagine.h"

#ifdef _DEBUG
#pragma  comment(lib,"../Debug/ymagine.lib")
#else
#pragma  comment(lib,"../Release/ymagine.lib")
#endif // _DEBUG

inline int DoPadding(int value, int padvalue) {
	return (value + padvalue - 1) & ~(padvalue - 1);
}

int main(int agrc,char** agrv)
{
	CImage * pImg = new CImage;
	pImg->Load(_T("bmp.bmp"));
	pImg->Backup();
	int nWidth = pImg->GetWidth();
	int nHeight = pImg->GetHeight();
	BYTE* pBmpColorData = pImg->GetBits();
	int nPitch = pImg->GetPitch();
	Ymagine_blurBuffer(pBmpColorData,nWidth,nHeight,nPitch,VBITMAP_COLOR_RGB,10);
	pImg->Save(_T("bmp_deal_blurBuffer.bmp"));
	pImg->CopyBackDataToCurData();
	Ymagine_blurSuperfast(pBmpColorData,nWidth,nHeight,nPitch,VBITMAP_COLOR_RGB,20,20);
	pImg->Save(_T("bmp_deal_blurSuperfast.bmp"));
	pImg->CopyBackDataToCurData();

//////////////////////////////////
	Vbitmap  vBit = {0};
	vBit.bitmaptype = VBITMAP_MEMORY;
	vBit.height = nHeight;
	vBit.width = nWidth;
	vBit.pitch = nPitch;
	vBit.pixels = pBmpColorData;
	vBit.colormode = VBITMAP_COLOR_RGB;
	
	Vbitmap out ={0};
	out.width = nWidth*2;
	out.height = nHeight*2;
	out.pitch = _DibPitch24(out.width);
	out.colormode = VBITMAP_COLOR_RGB;
	out.bitmaptype = VBITMAP_MEMORY;
	int nSize = _DibSize24(out.width,out.height);
	out.pixels = (unsigned char*)_HeapAlloc(nSize);

	int nRet =Ymagine_rotate(&out,&vBit,nWidth/2,nHeight/2,-30);
	if (nRet == YMAGINE_OK)
	{
		CImage* pBmpAttack = new CImage;
		pBmpAttack->Attach(out.width,out.height,out.pixels);
		pBmpAttack->Save(_T("bmp_deal_rotatet.bmp"));
		delete pBmpAttack;
	}
//////////////////////////////////
	pImg->CopyBackDataToCurData();
	int nRet4 = Ymagine_colorrize(pImg->GetBits(),pImg->GetWidth(),pImg->GetHeight(),pImg->GetPitch(),3,10);
	if (nRet4 == 0)
	{
		pImg->Save(_T("bmp_deal_rotatetcolorrize.bmp"));
	}
	delete pImg;
	
	unsigned char asfsf =0;
}