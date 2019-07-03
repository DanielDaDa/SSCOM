
// SSCOMDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <iostream>

// CSSCOMDlg 对话框
class CSSCOMDlg : public CDialogEx
{
// 构造
public:
	CSSCOMDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SSCOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnClickedButtonClear();
	afx_msg void OnClickedButtonSend();
	DECLARE_EVENTSINK_MAP()
	void OnOncommMscomm1();
	void List_outbuf();
	void List_inbuf(unsigned char *buf);
	CString Dec_2HEX_2bit(int input);
	CString Dec_2CHAR(int input);
	CString Dec_2CHAR_2bit(int input);
	void ShowInfo(CListBox *pListBox, CString infoStr);

	CByteArray OutBuf;
	CByteArray WindowTextArray;
	COleVariant varOutput;
	COleVariant ole_variant;
	COleSafeArray safearray_inp;
	CString ASCIIRx;
	LONG com_len, com_k;
	unsigned char rxbuf[1000];
	int buf_count;
	CMscomm1 m_mscomm1;
	CString m_static_status;
	CComboBox m_comindex;
	bool bOpen;	//串口状态
	CString m_baudrate;
	CString m_databit;
	CString m_paritybit;
	CString m_stopbit;
	CListBox m_list;
	CEdit m_edit;

public:
	BOOL m_check_hex;
	afx_msg void OnBnClickedCheckHex();
	afx_msg void OnClickedButtonClearlist();
	CComboBox m_dataformat;
	long m_rxframe;
	long m_rxbyte;
	long m_txframe;
	long m_txbyte;
};


