#pragma once

#include "Engine_Define.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CDInputManager : public CBase
{
	DECLARE_SINGLETON(CDInputManager)

private:
	explicit CDInputManager();
	virtual ~CDInputManager();

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) const
	{
		return m_byKeyState[byKeyID];
	}

    _byte	Get_DIKeyPressed(_ubyte byKeyID) const
    {
        if ((m_byKeyState[byKeyID] & 0x80) && !(m_byPrevKeyState[byKeyID] & 0x80))
            return _uint8(0x80);

        return _uint8(0x00);
    }

    _byte	Get_DIKeyUp(_ubyte byKeyID) const
    {
        if (!(m_byKeyState[byKeyID] & 0x80) && (m_byPrevKeyState[byKeyID] & 0x80))
            return _uint8(0x80);

        return _uint8(0x00);
    }

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) const 
	{
		return m_tMouseState.rgbButtons[eMouse];
	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];
	_byte					m_byPrevKeyState[256];
	DIMOUSESTATE			m_tMouseState;

public:
	virtual void	Free(void);

};

END
