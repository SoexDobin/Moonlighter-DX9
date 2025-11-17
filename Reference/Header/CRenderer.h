#pragma once

#include "CBase.h"
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)
private:
	explicit CRenderer();
	virtual ~CRenderer() override;

public:
	void	Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Clear_RenderGroup();
private:
	void	Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];

private:
	virtual void		Free();
};

END
