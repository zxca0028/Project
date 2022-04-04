#include "pch.h"
#include "ObjectTool.h"
#include "GameInstance.h"
#include "Test.h"
#include "Transform.h"
#include "Mesh_Static.h"
#include "KeyManager.h"
#include<fstream>

bool    m_CollisionView = true;
bool    m_NaviView      = false;
bool    m_PlayerControl = true;
bool    m_CameraChange  = false;
bool    m_bTargetRender = false;
float X = 0.f;
float Y = 0.f;
float Z = 0.f;

CObjectTool::CObjectTool(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{}

HRESULT CObjectTool::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CObjectTool::NativeConstruct(void* pArg)
{
    if (FAILED(D3DXCreateSphere(m_pGraphic_Device, 0.1f, 20, 20, &m_pMesh, nullptr)))
    {
        return E_FAIL;
    }

    CTransform::TRANSFORMDESC		TransformDesc;
    TransformDesc.fSpeedPerSec = 3.0f;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc)))
    {
        return E_FAIL;
    }

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Picking"), TEXT("Com_Picking"), (CComponent**)&m_pPicking)))
    {
        return E_FAIL;
    }

    m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Navi.hlsl"));

    if (nullptr == m_pShader)
    {
        return E_FAIL;
    }

    m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 10.f, 0.f));

    return S_OK;
}

_int CObjectTool::Tick(_double DeltaTime)
{

    ++m_dwFrameCounter;
    m_fTimeAcc += DeltaTime;

    if (m_fTimeAcc > 1.f)
    {
        m_fTimeAcc = 0.f;
        m_dwFrame = m_dwFrameCounter;
        m_dwFrameCounter = 0;
    }

    ImGuiTabBarFlags TabFlags = ImGuiTabBarFlags_None;

    ImGui::Begin("Setting");
    ImGui::BulletText("FPS : %d", m_dwFrame);
    ImGui::Separator();
    if (ImGui::BeginTabBar("Tab", TabFlags))
    {
#pragma region Tab_System
        if (ImGui::BeginTabItem("System"))
        {
            ImGui::BulletText("Collider");
            if (m_CollisionView == true) { ImGui::Text("Activate : On"); }
            else { ImGui::Text("Activate : Off"); }
            ImGui::SameLine();
            if (ImGui::Button("On##1")) { m_CollisionView = true; } ImGui::SameLine();
            if (ImGui::Button("Off##1")) { m_CollisionView = false; }
            ImGui::Separator();

            ImGui::BulletText("Navigation");
            if (m_NaviView == true) { ImGui::Text("Activate : On"); }
            else { ImGui::Text("Activate : Off"); }
            ImGui::SameLine();
            if (ImGui::Button("On##2")) { m_NaviView = true; } ImGui::SameLine();
            if (ImGui::Button("Off##2")) { m_NaviView = false; }
            ImGui::Separator();

            ImGui::BulletText("Player");
            if (m_PlayerControl == true) { ImGui::Text("Activate : On"); }
            else { ImGui::Text("Activate : Off"); }
            ImGui::SameLine();
            if (ImGui::Button("On##3")) { m_PlayerControl = true; } ImGui::SameLine();
            if (ImGui::Button("Off##3")) { m_PlayerControl = false; }
            ImGui::Separator();

            ImGui::BulletText("Camera");
            if (m_CameraChange == true) { ImGui::Text("Activate : Free"); }
            else { ImGui::Text("Activate : QV"); }
            ImGui::SameLine();
            if (ImGui::Button("Free")) { m_CameraChange = true; } ImGui::SameLine();
            if (ImGui::Button("QV")) { m_CameraChange = false; }
            ImGui::Separator();

            ImGui::BulletText("TargetRender");
            if (m_bTargetRender == true) { ImGui::Text("Activate : On"); }
            else { ImGui::Text("Activate : Off"); }
            ImGui::SameLine();
            if (ImGui::Button("On##4")) { m_bTargetRender = true; } ImGui::SameLine();
            if (ImGui::Button("Off##4")) { m_bTargetRender = false; }
            ImGui::EndTabItem();
        }
#pragma endregion

#pragma region Tap_MapTool
        if (ImGui::BeginTabItem("Map"))
        {
            ImGui::InputInt("Object Index", &m_iKey);
            if (ImGui::Button("Create Object"))
            {
                m_Index.push_back(m_iKey);
                Create_Object();
            }
            ImGui::Separator();
            ImGui::DragFloat3("Position", vPos, 0.01f, -1000.f, 1000.f, "%.2f");
            ImGui::DragFloat("Scale", &fScale, 0.001f, 0.001f, 1.f);

            if (m_ObjectTransform.size() != 0)
            {
                _float3 vZ = _float3(0.f, 0.f, 1.f);
                _float3 vLook = m_ObjectTransform[m_iObjectIndex]->Get_State(CTransform::STATE_LOOK);
                vLook.y = 0.f;
                D3DXVec3Normalize(&vLook, &vLook);
                fAngle = acosf(D3DXVec3Dot(&vLook, &vZ)) * (180.f / D3DX_PI);
            }
            ImGui::Text("Angle : %.2f", fAngle); ImGui::SameLine();
            ImGui::Text("(Z,C Key)"); ImGui::SameLine();
            if (ImGui::Button("FineTune"))
            {
                if (m_bSlow == true) { m_bSlow = false; }
                else { m_bSlow = true; }
            }
            if (m_bSlow == true) { ImGui::SameLine(); ImGui::Text("On"); }
            else { ImGui::SameLine(); ImGui::Text("Off"); }

            ImGui::InputInt("Index", &m_iTemp);
            if (ImGui::Button("Get_Transform"))
            {
                m_iObjectIndex = m_iTemp - 1;

                vPos = m_ObjectTransform[m_iObjectIndex]->Get_State(CTransform::STATE_POSITION);
                fScale = m_ObjectTransform[m_iObjectIndex]->Get_Scale().x;
            }
            ImGui::Separator();
            ImGui::BulletText("Created Objects : %d", m_ObjectTransform.size());
            ImGui::Separator();
            if (ImGui::Button("Save"))
            {
                Save_Data();
            }
            ImGui::EndTabItem();
        }
#pragma endregion

#pragma region Tap_NavigationTool
        if (ImGui::BeginTabItem("Navigation"))
        {
            vNaviPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
            ImGui::Text("X : %.2f", vNaviPos.x);
            ImGui::Text("Y : %.2f", vNaviPos.y);
            ImGui::Text("Z : %.2f", vNaviPos.z);
            Set_Navigation(DeltaTime);
            
            CKeyManager* pKey = GET_INSTANCE(CKeyManager);

            if (pKey->Key_Down(KEY_SPACE))
            {
                m_NaviPoints.push_back(vNaviPos);
                Save_NaviData();
            }

            RELEASE_INSTANCE(CKeyManager);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
#pragma endregion
    }
    ImGui::End();

    if (m_ObjectTransform.size() != 0)
    {
        Set_Objects(DeltaTime);
    }
    
    return _int();
}

_int CObjectTool::Late_Tick(_double DeltaTime)
{
    return _int();
}

HRESULT CObjectTool::Render()
{
    if (nullptr == m_pMesh || nullptr == m_pGraphic_Device)
    {
        return E_FAIL;
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    
    _matrix WorldMatrix = *m_pTransform->Get_WorldMatrix();
    _matrix ViewMatrix  = pGameInstance->Get_Transform(D3DTS_VIEW);
    _matrix ProjMatrix  = pGameInstance->Get_Transform(D3DTS_PROJECTION);
    
    m_pShader->Set_ConstTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ViewMatrix",  &ViewMatrix,  sizeof(_matrix));
    m_pShader->Set_ConstTable("g_ProjMatrix",  &ProjMatrix,  sizeof(_matrix));

    RELEASE_INSTANCE(CGameInstance);
    
    m_pShader->Begin_Shader(0);
    
    m_pMesh->DrawSubset(0);
    
    m_pShader->End_Shader();

    return S_OK;
}

HRESULT CObjectTool::Set_Objects(_double DeltaTime)
{
    m_ObjectTransform[m_iObjectIndex]->Set_State(CTransform::STATE_POSITION, vPos);
    m_ObjectTransform[m_iObjectIndex]->Set_Scale(_float3(fScale, fScale, fScale));

    CKeyManager* pKey = GET_INSTANCE(CKeyManager);

    if (pKey->Key_Pressing(KEY_Z))
    {
        if (m_bSlow == true)
        {
            m_ObjectTransform[m_iObjectIndex]->Rotation_Axis(_float3(0.f, 1.f, 0.f), DeltaTime / 10);
        }
        else
        {
            m_ObjectTransform[m_iObjectIndex]->Rotation_Axis(_float3(0.f, 1.f, 0.f), DeltaTime);
        }
    }
    if (pKey->Key_Pressing(KEY_C))
    {
        if (m_bSlow == true)
        {
            m_ObjectTransform[m_iObjectIndex]->Rotation_Axis(_float3(0.f, 1.f, 0.f), -DeltaTime / 10);
        }
        else
        {
            m_ObjectTransform[m_iObjectIndex]->Rotation_Axis(_float3(0.f, 1.f, 0.f), -DeltaTime);
        }
    }
    if (pKey->Key_Pressing(KEY_UP))
    {
        m_ObjectTransform[m_iObjectIndex]->Move_Front(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_DOWN))
    {
        m_ObjectTransform[m_iObjectIndex]->Move_Back(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_LEFT))
    {
        m_ObjectTransform[m_iObjectIndex]->Move_Left(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_RIGHT))
    {
        m_ObjectTransform[m_iObjectIndex]->Move_Right(DeltaTime);
    }

    RELEASE_INSTANCE(CKeyManager);

    return S_OK;
}

HRESULT CObjectTool::Set_Navigation(_double DeltaTime)
{
    CKeyManager* pKey = GET_INSTANCE(CKeyManager);

    if (pKey->Key_Pressing(KEY_UP))
    {
        m_pTransform->Move_Front(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_DOWN))
    {
        m_pTransform->Move_Back(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_LEFT))
    {
        m_pTransform->Move_Left(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_RIGHT))
    {
        m_pTransform->Move_Right(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_1))
    {
        m_pTransform->Move_Up(DeltaTime);
    }
    if (pKey->Key_Pressing(KEY_2))
    {
        m_pTransform->Move_Down(DeltaTime);
    }

    RELEASE_INSTANCE(CKeyManager);

    return S_OK;
}

void CObjectTool::Save_Data()
{
    ofstream fout("../../PosInfo.txt");

    _uint iSize = m_ObjectTransform.size();
    _uint iIndexSize = m_Index.size();

    for (_uint i = 0; i < iSize; ++i)
    {
        fout << "Hieracon" << m_Index[i] << endl;
        fout << "Right----------" << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[0][0] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[0][1] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[0][2] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[0][3] << endl;
        fout << "UP-------------" << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[1][0] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[1][1] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[1][2] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[1][3] << endl;
        fout << "LOOK-----------" << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[2][0] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[2][1] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[2][2] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[2][3] << endl;
        fout << "POSITION-------" << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[3][0] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[3][1] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[3][2] << endl;
        fout << m_ObjectTransform[i]->Get_WorldMatrix()->m[3][3] << endl;
        fout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    }

    fout.close();

}

void CObjectTool::Save_NaviData()
{
    ofstream fout("../../NaviInfo.txt");

    _uint iSize = m_NaviPoints.size();

    for (_uint i = 0; i < iSize; ++i)
    {
        fout << fixed;
        fout.precision(2);

        fout << i+1 << " = _float3(" << m_NaviPoints[i].x << "f, " << m_NaviPoints[i].y << "f, " << m_NaviPoints[i].z << "f);" << endl;
    }

    fout.close();
}

HRESULT CObjectTool::Create_Object()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _tchar* szPrototypeTag = new _tchar[MAX_PATH];
    _tchar* szFileName = new _tchar[MAX_PATH];
    _tchar  szDotX[MAX_PATH] = TEXT(".X");
    _tchar  szKeyTag[MAX_PATH] = TEXT("");

    ZeroMemory(szPrototypeTag, sizeof(MAX_PATH));
    ZeroMemory(szFileName, sizeof(MAX_PATH));

    lstrcpy(szPrototypeTag, TEXT("Prototype_Mesh_Hieracon"));
    lstrcpy(szFileName, TEXT("Hieracon"));

    _itow_s(m_iKey, szKeyTag, 10);

    wcscat(szPrototypeTag, szKeyTag);
    wcscat(szFileName, szKeyTag);
    wcscat(szFileName, szDotX);

    pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, szPrototypeTag, CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resource/Bless/Mesh/StaticMesh/Map/Hieracon/"), szFileName));

    if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_Test"), TEXT("Layer_Test"), &m_iKey)))
    {
        return E_FAIL;
    }

    CTransform* pObjectTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Test"), TEXT("Com_Transform"), m_iIndex);

    Safe_AddRef(pObjectTransform);

    ++m_iIndex;

    m_ObjectTransform.emplace_back(pObjectTransform);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CObjectTool* CObjectTool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CObjectTool* pInstance = new CObjectTool(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct(pGraphic_Device)))
    {
        MSGBOX("Failed to Creating ObjectTool");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CObjectTool::Clone(void* pArg)
{
    return nullptr;
}

void CObjectTool::Free()
{
    __super::Free();

    for (auto& pTransform : m_ObjectTransform)
    {
        Safe_Release(pTransform);
    }

    m_ObjectTransform.clear();
    m_NaviPoints.clear();

    Safe_Release(m_pMesh);
    Safe_Release(m_pShader);
    Safe_Release(m_pPicking);
    Safe_Release(m_pTransform);
}