#include "CPanel.h"

#include "CEditor.h"

CPanel::CPanel() : bDisplayEditor(false)
{
}

CPanel::~CPanel()
{
}

void CPanel::Display_Editor()
{
	ImGui::Begin(szPanelName);

	for (const EDITORFIELD& field : m_EditorFieldList)
	{
		CEditor::GetInstance()->Display_Editor(field);
	}

	ImGui::End();
}

CPanel* CPanel::Create(const char* pName)
{
	CPanel* pPanel = new CPanel();

	if (!pName)
		pName = "Debug Panel";

	pPanel->Set_PanelName(pName);

	return pPanel;
}

void CPanel::Free()
{

}