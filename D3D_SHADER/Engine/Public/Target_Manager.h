#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager);

private:

	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:

	TARGETDESC Get_TargetDesc(wstring pTargetTag);

public:

	HRESULT Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, wstring pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(wstring pMRTTag, wstring pTargetTag);

	HRESULT Begin_MRT(wstring pMRTTag);
	HRESULT End_MRT(wstring pMRTTag);

private:

	typedef unordered_map<wstring, class CTarget*> RENDERTARGETS;
	RENDERTARGETS m_RenderTargets;

	typedef unordered_map<wstring, vector<class CTarget*>> MRTS;
	MRTS m_MRTs;

private:

	class CTarget* Find_RenderTarget(wstring pTargetTag);
	vector<class CTarget*>* Find_MRT(wstring pMRTTag);

public:

	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);

public:

	virtual void Free() override;

};

END