#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager);

private:

	explicit CUI_Manager();
	virtual ~CUI_Manager() = default;

public:

	HRESULT Add_UI(wstring UITag, class CUI* pUI);
	void    Clear_UI();

private:

	class CUI* Find_UI(wstring UITag);

public:

	HRESULT Load_Score();
	HRESULT Save_Score();
	HRESULT Plus_Score();
	HRESULT Set_Score();

public:

	_uint Get_Score() { return m_iScore; }

private:

	_uint m_iScore     = 0;
	_uint m_iHighScore = 0;
	_uint m_iTempScore = 0;
	
private:

	unordered_map<wstring, class CUI*> m_UIs;

public:

	virtual void Free() override;
	
};

END