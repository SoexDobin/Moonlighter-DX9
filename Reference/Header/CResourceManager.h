#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CResourceManager final : public CBase
{
	DECLARE_SINGLETON(CResourceManager)
private:
	explicit CResourceManager();
	virtual ~CResourceManager() override;

public:
	HRESULT		Add_Sprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& swKey, const wstring& swPath, _uint iCnt);
	HRESULT		Add_Cube(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& swKey, const wstring& swPath);

public:
	const vector<IDirect3DBaseTexture9*>&	Get_Sprite(const wstring& swKey);

private:
	unordered_map<wstring, vector<IDirect3DBaseTexture9*>>		m_umapSprite;
	unordered_map<wstring, vector<IDirect3DCubeTexture9*>>		m_umapCube;

private:
	void		Free() override;
};

END
