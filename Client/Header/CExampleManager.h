#pragma once
#include "CBase.h"
#include "Engine_Define.h"

namespace Engine
{
    class CScene;
}

class CExampleManager : public CBase
{
	DECLARE_SINGLETON(CExampleManager)
private:
	explicit CExampleManager();
	virtual ~CExampleManager() override;

public :
	HRESULT Ready_Manager();
	_int		Update_Manager();

private :
	void		Add_ExamplePanel1();
	void		Add_ExamplePanel2();

private:
	_int	m_iExam = 10;
	_float m_fExam = -25.f;
	_vec3 m_vExam = { 0.f, 5.f, 2.f };

private :
    vector<CScene*> m_vecScene;

private :
	virtual void Free() {}

};

