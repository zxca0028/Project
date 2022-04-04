#pragma once

#include "PipeLine.h"
#include "key_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Light_Manager.h"
#include "Graphic_Device.h"
#include "Target_Manager.h"
#include "Component_Manager.h"
#include "GameObject_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:

	explicit CGameInstance();
	virtual ~CGameInstance() = default;

#pragma region GAMEINSTANCE
public:

	HRESULT     NativeConstruct(_uint iNumLevel, HINSTANCE hInst, HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device);
	_int        Tick     (_double DeltaTime);
	_int        Late_Tick(_double DeltaTime);
	void		Clear(_uint iLevelIndex);
	static void Release_Engine();
#pragma endregion

#pragma region GRAPHIC_DEVICE
public:

	HRESULT NativeConstruct(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9 * ppGraphic_Device);
	void    Render_Begin();
	void    Render_End(HWND hWnd = nullptr);
#pragma endregion

#pragma region TIMER
public:

	HRESULT Add_Timer		 (wstring pTimerTag);
	_double Compute_DeltaTime(wstring pTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:

	HRESULT Set_Level(class CLevel* pLevel);
	HRESULT Render_Level();
#pragma endregion

#pragma region COMPONENT_MANAGER
public:

	HRESULT Add_Component_Prototype(_uint iLevelIndex, wstring pPrototypeTag, CComponent* pPrototype);
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
public:

	HRESULT     Add_GameObject_Prototype(wstring pPrototypeTag, class CGameObject* pPrototype);
	HRESULT     Add_GameObject(_uint iLevelIndex, wstring pPrototypeTag, wstring pLayerTag, void* pArg = nullptr);
	CComponent* Get_ComponentPtr(_uint iLevelIndex, wstring pLayerTag, wstring pComponentTag, _uint iIndex = 0);
#pragma endregion

#pragma region PIPELINE

	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eStateType);
	_float3 Get_CamPos();

#pragma endregion

#pragma region KEY_MANAGER

	_bool Key_Up      (DWORD dwKey);
	_bool Key_Down    (DWORD dwKey);
	_bool Key_Pressing(DWORD dwKey);

	_long Mouse_Move(CKey_Manager::MOUSE eMouseState);

#pragma endregion

#pragma region LIGHT_MANAGER

public:

	HRESULT Add_Right(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9_DERIVED& LightDesc);
	_matrix Get_LightTransform(D3DTRANSFORMSTATETYPE eStateType);

#pragma endregion

#pragma region TARGET_MANAGER

public:

	TARGETDESC Get_TargetDesc(wstring pTargetTag);

#pragma endregion

private:

	CPipeLine*			 m_pPipeLine		   = nullptr;
	CKey_Manager*		 m_pKey_Manager		   = nullptr;
	CLevel_Manager*      m_pLevel_Manager      = nullptr;
	CTimer_Manager*      m_pTimer_Manager      = nullptr;
	CLight_Manager*      m_pLight_Manager      = nullptr;
	CGraphic_Device*     m_pGraphic_Device     = nullptr;
	CTarget_Manager*	 m_pTarget_Manager     = nullptr;
	CComponent_Manager*  m_pComponent_Manager  = nullptr;
	CGameObject_Manager* m_pGameObject_Manager = nullptr;

public:

	virtual void Free() override;
};

END