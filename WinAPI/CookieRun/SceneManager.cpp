#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Loading.h"
#include "Lobby.h"
#include "CookieSelect.h"
#include "StageLoading.h"
#include "Stage.h"
#include "Result.h"

#include "RenderTest.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager()
    : m_eCurrentScene(SCENE::END)
    , m_eNextScene(SCENE::END)
    , m_pScene(nullptr)
{}

CSceneManager::~CSceneManager()
{
    Release_Scene_Manager();
}

CSceneManager* CSceneManager::Get_Instance()
{
    if (nullptr == m_pInstance) { m_pInstance = new CSceneManager; }
    return m_pInstance;
}

void CSceneManager::Destroy_Instance()
{
    if (nullptr != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void CSceneManager::Scene_Change(SCENE::TYPE eNextID)
{
    m_eNextScene = eNextID;

    if (m_eCurrentScene != m_eNextScene)
    {
        Safe_Delete(m_pScene);

        switch (m_eNextScene)
        {
        case SCENE::TYPE::LOADING:
            m_pScene = CLoading::Create();
            break;

        case SCENE::TYPE::LOBBY:            
            m_pScene = CLobby::Create();
            break;

        case SCENE::TYPE::COOKIESELECT:
            m_pScene = CCookieSelect::Create();
            break;

        case SCENE::TYPE::STAGELOADING:
            m_pScene = CStageLoading::Create();
            break;

        case SCENE::TYPE::STAGE:
            m_pScene = CStage::Create();
            break;

        case SCENE::TYPE::RESULT:
            m_pScene = CResult::Create();
            break;

        case SCENE::TYPE::RENDERTEST:
            m_pScene = CRenderTest::Create();
            break;

        default:
            break;
        }

        m_eCurrentScene = m_eNextScene;

    }
}

void CSceneManager::Update_Scene_Manager()
{
    m_pScene->Update_Scene();
}

void CSceneManager::Render_Scene_Manager(HDC hDC)
{
    m_pScene->Render_Scene(hDC);
}

void CSceneManager::Release_Scene_Manager()
{
    Safe_Delete(m_pScene);
}
