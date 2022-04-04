#pragma once

#include "GameObject.h"

BEGIN(Client)

class CBackGround final : public CGameObject
{
public:

	typedef struct tagBGDesc
	{
		_float fInitPosX;
		_float fSpeed;

	}BGDESC;

private:

	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBackGround() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick     (_double DeltaTime) override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()				     override;

private:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	class CShader*		  m_pShader    = nullptr;
	class CTexture*		  m_pTexture   = nullptr;
	class CTransform*	  m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	BGDESC m_BGDesc;
	_bool  m_bCreateLock = false;

public:

	static  CBackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END