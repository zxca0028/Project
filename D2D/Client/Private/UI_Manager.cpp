#include "pch.h"
#include "UI_Manager.h"
#include<fstream>

BEGIN(Client)

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Load_Score()
{
	ifstream InputFile("Data.bin", ios::in | ios::binary);

	if (false == InputFile.is_open())
	{
		m_iHighScore = 0;

		return S_OK;
	}

	InputFile.read((char*)&m_iHighScore, sizeof(_uint));

	InputFile >> m_iHighScore;

	return S_OK;
}

HRESULT CUI_Manager::Save_Score()
{
	ofstream OutFile("Data.bin", ios::out | ios::binary);

	OutFile.write((char*)&m_iHighScore, sizeof(_uint));

	OutFile << m_iHighScore;

	return S_OK;
}

HRESULT CUI_Manager::Plus_Score()
{
	++m_iScore;

	if (m_iScore < 10)
	{
		CUI* pUI_Units = Find_UI(TEXT("GamePlay_Units"));

		if (nullptr == pUI_Units)
		{
			return E_FAIL;
		}

		pUI_Units->Set_Score(m_iScore);
	}
	else if (m_iScore < 100)
	{
		CUI* pUI_Tens  = Find_UI(TEXT("GamePlay_Tens"));
		CUI* pUI_Units = Find_UI(TEXT("GamePlay_Units"));

		if (nullptr == pUI_Tens)  { return E_FAIL; }
		if (nullptr == pUI_Units) { return E_FAIL; }

		_uint iNumber_Tens  = m_iScore / 10;
		_uint iNumber_Units = m_iScore % 10;

		pUI_Tens ->Set_Score(iNumber_Tens);
		pUI_Units->Set_Score(iNumber_Units);
	}
	else if (m_iScore < 1000)
	{
		CUI* pUI_Hundreds = Find_UI(TEXT("GamePlay_Hundreds"));
		CUI* pUI_Tens     = Find_UI(TEXT("GamePlay_Tens"));
		CUI* pUI_Units    = Find_UI(TEXT("GamePlay_Units"));

		if (nullptr == pUI_Hundreds) { return E_FAIL; }
		if (nullptr == pUI_Tens)     { return E_FAIL; }
		if (nullptr == pUI_Units)    { return E_FAIL; }

		_uint iNumber_Hundreds = m_iScore / 100;
		_uint iNumber_Tens     = (m_iScore / 10) % 10;
		_uint iNumber_Units    = m_iScore % 10;

		pUI_Hundreds->Set_Score(iNumber_Hundreds);
		pUI_Tens    ->Set_Score(iNumber_Tens);
		pUI_Units   ->Set_Score(iNumber_Units);
	}

	return S_OK;
}

HRESULT CUI_Manager::Set_Score()
{
	if (m_iHighScore < m_iScore)
	{
		m_iHighScore = m_iScore;
	}

	/* Score */
	if (m_iScore < 10)
	{
		CUI* pUI_Units = Find_UI(TEXT("Result_Score_Units"));

		if (nullptr == pUI_Units)
		{
			return E_FAIL;
		}

		pUI_Units->Set_Score(m_iScore);
	}
	else if (m_iScore < 100)
	{
		CUI* pUI_Tens  = Find_UI(TEXT("Result_Score_Tens"));
		CUI* pUI_Units = Find_UI(TEXT("Result_Score_Units"));

		if (nullptr == pUI_Tens)  { return E_FAIL; }
		if (nullptr == pUI_Units) { return E_FAIL; }

		_uint iNumber_Tens  = m_iScore / 10;
		_uint iNumber_Units = m_iScore % 10;

		pUI_Tens ->Set_Score(iNumber_Tens);
		pUI_Units->Set_Score(iNumber_Units);
	}
	else if (m_iScore < 1000)
	{									  
		CUI* pUI_Hundreds = Find_UI(TEXT("Result_Score_Hundreds"));
		CUI* pUI_Tens     = Find_UI(TEXT("Result_Score_Tens"));
		CUI* pUI_Units    = Find_UI(TEXT("Result_Score_Units"));

		if (nullptr == pUI_Hundreds) { return E_FAIL; }
		if (nullptr == pUI_Tens)     { return E_FAIL; }
		if (nullptr == pUI_Units)    { return E_FAIL; }

		_uint iNumber_Hundreds = m_iScore / 100;
		_uint iNumber_Tens     = (m_iScore / 10) % 10;
		_uint iNumber_Units    = m_iScore % 10;

		pUI_Hundreds->Set_Score(iNumber_Hundreds);
		pUI_Tens    ->Set_Score(iNumber_Tens);
		pUI_Units   ->Set_Score(iNumber_Units);
	}

	/* HighScore */
	if (m_iHighScore < 10)
	{
		CUI* pUI_Units = Find_UI(TEXT("Result_HighScore_Units"));

		if (nullptr == pUI_Units)
		{
			return E_FAIL;
		}

		pUI_Units->Set_Score(m_iHighScore);
	}
	else if (m_iHighScore < 100)
	{
		CUI* pUI_Tens  = Find_UI(TEXT("Result_HighScore_Tens"));
		CUI* pUI_Units = Find_UI(TEXT("Result_HighScore_Units"));

		if (nullptr == pUI_Tens)  { return E_FAIL; }
		if (nullptr == pUI_Units) { return E_FAIL; }

		_uint iNumber_Tens  = m_iHighScore / 10;
		_uint iNumber_Units = m_iHighScore % 10;

		pUI_Tens ->Set_Score(iNumber_Tens);
		pUI_Units->Set_Score(iNumber_Units);
	}
	else if (m_iHighScore < 1000)
	{
		CUI* pUI_Hundreds = Find_UI(TEXT("Result_HighScore_Hundreds"));
		CUI* pUI_Tens     = Find_UI(TEXT("Result_HighScore_Tens"));
		CUI* pUI_Units    = Find_UI(TEXT("Result_HighScore_Units"));

		if (nullptr == pUI_Hundreds) { return E_FAIL; }
		if (nullptr == pUI_Tens)     { return E_FAIL; }
		if (nullptr == pUI_Units)    { return E_FAIL; }

		_uint iNumber_Hundreds = m_iHighScore / 100;
		_uint iNumber_Tens     = (m_iHighScore / 10) % 10;
		_uint iNumber_Units    = m_iHighScore % 10;

		pUI_Hundreds->Set_Score(iNumber_Hundreds);
		pUI_Tens    ->Set_Score(iNumber_Tens);
		pUI_Units   ->Set_Score(iNumber_Units);
	}

	return S_OK;
}

HRESULT CUI_Manager::Add_UI(wstring UITag, CUI* pUI)
{
	m_UIs.emplace(UITag, pUI);

	Safe_AddRef(pUI);

	return S_OK;
}

void CUI_Manager::Clear_UI()
{
	for (auto& Pair : m_UIs)
	{
		Safe_Release(Pair.second);
	}

	m_UIs.clear();

	m_iScore = 0;
}

CUI* CUI_Manager::Find_UI(wstring UITag)
{
	auto iter = m_UIs.find(UITag);

	if (iter == m_UIs.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CUI_Manager::Free()
{
	Save_Score();

	Clear_UI();
}

END