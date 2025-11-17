#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
	: m_pSystem(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	Free();
}

HRESULT CSoundManager::Ready_SoundManager()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	return S_OK;
}

void CSoundManager::PlaySoundOnce(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	unordered_map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_umapSound.begin(), m_umapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_umapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundManager::PlaySound(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	unordered_map<_tchar*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_umapSound.begin(), m_umapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_umapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundManager::PlayBGM(const _tchar* pSoundKey, float fVolume)
{
	unordered_map<_tchar*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_umapSound.begin(), m_umapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_umapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[CHANNEL_0]);
	FMOD_Channel_SetMode(m_pChannelArr[CHANNEL_0], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[CHANNEL_0], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundManager::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundManager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundManager::LoadSoundFile()
{
	// TODO : 사운드 가져오는 방식 다시 체그 필요함
	// READY에서 패스 지정해서 던저주는 방식이 이로울 듯
	_finddata_t fd;
	intptr_t lHandle = _findfirst("../Resource/Sound/*.mp3", &fd);
	if (lHandle == -1) return;

	int iResult = 0;

	char szCurPath[256] = "../Resource/Sound/";
	char szFullPath[256] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = static_cast<int>(strlen(fd.name) + 1);

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_umapSound.emplace(pSoundKey, pSound);
		}
	
		iResult = _findnext(lHandle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(lHandle);
}

void CSoundManager::Free()
{
	for (auto& Mypair : m_umapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_umapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
