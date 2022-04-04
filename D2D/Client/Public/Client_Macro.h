#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW		

#endif

#endif

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("Error"), MB_OK)

#define BEGIN(NAME) namespace NAME {
#define END }

#define GET_INSTANCE(CLASSNAME) []()				   \
{													   \
	CLASSNAME* pInstance = CLASSNAME::Get_Instance();  \
													   \
	if (nullptr != pInstance)						   \
	{												   \
		pInstance->AddRef();						   \
	}												   \
													   \
	return pInstance;								   \
}();

#define RELEASE_INSTANCE(CLASSNAME) []()			   \
{													   \
	CLASSNAME* pInstance = CLASSNAME::Get_Instance();  \
													   \
	if (nullptr != pInstance)						   \
	{												   \
		pInstance->Release();						   \
	}												   \
}();

#define DECLARE_SINGLETON(Class)					   \
public:												   \
													   \
	static Class* Get_Instance()					   \
	{												   \
		if (nullptr == m_pInstance)					   \
		{											   \
			m_pInstance = new Class;				   \
		}											   \
													   \
		return m_pInstance;							   \
	}												   \
													   \
	static unsigned long Destroy_Instance()			   \
	{												   \
		unsigned long dwRefCnt = 0;					   \
													   \
		dwRefCnt = Safe_Release(m_pInstance);		   \
													   \
		return dwRefCnt;							   \
	}												   \
													   \
private:											   \
													   \
		static Class* m_pInstance;

#define IMPLEMENT_SINGLETON(Class)					   \
Class* Class::m_pInstance = nullptr;

#define FPS(FPS) 1.f / FPS