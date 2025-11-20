#pragma once
#include "CRenderObject.h"

class CExampleObject : public CRenderObject
{
private:
	explicit CExampleObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CExampleObject(const CExampleObject& rhs);
	virtual ~CExampleObject();

public :
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float fTimeDelta) override;
	void		LateUpdate_GameObject(const _float fTimeDelta) override;
	void		Render_GameObject() override;

public :
	static CExampleObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual void Free();


public :
	void Add_EditorField();

	_bool			m_bBool = true;
	_byte			m_bByte = -10;
	_ubyte			m_ubByte = 120;
	TCHAR			m_szChar[32] = L"test";
	_short			m_sShort = 'A';
	_ushort		m_usShort = ' B';
	_int				m_iInt = 23232;
	_uint			m_uiInt = 21454;
	_long			m_lLong = 155;
	_ulong			m_ulLong = 12354;
	_float			m_fFloat = 0.0656f;
	_double       m_dDouble = 0.25f;
	_vec2			m_v2 = { 0.f, 5.f };
	_vec3			m_v3 = { 0.f, 2.f, 5.f };
	_vec4			m_v4 = { 0.f, 2.f, 6.f, 8.f };
	_matrix		m_mat;
};

