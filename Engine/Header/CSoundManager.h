#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CSoundManager : public CBase
{
    DECLARE_SINGLETON(CSoundManager)
private:
    explicit CSoundManager();
    virtual ~CSoundManager() override;

public:
    HRESULT     Ready_SoundManager();

    void        PlaySoundOnce(const _tchar* pSoundKey, CHANNELID eID, float fVolume);
    void        PlaySound(const _tchar* pSoundKey, CHANNELID eID, float fVolume);
    void        PlayBGM(const _tchar* pSoundKey, float fVolume);
    void        StopSound(CHANNELID eID);
    void        StopAll();
    void        SetChannelVolume(CHANNELID eID, float fVolume);

private:
    void        LoadSoundFile();

private:
    unordered_map<_tchar*, FMOD_SOUND*> m_umapSound;
    FMOD_CHANNEL* m_pChannelArr[Engine::MAXCHANNEL];
    FMOD_SYSTEM* m_pSystem;

private:
    void        Free() override;
};

END