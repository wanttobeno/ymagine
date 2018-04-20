#include <Windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"

#pragma comment(lib,"comctl32.lib")

#include "Image.h"
#include "DlgHelper.h"

#include "../ymagine/ymagine.h"

#ifdef _DEBUG
#pragma  comment(lib,"../Debug/ymagine.lib")
#else
#pragma  comment(lib,"../Release/ymagine.lib")
#endif // _DEBUG



//#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
//#endif

typedef struct _BmpInfo
{
	int nWidth;
	int nHeight;
	int nBitCount;
	int nPitch;
	BYTE *pBmpColorData;
}BmpInfo,*PBmpInfo;



CImage* g_pImg = NULL;

void SetPicControlPic(HWND hDlg);
void OnDropFiles(WPARAM wParam, HWND hDlg);
void OnTextChanged(WPARAM wParam, HWND hDlg);
void OnButtonClick(WPARAM wParam, HWND hDlg);
void SetValueToDefault(HWND hDlg);


void CheckValue0255(int &nValue)
{
	if (!nValue) return;
	if (nValue<0)
		nValue = 0;
	if (nValue>255)
		nValue = 255;
}

HBITMAP MakeBitmap(HDC hDc, LPBYTE lpBits, long lWidth, long lHeight, WORD wBitCount)
{
	BITMAPINFO bitinfo;
	memset(&bitinfo, 0, sizeof(BITMAPINFO));
	bitinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitinfo.bmiHeader.biWidth = lWidth;
	bitinfo.bmiHeader.biHeight = lHeight;
	bitinfo.bmiHeader.biPlanes = 1;
	bitinfo.bmiHeader.biBitCount = wBitCount;
	bitinfo.bmiHeader.biCompression = BI_RGB;
	bitinfo.bmiHeader.biSizeImage = lWidth*lHeight*(wBitCount / 8);
	bitinfo.bmiHeader.biXPelsPerMeter = 96;
	bitinfo.bmiHeader.biYPelsPerMeter = 96;
	bitinfo.bmiHeader.biClrUsed = 0;
	bitinfo.bmiHeader.biClrImportant = 0;

	return CreateDIBitmap(hDc, &bitinfo.bmiHeader, CBM_INIT, lpBits, &bitinfo, DIB_RGB_COLORS);
}

BOOL OpenFile(HWND hWnd)
{
	TCHAR szFile[MAX_PATH] = {0};       // buffer for file name
	if(GetOpenFilePath(hWnd,szFile))
	{
		if (g_pImg)
		{
			delete g_pImg;
			g_pImg = NULL;
		}
		g_pImg = new CImage;
		g_pImg->Load(szFile);
		g_pImg->Backup();
		SetPicControlPic(hWnd);
		return TRUE;
	}
	return FALSE;
}

void SetPicControlPic(HWND hDlg)
{
	HBITMAP hBitmap = MakeBitmap(GetDC(hDlg),g_pImg->GetBits(),g_pImg->GetWidth(),g_pImg->GetHeight(),24);
	if (!hBitmap) return;

	HWND hPic = GetDlgItem(hDlg,IDC_PIC);
	HBITMAP dwRetHandle = (HBITMAP)SendMessage(hPic, STM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap)); 
	if (dwRetHandle)
	{
		if (dwRetHandle!=hBitmap)
			DeleteObject(dwRetHandle);
	}
}

void OnDropFiles(WPARAM wParam, HWND hDlg)
{
	TCHAR* pszCurrentFile = GetDragFile(wParam, hDlg);
	if (pszCurrentFile)
	{
		if (g_pImg)
		{
			delete g_pImg;
		}
		g_pImg = new CImage;
		g_pImg->Load(pszCurrentFile);
		g_pImg->Backup();
		SetPicControlPic(hDlg);
		GlobalFree(pszCurrentFile);
	}
}

void ApplyFilterBlur(HWND hDlg);

void ApplyFilterColorsize(HWND hDlg);

void ApplyFilterRotate(HWND hDlg);

void OnTextChanged(WPARAM wParam, HWND hDlg)
{
	int nMsg =  HIWORD(wParam);
	switch(nMsg)
	{
	case EN_CHANGE:
		{
			if (!g_pImg)
				break;
			if (g_pImg->GetSize() < 10)
				break;
			int nID = LOWORD(wParam);
			switch(nID)
			{
			case  IDC_INPUT_ROTATE:
				{
					ApplyFilterRotate(hDlg);
					return;
				}
				break;
			case IDC_INPUT_COLOR:
				{
					ApplyFilterColorsize(hDlg);
				}
				break;
			case IDC_INTPUT_BLUR:
				{
					ApplyFilterBlur(hDlg);
				}
				break;
			}
		}
		break;
	}
}

void OnButtonClick(WPARAM wParam, HWND hDlg)
{
	int nID =  LOWORD(wParam);
	switch(nID)
	{
	case  IDC_BTN_OPEN:
		OpenFile(hDlg);
		break;
	case IDC_BTN_ORG:
			SetValueToDefault(hDlg);
		break;
	case IDC_BTN_SAVE:
		{
			if (g_pImg)
			{
				if (g_pImg->GetSize()>0)
				{
					TCHAR szFile[MAX_PATH] = {0};
					GetSaveFilePath(hDlg,szFile);

					if(!_tcsstr(szFile,_T(".bmp")))
						_tcscat(szFile,_T(".bmp"));
					g_pImg->Save(szFile);
				}
			}
		}
		break;
	case IDC_BTN_WINDOWS:
		{
			if (!g_pImg) return;
			if (g_pImg->GetSize()<=0)  return;
			bool bNeedChangeFlag = false;
			int nWidth = g_pImg->GetWidth();
			int nHeight = g_pImg->GetHeight();
			RECT rtWindows = {0};
			GetWindowRect(hDlg,&rtWindows);
			if (rtWindows.right - rtWindows.left < nWidth + 40)
			{
				bNeedChangeFlag = true;
			}
			if (rtWindows.bottom - rtWindows.top < nHeight)
			{
				bNeedChangeFlag = true;
			}
			if (bNeedChangeFlag)
			{
				MoveWindow(hDlg,rtWindows.left,rtWindows.top,nWidth+50,nHeight+130,TRUE);
			}
			else
			{
				MoveWindow(hDlg,rtWindows.left,rtWindows.top,508,409,TRUE);
			}
		}
		break;
	}
}

LRESULT CALLBACK Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			char szTitle[MAX_PATH+1] ={0};
			GetWindowTextA(hDlg,szTitle,MAX_PATH);
			strcat(szTitle," ±àÒëÊ±¼ä£º ");
			strcat(szTitle,__DATE__);
			strcat(szTitle," ");
			strcat(szTitle,__TIME__);
			SetWindowTextA(hDlg,szTitle);
			DragAcceptFiles(hDlg,true);
			SetClassLongPtr(hDlg, GCLP_HICON, (long)LoadIcon(0, IDI_APPLICATION));
			SetValueToDefault(hDlg);
		}

		break;
	case WM_COMMAND:
		OnTextChanged(wParam, hDlg);
		OnButtonClick(wParam, hDlg);
		break;
	case WM_DROPFILES:
		OnDropFiles(wParam, hDlg);
		break;
	case WM_CLOSE:
		EndDialog(hDlg, wParam);
		break;
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	InitCommonControls();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_LEVELSADJUSTMENT_DIALOG), NULL, (DLGPROC)Proc);
	return 0;
}

void SetValueToDefault(HWND hDlg)
{
	SetDlgItemInt(hDlg,IDC_INPUT_ROTATE,0,FALSE);
	SetDlgItemInt(hDlg,IDC_INPUT_COLOR,0,FALSE);
	SetDlgItemInt(hDlg,IDC_INTPUT_BLUR,0,FALSE);
	SetDlgItemInt(hDlg,IDC_OUTPUT_WHLTE,0,FALSE);
	if (g_pImg)
	{
		g_pImg->CopyBackDataToCurData();
		SetPicControlPic(hDlg);
	}
}

void ApplyFilterBlur(HWND hDlg)
{
	g_pImg->CopyBackDataToCurData();
	int nValue = GetDlgItemInt(hDlg,IDC_INTPUT_BLUR,FALSE,FALSE);
	CheckValue0255(nValue);
	char buf[32] = {0};
	sprintf(buf,"Blur is %d \n",nValue);
	OutputDebugStringA(buf);
	Ymagine_blurBuffer(g_pImg->GetBits(),g_pImg->GetWidth(),g_pImg->GetHeight(),g_pImg->GetPitch(),VBITMAP_COLOR_RGB,nValue);
	SetPicControlPic(hDlg);
	::InvalidateRect(hDlg,NULL,TRUE);
}

void ApplyFilterColorsize(HWND hDlg)
{
	g_pImg->CopyBackDataToCurData();
	int nValue = GetDlgItemInt(hDlg,IDC_INPUT_COLOR,FALSE,TRUE);
	//CheckValue0255(nValue);
	char buf[128] = {0};
	sprintf(buf,"Color is %d r=%d  g=%d  b=%d\n",nValue,(nValue >> 24) & 0xff,(nValue >> 16) & 0xff,(nValue >> 0) & 0xff);
	OutputDebugStringA(buf);
	Ymagine_colorrize(g_pImg->GetBits(),g_pImg->GetWidth(),g_pImg->GetHeight(),g_pImg->GetPitch(),3,nValue);
	SetPicControlPic(hDlg);
	::InvalidateRect(hDlg,NULL,TRUE);
}

void ApplyFilterRotate(HWND hDlg)
{
	g_pImg->CopyBackDataToCurData();
	int nValue = GetDlgItemInt(hDlg,IDC_INPUT_ROTATE,FALSE,TRUE);
	//CheckValue0255(nValue);
	char buf[32] = {0};
	sprintf(buf,"Rotate is %d \n",nValue);
	OutputDebugStringA(buf);
	int nHeight = g_pImg->GetHeight();
	int nWidth = g_pImg->GetWidth();
	int nPitch = g_pImg->GetPitch();
	BYTE *pBmpColorData = g_pImg->GetBits();
	Vbitmap  vBit = {0};
	vBit.bitmaptype = VBITMAP_MEMORY;
	vBit.height = nHeight;
	vBit.width = nWidth;
	vBit.pitch = nPitch;
	vBit.pixels = pBmpColorData;
	vBit.colormode = VBITMAP_COLOR_RGB;

	Vbitmap out ={0};
	out.width = nWidth;
	out.height = nHeight;
	out.pitch = _DibPitch24(out.width);
	out.colormode = VBITMAP_COLOR_RGB;
	out.bitmaptype = VBITMAP_MEMORY;
	int nSize = _DibSize24(out.width,out.height);
	out.pixels = (unsigned char*)_HeapAlloc(nSize);

	int nRet =Ymagine_rotate(&out,&vBit,nWidth/2,nHeight/2,nValue);
	if (nRet == YMAGINE_OK)
	{
		HBITMAP hBitmap = MakeBitmap(GetDC(hDlg),out.pixels,out.width,out.height,24);
		if (!hBitmap) return;

		HWND hPic = GetDlgItem(hDlg,IDC_PIC);
		HBITMAP dwRetHandle = (HBITMAP)SendMessage(hPic, STM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap)); 
		if (dwRetHandle)
		{
			if (dwRetHandle!=hBitmap)
				DeleteObject(dwRetHandle);
		}
		::InvalidateRect(hDlg,NULL,TRUE);
	}
	_HeapFree(out.pixels);
}

////////////////////////////////////////////////////////////////////////////////////////////////////