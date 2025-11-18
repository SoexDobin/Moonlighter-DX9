#pragma once
#include "CScene.h"

class CMainScene final : public CScene
{
private:
	explicit CMainScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainScene() override;

public:
	HRESULT			Ready_Scene() override;
	_int			Update_Scene(const _float fTimeDelta) override;
	void			LateUpdate_Scene(const _float fTimeDelta) override;
	void			Render_Scene() override;

private:
	HRESULT			Ready_Camera_Layer(const wstring wsLayerTag);
	HRESULT			Ready_Environment_Layer(const wstring wsLayerTag);
	HRESULT			Ready_GameLogic_Layer(const wstring wsLayerTag);
	HRESULT			Ready_UI_Layer(const wstring wsLayerTag);

	HRESULT			Ready_Prototype();

public:
	static CMainScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void			Free() override;
};

