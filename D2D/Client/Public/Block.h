#pragma once

#include "GameObject.h"

BEGIN(Client)

class CBlock final : public CGameObject
{
public:

	enum POSITION { POSITION_TOP, POSITION_GND, POSITION_END };

public:

	typedef struct tagBlockDesc
	{
		_float3 vInitPos;
		_float3 vScale;

		_float fDeletePos;
		_float fSpeed;

		POSITION eType;

	}BLOCKDESC;

private:

	explicit CBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBlock() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()				     override;

private:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable() override;

private:

	class CShader*		  m_pShader    = nullptr;
	class CTexture*		  m_pTexture   = nullptr;
	class CCollider*	  m_pCollider  = nullptr;
	class CTransform*	  m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:
	
	BLOCKDESC m_BlockDesc;

public:

	static  CBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END