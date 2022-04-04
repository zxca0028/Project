#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:

	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:

	
};

END