
// SSCOM.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSSCOMApp: 
// �йش����ʵ�֣������ SSCOM.cpp
//

class CSSCOMApp : public CWinApp
{
public:
	CSSCOMApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSSCOMApp theApp;