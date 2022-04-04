#pragma once

class CMyBitmap;

class CBitmapManager
{
private:

	 CBitmapManager();
	~CBitmapManager();

public:

	static CBitmapManager* Get_Instance();
	static void Destroy_Instance();

public:

	HDC	Find_Image(const TCHAR* pImageKey);
	void Insert_Bitmap(const TCHAR* pPath, const TCHAR* pImageKey);
	void Load_Bitmap();
	void Release_BitMap();

private:

	static CBitmapManager* m_pInstance;

	map<const TCHAR*, CMyBitmap*> m_mapBmp;
};

