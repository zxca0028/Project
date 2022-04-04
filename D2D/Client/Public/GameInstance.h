#pragma once

#include "Random.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "key_Manager.h"
#include "UI_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "GameObject_Manager.h"

BEGIN(Client)

class CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:

	explicit CGameInstance();
	virtual ~CGameInstance() = default;

#pragma region GAMEINSTANCE
public:

	HRESULT     NativeConstruct(LPDIRECT3DDEVICE9* ppGraphic_Device);
	_int        Tick	 (_double DeltaTime);
	_int        Late_Tick(_double DeltaTime);
	HRESULT     Render_Begin();
	HRESULT		Render_End();
	static void Release_Instances();

#pragma endregion

#pragma region RENDERER
public:

	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eGroup, class CGameObject* pGameObject);

#ifdef _DEBUG

	HRESULT Add_RenderComponent(class CComponent* pComponent);

#endif

#pragma endregion

#pragma region KEY_MANAGER
public:

	_bool Key_Up      (DWORD dwKey);
	_bool Key_Down    (DWORD dwKey);
	_bool Key_Pressing(DWORD dwKey);

#pragma endregion

#pragma region LEVEL_MANAGER
public:

	HRESULT Set_Level(class CLevel* pLevel);

#pragma endregion

#pragma region TIMER_MANAGER
public:

	HRESULT Add_Timer(wstring pTimerTag);
	_double Compute_DeltaTime(wstring pTimerTag);

#pragma endregion

#pragma region GAMEOBJECT_MANAGER
public:

	HRESULT Add_GameObject(wstring LayerTag, class CGameObject* pGameObject);
	HRESULT Add_DeleteQueue(wstring LayerTag, class CGameObject* pGameObject);
	HRESULT Delete_GameObject();
	void    OnNotify(_bool bIsGameOver);
	void    OnNotify(LEVEL eLevel);

public:

	class CComponent* Get_ComponentPtr(wstring LayerTag, wstring ComponentTag, _uint iIndex = 0);

#pragma endregion

#pragma region PIPELINE
public:

	HRESULT Set_Transform(_matrix TransformMatrix, D3DTRANSFORMSTATETYPE eType);
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eType);

#pragma endregion

#pragma region RANDOM
public:

	_float Get_Float(_float fMin, _float fMax);

#pragma endregion

#pragma region SCORE_MANAGER
public:

	HRESULT Add_UI(wstring UITag, class CUI* pUI);
	HRESULT Plus_Score();
	HRESULT Set_Score();
	void    Clear_UI();

#pragma endregion

private:

	CRandom*			 m_pRandom			   = nullptr;
	CPipeline*			 m_pPipeline		   = nullptr;
	CRenderer*			 m_pRenderer		   = nullptr;
	CKey_Manager*        m_pKey_Manager        = nullptr;
	CLevel_Manager*      m_pLevel_Manager      = nullptr;
	CTimer_Manager*      m_pTimer_Manager      = nullptr;
	CUI_Manager*		 m_pUI_Manager	   = nullptr;
	CGraphic_Device*     m_pGraphic_Device     = nullptr;
	CGameObject_Manager* m_pGameObject_Manager = nullptr;

public:

	virtual void Free() override;
};

END