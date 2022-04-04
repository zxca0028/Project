#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Frustum.h"
#include "Picking.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "VIBuffer_Rect.h"
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

	HRESULT		Reserve_Container(_uint iNumLevels);
	HRESULT		Add_Component_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	void		Clear(_uint iLevelIndex);

private:

	typedef map<const _tchar*, CComponent*> PROTOTYPES;
	PROTOTYPES* m_pPrototypes = nullptr;
	
private:

	_uint m_iNumLevels = 0;

private:

	CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:

	virtual void Free() override;

};

END