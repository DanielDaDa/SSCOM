// SSCOMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SSCOM.h"
#include "SSCOMDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:	
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{	
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)	
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()
// CSSCOMDlg 对话框
CSSCOMDlg::CSSCOMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSSCOMDlg::IDD, pParent)
	, m_static_status(_T(""))
	, m_baudrate(_T(""))
	, m_databit(_T(""))
	, m_paritybit(_T(""))
	, m_stopbit(_T(""))
	, m_check_hex(FALSE)
	, m_rxbyte(0)
	, m_rxframe(0)
	, m_txframe(0)
	, m_txbyte(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSCOMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm1);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_static_status);
	DDX_Control(pDX, IDC_COMINDEX, m_comindex);
	DDX_CBString(pDX, IDC_BAUDRATE, m_baudrate);
	DDX_CBString(pDX, IDC_DATABIT, m_databit);
	DDX_CBString(pDX, IDC_PARITYBIT, m_paritybit);
	DDX_CBString(pDX, IDC_STOPBIT, m_stopbit);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Check(pDX, IDC_CHECK_HEX, m_check_hex);
	DDX_Control(pDX, IDC_DATAFORMAT, m_dataformat);
	DDX_Text(pDX, IDC_STATIC_RXFRAME, m_rxframe);
	DDX_Text(pDX, IDC_STATIC_RXBYTE, m_rxbyte);
	DDX_Text(pDX, IDC_STATIC_TXFRAME, m_txframe);
	DDX_Text(pDX, IDC_STATIC_TXBYTE, m_txbyte);
}

BEGIN_MESSAGE_MAP(CSSCOMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSSCOMDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSSCOMDlg::OnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSSCOMDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK_HEX, &CSSCOMDlg::OnBnClickedCheckHex)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLIST, &CSSCOMDlg::OnClickedButtonClearlist)
END_MESSAGE_MAP()

// CSSCOMDlg 消息处理程序
BOOL CSSCOMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_baudrate = "115200";
	m_databit = "8";
	m_paritybit = "n";
	m_stopbit = "1";
	UpdateData(FALSE);
	m_comindex.SetCurSel(0);
	m_dataformat.SetCurSel(0);
	buf_count = 1;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSSCOMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CSSCOMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSSCOMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSSCOMDlg::OnBnClickedButtonConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_mscomm1.get_PortOpen())
	{
		m_mscomm1.put_PortOpen(FALSE);	//关闭串口
		m_static_status = "已关闭";
		bOpen = FALSE;
		UpdateData(FALSE);
		return;
	}
	
	m_mscomm1.put_CommPort(m_comindex.GetCurSel() + 1);	//设定串口号
	m_mscomm1.put_RThreshold(1);		//每接收到一个字符时，触发OnComm事件
	m_mscomm1.put_InBufferSize(2048);	//设置输入缓冲区大小为2048 byte	

	if (!m_mscomm1.get_PortOpen())
		m_mscomm1.put_PortOpen(TRUE);	//打开串口	
	
	CString str;
	str = m_baudrate + ",n," + m_databit + "," + m_stopbit;
	m_mscomm1.put_Settings(str);	//波特率m_baudrate，无校验，m_databit数据位，m_stopbit停止位
	m_mscomm1.put_InputMode(1);		// 以二进制方式检取数据
	m_mscomm1.put_RThreshold(1);	//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	m_mscomm1.put_InputLen(0);		//设置当前接收区数据长度为0
	m_mscomm1.get_Input();			//先预读缓冲区以清除残留数据
	bOpen = TRUE;

	m_static_status = "已打开";
	UpdateData(FALSE);
}

BEGIN_EVENTSINK_MAP(CSSCOMDlg, CDialogEx)
	ON_EVENT(CSSCOMDlg, IDC_MSCOMM1, 1, CSSCOMDlg::OnOncommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

void CSSCOMDlg::OnOncommMscomm1()
{
	// TODO:  在此处添加消息处理程序代码
	CString strtemp;
	if (m_mscomm1.get_CommEvent() == 2)				//事件值为2表示接收缓冲区内有字符
	{
		ole_variant.Attach(m_mscomm1.get_Input());
		safearray_inp = ole_variant;				//VARIANT型变量转换为ColeSafeArray型变量
		com_len = safearray_inp.GetOneDimSize();	//得到有效数据长度

		for (com_k = 0; com_k<com_len; com_k++)		//转换为BYTE型数组
		{
			safearray_inp.GetElement(&com_k, rxbuf + buf_count);		
			buf_count++;
		}
		
		for (com_k = 0; com_k < com_len; com_k++)	//将数组转换为Cstring型变量 
		{
			BYTE bt = *(char*)(rxbuf + com_k + 1);	//将字符送入临时变量strtemp存放  
			strtemp.Format(_T("%c"), bt);			//加入接收编辑框对应字符串   
			ASCIIRx += strtemp;
		}

		List_inbuf(rxbuf);

		m_rxbyte += com_len;
		m_rxframe += 1;
		UpdateData(FALSE);

		ASCIIRx.Empty();
		buf_count = 1;
	}
}

CString CSSCOMDlg::Dec_2CHAR(int input)
{
	CString str;
	str.Format(_T("%d"), input);
	return str;
}

CString CSSCOMDlg::Dec_2CHAR_2bit(int input)
{
	CString str;
	if (input<10)
		str.Format(_T("0%d"), input);
	else
		str.Format(_T("%d"), input);
	return str;
}

CString CSSCOMDlg::Dec_2HEX_2bit(int input)
{
	CString str;
if (input<16)
	str.Format(_T("0%X"), input);
else str.Format(_T("%X"), input);
return str;
}


//void CSSCOMDlg::ShowInfo(CListBox *pListBox, CString infoStr)
//{
//	CRect clientRect;
//	pListBox->GetClientRect(&clientRect);
//	int maxWidth = clientRect.Width();
//
//	int len = infoStr.GetLength();
//
//	CFont *pFont = pListBox->GetFont();
//	CDC *pDC = pListBox->GetDC();
//
//	CFont *pOldFont = pDC->SelectObject(pFont);
//
//	CSize sz = pDC->GetTextExtent(infoStr, len);
//
//	while (sz.cx > maxWidth)
//	{
//		len--;
//		sz = pDC->GetTextExtent(infoStr, len);
//	}
//
//	pDC->SelectObject(pOldFont);
//
//	pListBox->AddString(infoStr.Left(len));
//
//	int strLen = infoStr.GetLength();
//	if (len != strLen)
//	{
//		ShowInfo(pListBox, infoStr.Right(strLen - len));
//	}
//}


void CSSCOMDlg::List_outbuf()
{
	CString str;
	CTime ct = CTime::GetCurrentTime();
	CString str_time;

	if (m_check_hex)
	{
		str = "发送 0x";
		CString str_input1;
		m_edit.GetWindowText(str_input1);
		int len;
		len = str_input1.GetLength();
		for (int i = 0; i < (len / 2 + len % 2); i++)
		{
			str = str + Dec_2HEX_2bit(WindowTextArray[i]);
		}
	}
	else
	{
		str = "发送 ASCII ";
		CString str_input2;
		m_edit.GetWindowText(str_input2);
		str = str + str_input2;
	}
	
	str_time = Dec_2CHAR_2bit(ct.GetHour()) + ":" + Dec_2CHAR_2bit(ct.GetMinute()) + ":" + Dec_2CHAR_2bit(ct.GetSecond());
	str_time = str_time + " " + str;

	//ShowInfo(&m_list, str_time);
	m_list.InsertString(-1,str_time);
	m_list.SetCurSel(m_list.GetCount() - 1);
}


void CSSCOMDlg::List_inbuf(unsigned char *buf)
{
	CTime ct = CTime::GetCurrentTime();
	CString str_time;
	CString str;

	str_time = Dec_2CHAR_2bit(ct.GetHour()) + ":" + Dec_2CHAR_2bit(ct.GetMinute()) + ":" + Dec_2CHAR_2bit(ct.GetSecond());

	if (m_dataformat.GetCurSel() == 1)
	{
		str = " 接收 0x";
		for (int i = 0; i < com_len ; i++)
			str = str + Dec_2HEX_2bit(buf[i+1]);
	}

	if (m_dataformat.GetCurSel() == 0)
	{
		str = " 接收 ASCII ";
		str = str + ASCIIRx;
	}

	str_time = str_time + str;
	m_list.InsertString(-1,str_time);
	m_list.SetCurSel(m_list.GetCount() - 1);
}


void CSSCOMDlg::OnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_edit.SetWindowText(_T(""));
}


void CSSCOMDlg::OnClickedButtonSend()
{
	//// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString str;
	m_edit.GetWindowText(str);

	if (m_check_hex)
	{
		char *stops, s[3];	//CString ->HEX
		int len;
		len = str.GetLength();

		WindowTextArray.SetSize(len / 2 + len % 2);
		for (size_t i = 0; i < (len / 2 + len % 2); i++)
		{
			s[0] = str.GetAt(i * 2);
			s[1] = str.GetAt(i * 2 + 1);
			s[2] = 0x0;
			WindowTextArray[i] = strtoul(s, &stops, 16);
		}
		varOutput = WindowTextArray;

		m_txbyte += (len / 2 + len % 2);
		UpdateData(FALSE);
	}
	else
	{
		m_txbyte += str.GetLength();
		UpdateData(FALSE);

		varOutput = str;
	}
		
	if (bOpen)
	{
		if (str.IsEmpty())
		{
			CString noinput;
			noinput = "不输入数据，你想干什么？";
			m_list.InsertString(-1,noinput);
			m_list.SetCurSel(m_list.GetCount() - 1);

			varOutput.Clear();
			WindowTextArray.RemoveAll();
			return;
		}

		m_mscomm1.put_Output(varOutput);
		List_outbuf();
		m_txframe += 1;
		UpdateData(FALSE);
		varOutput.Clear();
		WindowTextArray.RemoveAll();
	}
	else
	{
		MessageBox(_T("           你串口开了？"));
		WindowTextArray.RemoveAll();
		varOutput.Clear();
		return;
	}
}


void CSSCOMDlg::OnBnClickedCheckHex()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CSSCOMDlg::OnClickedButtonClearlist()
{
	// TODO:  在此添加控件通知处理程序代码
	m_list.ResetContent();
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

