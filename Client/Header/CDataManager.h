#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CDataManager : public CBase
{
	DECLARE_SINGLETON(CDataManager)
private:
	explicit CDataManager();
	virtual ~CDataManager() override;

public:
	HRESULT		Ready_Data();

private:
	HRESULT		Ready_Resource();
	HRESULT		Ready_Sound();

private:
	void		Free() override;
};

