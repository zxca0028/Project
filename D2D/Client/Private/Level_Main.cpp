#include "pch.h"
#include "Level_Main.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "BackGround.h"
#include "Player.h"
#include "Grass.h"
#include "Box.h"
#include "Title.h"
#include "Transform.h"
#include "Point.h"
#include "Sound_Manager.h"

BEGIN(Client)

CLevel_Main::CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Main::NativeConstruct(LEVEL eLevel)
{
    m_eLevelIndex = eLevel;

    if (FAILED(Ready_Layer_Camera()))
    {
        return E_FAIL;
    }
    if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
    {
        return E_FAIL;
    }
    if (FAILED(Ready_Layer_Title(TEXT("Layer_Title"))))
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->OnNotify(false);
    pGameInstance->OnNotify(eLevel);
    pGameInstance->Clear_UI();

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

_int CLevel_Main::Tick(_double DeltaTime)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (pGameInstance->Key_Down(KEY_SPACE))
    {
        CSound_Manager::Get_Instance()->PlaySound(TEXT("Open.mp3"), CSound_Manager::CHANNELID::INPUT);

        if (FAILED(pGameInstance->Set_Level(CLevel_GamePlay::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
        {
            RELEASE_INSTANCE(CGameInstance);

            return E_FAIL;
        }
    }

    RELEASE_INSTANCE(CGameInstance);

    return _int();
}

_int CLevel_Main::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CLevel_Main::Render()
{
    _tchar szText[MAX_PATH] = TEXT("MOKOKO");

    SetWindowText(g_hWnd, szText);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Camera()
{
    _matrix ViewMatrix;
    _matrix ProjMatrix;

    D3DXMatrixIdentity(&ViewMatrix);
    D3DXMatrixOrthoOffCenterLH(&ProjMatrix, 0, (FLOAT)g_iWinCX, 0.f, (FLOAT)g_iWinCY, 0.f, 1.f);

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    pGameInstance->Set_Transform(ViewMatrix, D3DTS_VIEW); 
    pGameInstance->Set_Transform(ProjMatrix, D3DTS_PROJECTION);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Player(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, CPlayer::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Title(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, CTitle::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_Point(wstring LayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(pGameInstance->Add_GameObject(LayerTag, CPoint::Create(m_pGraphic_Device))))
    {
        return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_Main* CLevel_Main::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
    CLevel_Main* pInstance = new CLevel_Main(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(eLevel)))
    {
        MSGBOX("Failed to Create Level_Main");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Main::Free()
{
    __super::Free();
}

END