#pragma once

#include "Input.h"
#include "PipeLine.h"
#include "Time_Manager.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Target_Manager.h"
#include "Component_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:

	explicit CGameInstance();
	virtual ~CGameInstance() = default;

/////////////////////////////////////////////////////////////////////////
#pragma region PUBLIC

public:

	HRESULT     Initialize_Engine(_uint iNumLevel, HINSTANCE hInst, HWND hWnd);
	_int        Tick (_double DeltaTime);
	_int		Late_Tick(_double DeltaTime);
	void		Clear(_uint iLevelIndex);
	static void Release_Engine();

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region GRAPHIC_DEVICE

public:

	HRESULT Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);
	void	Render_Begin();
	void	Render_End(HWND hWND = nullptr);

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region LEVEL_MANAGER

public:

	HRESULT Set_Level(class CLevel* pLevel);
	HRESULT Render_Level();

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region OBJECT_MANAGER

public:

	HRESULT      Add_GameObject_Prototype(const _tchar * pPrototypeTag, class CGameObject* pPrototype);
	HRESULT      Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg = nullptr);
	HRESULT      Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, class CGameObject* pGameObject);
	CComponent*  Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex = 0);
	CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex = 0);
	HRESULT		 Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex);
	HRESULT		 Delete_GameObject();
	HRESULT		 Add_DeleteContainer(_uint iLevelIndex, const _tchar * pLayerTag, class CGameObject* pDeleteGameObject);
	_uint		 Get_LayerSize(_uint iLevelIndex, const _tchar * pLayerTag);

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region COMPONENT_MANAGER

public:

	HRESULT		Add_Component_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void* pArg = nullptr);

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region TIME_MANAGER

	HRESULT Add_Timer(string pTimerTag);//const _tchar * pTimerTag);
	_float  Compute_DeltaTime(string pTimerTag);//const _tchar * pTimerTag);

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region PIPELINE

	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eStateType);
	_float3 Get_CamPosition();

#pragma endregion
/////////////////////////////////////////////////////////////////////////
#pragma region INPUT

	_byte	Get_KeyState       (_byte bKey);
	_byte	Get_MouseClickState(CInput::MOUSE_CLICK eClick);
	_long	Get_MouseMoveState (CInput::MOUSE_MOVE eMove);

#pragma endregion
	/////////////////////////////////////////////////////////////////////////
#pragma region LIGHT_MANAGER

public:

	D3DLIGHT9 Get_LightDesc(_uint iIndex = 0);
	HRESULT   Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightDesc);

#pragma endregion

private:

	CInput*				m_pInput			 = nullptr;
	CPipeLine*			m_pPipeLine			 = nullptr;
	CTime_Manager*		m_pTime_Manager		 = nullptr;
	CLight_Manager*     m_pLight_Manager     = nullptr;
	CLevel_Manager*		m_pLevel_Manager	 = nullptr;
	CObject_Manager*	m_pObject_Manager	 = nullptr;
	CGraphic_Device*	m_pGraphic_Device	 = nullptr;
	CComponent_Manager* m_pComponent_Manager = nullptr;

public:

	virtual void Free() override;

};

END