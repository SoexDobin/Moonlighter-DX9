#pragma once

#include "CBase.h"
#include "CFrame.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrameManager : public CBase
{
	DECLARE_SINGLETON(CFrameManager)

private:
	explicit CFrameManager();
	virtual ~CFrameManager() override;

public:
	_bool		IsPermit_Call(const _tchar* pFrameTag, const float fTimeDelta);
	HRESULT		Ready_Frame(const _tchar* pFrameTag, const float fCallLimit);

private:
	CFrame*		Find_Frame(const _tchar* pFrameTag);

private:
	unordered_map<const _tchar*, CFrame*>	m_umFrame;

private:
	void	Free() override;
};

END