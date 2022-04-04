#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CLight;
END

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:

	explicit CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:

	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double DeltaTime)override;
	virtual _int    Late_Tick(_double DeltaTime)override;
	virtual HRESULT Render()override;

private:

	HRESULT Ready_Light();
	HRESULT Ready_Layer_SkyBox		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera	       (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI			   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Weapon		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Test		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Structure	   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC		       (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Icon		   (const _tchar* pLayerTag);
	HRESULT Ready_Layer_CheckPoint     (const _tchar* pLayerTag);
	HRESULT Ready_Layer_Skeleton	   (const _tchar* pLayerTag);

private:

	CLight* m_pLight = nullptr;
	CLight* m_pLight2 = nullptr;

public:

	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free() override;

};

END