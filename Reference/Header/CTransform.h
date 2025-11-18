#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() override;

public:
	const _vec3&	Get_Scale() const { return m_vScale; }
	void			Set_Scale(const _vec3& vScale) { m_vScale = { vScale.x , vScale.y, vScale.z }; }
	void			Set_Scale(const _float fX, const _float fY, const _float fZ) { m_vScale = { fX ,fY, fZ }; }

	const _vec3&	Get_Angle() const { return m_vAngle; }
	void			Set_Angle(const _vec3& vAngle) { m_vAngle = { vAngle.x , vAngle.y, vAngle.z }; }
	void			Set_Angle(const _float fX, const _float fY, const _float fZ) { m_vAngle = { fX ,fY, fZ }; }

	const _vec3&	Get_Pos() const { return m_vInfo[INFO_POS]; }
	void			Set_Pos(const _vec3& vPos) { m_vInfo[INFO_POS] = { vPos.x , vPos.y, vPos.z }; }
	void			Set_Pos(const _float fX, const _float fY, const _float fZ) { m_vInfo[INFO_POS] = { fX ,fY, fZ }; }

	void			Get_Info(INFO eType, _vec3* pInfo) { memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3)); }

	void			Move_Pos(const _vec3* pDir, const _float fTimeDelta, const _float fSpeed)
	{
		m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
	}

	void			Rotation(ROTATION eType, const _float fAngle) { *(((_float*)&m_vAngle) + eType) += fAngle; }
	_matrix*		Get_World() { return &m_matWorld; }

public:
	HRESULT				Ready_Transform();
	virtual _int		Update_Component(const _float fTimeDelta) override;
	virtual void		LateUpdate_Component() override;

private:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TRANSFORM; }

private:
	virtual void			Free() override;
};

END