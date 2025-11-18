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
	_bool		IsPermit_Call(const wstring wsFrameTag, const _float fTimeDelta);
	HRESULT		Ready_Frame(const wstring wsFrameTag, const _float fCallLimit);

private:
	CFrame*		Find_Frame(const wstring wsFrameTag);

private:
	unordered_map<wstring, CFrame*>	m_umFrame;

private:
	void	Free() override;
};

END