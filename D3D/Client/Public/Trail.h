#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
END

BEGIN(Client)

class CTrail final : public CGameObject
{
public:

	struct INDEX16
	{
		unsigned short _0;
		unsigned short _1;
		unsigned short _2;
	};

	typedef struct tagTrailInfo
	{
		const _matrix* pWorldMatrix = nullptr;
		_uint iIndex;

	}TRAILINFO;

private:

	struct TrailData
	{
		_float3 Position[2];
		double TimeCount = 0.0;

		TrailData(const _float3& UpPosition, const _float3& DownPosition)
			: TimeCount(0.0)
		{
			Position[0] = UpPosition;
			Position[1] = DownPosition;
		}
	};

private:

	explicit CTrail(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTrail() = default;

public:

	virtual HRESULT NativeConstruct_Prototype()  override;
	virtual HRESULT NativeConstruct(void* pArg)  override;
	virtual _int    Tick(_double DeltaTime)		 override;
	virtual _int    Late_Tick(_double DeltaTime) override;
	virtual HRESULT Render()					 override;

public:

	void    AddNewTrail(const _float3& UpPosition, const _float3& DownPosition, _double DeltaTime);
	void	UpdateTrail(_double DeltaTime);

private:

	void    SplineTrailPosition(VTXTEX* Vertex, const size_t& DataIndex, _ulong& Index, const _matrix* WorldMatrix);


private:

	TRAILINFO	m_TrailInfo;

	_matrix* m_pObjectWorldMatrix = nullptr;

	CShader*   m_pShader   = nullptr;
	CTexture*  m_pTexture  = nullptr;
	CRenderer* m_pRenderer = nullptr;

private:

	HRESULT Add_Component(void* pArg = nullptr);

private:

	DWORD   m_dwFVF;
	_ulong  VTXSize    = 0;
	_ulong  MaxVTXCnt  = 0;
	_ulong  MaxTriCnt  = 0;
	_ulong  CurTriCnt  = 0;
	_ulong  CurVTXCnt  = 0;
	_double m_Duration = 0.0;
	_double AliveTime  = 0.0;
	size_t  LerpCnt    = 0;
	_float  m_fTimeAcc = 0.0f;
	_float  UVRate     = 0.0f;
	vector<TrailData> m_TrailDatas;

	_int m_iIndex = 0;

public:

	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer   = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer    = nullptr;

public:

	static  CTrail* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free() override;

};

END