#pragma once

class CMyBitmap
{
public:

	 CMyBitmap();
	~CMyBitmap();

public:

	HDC	Get_MemDC() { return m_hMemDC; }
	void Insert_Bitmap(const TCHAR* pFilePath);
	void Release_Bitmap();

private:

	HDC	m_hMemDC;

	HBITMAP	m_hBitmap;
	HBITMAP m_hOldBitmap;
};

