#include "Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
    : m_pGraphic_Device(pGraphic_Device)
    , m_pAnimationController(pAnimationController)
{
    Safe_AddRef(m_pGraphic_Device);
}

CAnimation::CAnimation(const CAnimation& rhs)
    : m_pGraphic_Device(rhs.m_pGraphic_Device)
{
    rhs.m_pAnimationController->CloneAnimationController
    (
        rhs.m_pAnimationController->GetMaxNumAnimationOutputs(),
        rhs.m_pAnimationController->GetMaxNumAnimationSets(),
        rhs.m_pAnimationController->GetMaxNumTracks(),
        rhs.m_pAnimationController->GetMaxNumEvents(),
        &m_pAnimationController
    );

    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CAnimation::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CAnimation::NativeConstruct()
{
    return S_OK;
}

HRESULT CAnimation::Set_AnimationIndex(_uint iAnimationIndex, _float fAnimationSpeed)
{
    if (m_iCurrentAnimationIndex == iAnimationIndex)
    {
        return S_OK;
    }

    if (m_TimeAcc < 0.2f)
    {
        return S_OK;
    }

    LPD3DXANIMATIONSET pAS = nullptr;

    if (FAILED(m_pAnimationController->GetAnimationSet(iAnimationIndex, &pAS)))
    {
        return E_FAIL;
    }

    m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

    m_pAnimationController->SetTrackAnimationSet(m_iNewTrack, pAS);

    Safe_Release(pAS);

    D3DXTRACK_DESC TrackDesc;

    m_pAnimationController->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

    m_pAnimationController->UnkeyAllTrackEvents(m_iCurrentTrack);
    m_pAnimationController->UnkeyAllTrackEvents(m_iNewTrack);

    m_pAnimationController->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + 0.2);
    m_pAnimationController->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
    m_pAnimationController->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);

    m_pAnimationController->SetTrackEnable(m_iNewTrack, TRUE);
    m_pAnimationController->KeyTrackWeight(m_iNewTrack, 0.9f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
    m_pAnimationController->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);

    m_pAnimationController->SetTrackPosition(m_iNewTrack, 0.0);
    m_pAnimationController->SetTrackSpeed(m_iNewTrack, fAnimationSpeed);

    m_pAnimationController->ResetTime();
    m_TimeAcc = 0.0;

    m_iCurrentTrack = m_iNewTrack;

    m_iCurrentAnimationIndex = iAnimationIndex;

    return S_OK;
}

HRESULT CAnimation::Play_Animation(_double DeltaTime)
{
    if (nullptr == m_pAnimationController)
    {
        return E_FAIL;
    }

    m_TimeAcc += DeltaTime;
    m_pAnimationController->AdvanceTime(DeltaTime, nullptr);

    return S_OK;
}

HRESULT CAnimation::Bind_Frames(D3DXFRAME_DERIVED* pRootFrame)
{
    if (FAILED(D3DXFrameRegisterNamedMatrices(pRootFrame, m_pAnimationController)))
    {
        return E_FAIL;
    }

    return S_OK;
}

_double CAnimation::Get_Period(_uint iAnimationIndex)
{
    LPD3DXANIMATIONSET pAS = nullptr;

    if (FAILED(m_pAnimationController->GetAnimationSet(iAnimationIndex, &pAS)))
    {
        return E_FAIL;
    }

    m_Period = pAS->GetPeriod();

    Safe_Release(pAS);

    return m_Period;
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
{
    CAnimation* pInstance = new CAnimation(pGraphic_Device, pAnimationController);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating Animation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CAnimation* CAnimation::Clone()
{
    CAnimation* pInstance = new CAnimation(*this);

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Creating Animation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimation::Free()
{
    Safe_Release(m_pAnimationController);
    Safe_Release(m_pGraphic_Device);
}
