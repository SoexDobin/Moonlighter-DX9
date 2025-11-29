#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed);
	explicit CTexture(const CTexture& rhs);
	virtual	~CTexture() override;

public:
	const _uint			Get_FrameCount(const _uint iTex);

	_bool			Get_Loop()						{ return m_bLoop; }
	void			Set_Loop(_bool bIsLoop)			{ m_bLoop = bIsLoop; }
	_bool			Get_OneLoop()					{ return m_bOneLoop; }
	void			Set_OneLoop(_bool bIsOneLoop)	{ m_bOneLoop = bIsOneLoop; }

	_int			Get_CurTex()					{ return m_iCurTex; }
	_int			Get_CurFrame()					{ return m_iCurFrame; }
	_float			Get_Speed()						{ return m_fSpeed; }
	void			Set_Speed(_float fSpeed)		{ m_fSpeed = fSpeed; }

public:
	void			Stop_Anim() { m_bStop = true; }
	void			Resume_Anim() { m_bStop = false; }


public:
	HRESULT				Ready_Texture(const wstring& wsPath);
	void				Set_Texture(const _uint iTex, const _uint iFrame = 0);

	_int				Update_Component(const _float fTimeDelta) override;
	void				LateUpdate_Component() override;
    void                SetUp_Texture();                                        // GameObject 랜더링시 버퍼 세팅전 필수 호출

private:
	vector<vector<IDirect3DBaseTexture9*>>	m_vecTexture;           /// 클라이언트에서 등록한 스프라이트 [스프라이트 그룹] [스프라이트 png들]
	_bool	m_bStop;                    // 애니매이션 정지
	_bool	m_bLoop;                    // 반복 애니매이션 인지, false면 마지막 프레임 시 정지
	_bool	m_bOneLoop;                 // 텍스쳐 변경 후 반복을 한번 이상 했는지 여부

	_uint	m_iCurTex;                  // 현재 텍스쳐 인덱스
	_uint	m_iCurFrame;                // 현재 프레임 인덱스

	_float	m_fSpeed;                   // 프레임당 현재 애니매이션 속도 값
	_float  m_fFrameAcc;                // 프레임 누산 계산 변수

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TEXTURE; }

private:
	virtual void		Free() override;

#pragma region Editor
public:
	virtual void Display_Editor(const char* pObjTag) override;
#pragma endregion

};

END
