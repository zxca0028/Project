#include "pch.h"
#include "MyBitmap.h"

CMyBitmap::CMyBitmap()
{}

CMyBitmap::~CMyBitmap()
{
	Release_Bitmap();
}

void CMyBitmap::Insert_Bitmap(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage
	(	
		NULL, 
		pFilePath, 
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBitmap::Release_Bitmap()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
