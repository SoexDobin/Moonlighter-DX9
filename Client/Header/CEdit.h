#pragma once
#include "CScene.h"
class CEdit : public CScene
{
private:
	explicit CEdit(LPDIRECT3DDEVICE9 pGraphciDev);
	virtual ~CEdit();

	_vec4 clear_color = { 0.f, 0.f, 0.f, 0.f };
	bool show_demo_window = true;
	bool show_another_window = true;

	HRESULT Ready_Environment_Layer(const wstring pLayerTag);
	HRESULT Ready_GameLogic_Layer(const wstring pLayerTag);
	HRESULT Ready_UI_Layer(const wstring pLayerTag);

	HRESULT Ready_Prototype();
	virtual void Free();

public:
	HRESULT Ready_Scene() override;
	_int Update_Scene(const _float fTimeDelta) override;
	void LateUpdate_Scene(const _float fTimeDelta) override;
	void Render_Scene() override;

	static CEdit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

