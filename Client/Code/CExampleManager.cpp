#include "pch.h"
#include "CExampleManager.h"

// Must be included
#include "CEditor.h"
#include "CPanel.h"

IMPLEMENT_SINGLETON(CExampleManager)

CExampleManager::CExampleManager()
{

}

CExampleManager::~CExampleManager()
{
}

HRESULT CExampleManager::Ready_Manager()
{
	Add_ExamplePanel1();
	
	Add_ExamplePanel2();

	return S_OK;
}

_int CExampleManager::Update_Manager()
{
	// Set a breakpoint here to inspect variable changes
	m_fExam;
	m_iExam;
	m_vExam;

	return 0;
}

void CExampleManager::Add_ExamplePanel1()
{
	CPanel* pPanel = CEditor::GetInstance()->Create_Panel("Example");
	pPanel->Add_EditorField("Exam int :", DATATYPE::DATA_INT, &m_iExam);
	pPanel->Add_EditorField("Exam float : ", DATATYPE::DATA_FLOAT, &m_fExam);
}

void CExampleManager::Add_ExamplePanel2()
{
	CEditor::GetInstance()->Find_Panel("Example")->Add_EditorField({ "float ? ", DATATYPE::DATA_VEC3, &m_vExam });
}
