#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame() override;

public:
	_bool		IsPermit_Call(const _float fTimeDleta);

public:
	HRESULT		Ready_Frame(const _float fCallLimit);

private:
	_float		m_fCallLimit;
	_float		m_fAccTimeDelta;

public:
	static CFrame* Create(const _float fCallLimit);

private:
	void		Free() override;
};

END
