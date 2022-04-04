#pragma once

#include "Shader.h"
#include "Picking.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:

	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:

	HRESULT	    NativeConstruct(_uint iNumLevel);
	HRESULT     Add_Component_Prototype(_uint iLevelIndex, wstring pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, wstring pPrototypeTag, void* pArg = nullptr);
	void        Clear(_uint iLevelIndex);

private:

	typedef unordered_map<wstring, CComponent*> PROTOTYPES;
	PROTOTYPES* m_pPrototypes = nullptr;

private:

	_uint m_iNumLevel = 0;

private:

	CComponent* Find_Component(_uint iLevelIndex, wstring pPrototypeTag);

public:

	virtual void Free() override;

};

END