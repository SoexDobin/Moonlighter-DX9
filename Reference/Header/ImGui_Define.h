#pragma once
#include <string.h>
namespace Engine
{
	typedef struct tagEditorField
	{
		const char* _label;
		DATATYPE _type;
		void* _pData;
	}EDITORFIELD;

//#define PANEL_NAME(name);	\
//	wcscpy_s(m_szDisplayName, name);	\
//	WideCharToMultiByte(CP_ACP, 0, m_szDisplayName, -1, m_szBuffer, 256, nullptr, nullptr);

#define PANEL_NAME(name) \
    wcscpy_s(m_szDisplayName, name.c_str()); \
    WideCharToMultiByte(CP_UTF8, 0, m_szDisplayName, -1, m_szBuffer, 256, nullptr, nullptr);


}

