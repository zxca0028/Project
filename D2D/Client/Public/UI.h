#pragma once

#include "GameObject.h"

BEGIN(Client)

class CUI final : public CGameObject
{
public:

	enum TYPE  { TYPE_STATIC,    TYPE_DYNAMIC, TYPE_END  };
	enum USAGE { USAGE_GAMEPLAY, USAGE_RESULT, USAGE_END };

	typedef struct tagUIDesc
	{
		_float3 vPos;
		_float3 vScale = _float3(50.f, 50.f, 0.f);

		TYPE  eType;
		USAGE eUsage;

	}UIDESC;

private:

	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CUI() = default;

public:

	virtual HRESULT NativeConstruct(void* pArg)	 override;
	virtual _int    Tick	 (_double DeltaTime) override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	virtual HRESULT Add_Components(void* pArg = nullptr) override;
	virtual HRESULT Set_ConstTable()					 override;

public:

	void Set_Score(_uint iScore) { m_iScore = iScore; }

private:

	class CShader*        m_pShader    = nullptr;
	class CTexture*       m_pTexture   = nullptr;
	class CTransform*     m_pTransform = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer  = nullptr;

private:

	UIDESC m_UIDesc;

private:

	_uint m_iScore = 0;

public:

	static  CUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual void Free() override;

};

END