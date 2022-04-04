#include "pch.h"
#include "Sound_Manager.h"

CSound_Manager* CSound_Manager::m_pInstance = nullptr;

CSound_Manager::CSound_Manager()
{
	m_pSystem = nullptr;
}


CSound_Manager::~CSound_Manager()
{
	Release();
}

void CSound_Manager::Initialize()
{
	FMOD_System_Create(&m_pSystem);

	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSound_Manager::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSound_Manager::PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVol/*=0.5f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	//if (/*FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay)*/eID != CSound_Manager::CHANNELID::BUBBLE_POP)
	//{
	//	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	//}
	//else
	//{


	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVol);
	/*}*/

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(const TCHAR* pSoundKey, float fVol)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;


	FMOD_SOUND* pSound = nullptr;
	FMOD_Channel_GetCurrentSound(m_pChannelArr[BGM], &pSound);
	if (pSound == iter->second)
	{
		return;
	}


	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVol);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::Set_Volume(CHANNELID eID, float fVol)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVol);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::LoadSoundFile()
{
	_finddatai64_t fd;

	long handle = _findfirsti64("../Sound/*.*", &fd);

	if (handle == 0)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
				{
					return !lstrcmp(pSoundKey, iter.first);
				});
			if (iter == m_mapSound.end())
			{
				m_mapSound.emplace(pSoundKey, pSound);
			}
		}
		iResult = _findnexti64(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

void CSound_Manager::LoadSoundFilePrev(const char* szName)
{
	_finddata_t fd;

	long handle = _findfirst(szName, &fd);

	if (handle == 0)
		return;

	int iResult = 0;

	char szCurPath[128] = "../bin/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
				{
					return !lstrcmp(pSoundKey, iter.first);
				});
			if (iter == m_mapSound.end())
			{
				m_mapSound.emplace(pSoundKey, pSound);
			}
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}
