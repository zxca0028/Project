#pragma once

#include "Base.h"

class CSound_Manager
{
public:
	static CSound_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSound_Manager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID { BGM, EVENT_BGM, JUMP, MOKOKO, INPUT, CRASH, MAXCHANNEL };
private:
	CSound_Manager();
	~CSound_Manager();

public:
	void Initialize();
	void Release();
public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVol=1.0f);
	void PlayBGM(const TCHAR* pSoundKey, float fVol = 1.0f);
	void StopSound(CHANNELID eID);
	void StopAll();

	void Set_Volume(CHANNELID eID, float fVol);

private:
	void LoadSoundFile();
	void LoadSoundFilePrev(const char* szName);

private:
	static CSound_Manager* m_pInstance;
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};
