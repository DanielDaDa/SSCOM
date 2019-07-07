// SSCOMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SSCOM.h"
#include "SSCOMDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:	
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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
// CSSCOMDlg �Ի���
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

// CSSCOMDlg ��Ϣ�������
BOOL CSSCOMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_baudrate = "115200";
	m_databit = "8";
	m_paritybit = "n";
	m_stopbit = "1";
	UpdateData(FALSE);
	m_comindex.SetCurSel(0);
	m_dataformat.SetCurSel(0);
	buf_count = 1;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CSSCOMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSSCOMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSSCOMDlg::OnBnClickedButtonConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_mscomm1.get_PortOpen())
	{
		m_mscomm1.put_PortOpen(FALSE);	//�رմ���
		m_static_status = "�ѹر�";
		bOpen = FALSE;
		UpdateData(FALSE);
		return;
	}
	
	m_mscomm1.put_CommPort(m_comindex.GetCurSel() + 1);	//�趨���ں�
	m_mscomm1.put_RThreshold(1);		//ÿ���յ�һ���ַ�ʱ������OnComm�¼�
	m_mscomm1.put_InBufferSize(2048);	//�������뻺������СΪ2048 byte	

	if (!m_mscomm1.get_PortOpen())
		m_mscomm1.put_PortOpen(TRUE);	//�򿪴���	
	
	CString str;
	str = m_baudrate + ",n," + m_databit + "," + m_stopbit;
	m_mscomm1.put_Settings(str);	//������m_baudrate����У�飬m_databit����λ��m_stopbitֹͣλ
	m_mscomm1.put_InputMode(1);		// �Զ����Ʒ�ʽ��ȡ����
	m_mscomm1.put_RThreshold(1);	//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
	m_mscomm1.put_InputLen(0);		//���õ�ǰ���������ݳ���Ϊ0
	m_mscomm1.get_Input();			//��Ԥ���������������������
	bOpen = TRUE;

	m_static_status = "�Ѵ�";
	UpdateData(FALSE);
}

BEGIN_EVENTSINK_MAP(CSSCOMDlg, CDialogEx)
	ON_EVENT(CSSCOMDlg, IDC_MSCOMM1, 1, CSSCOMDlg::OnOncommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

void CSSCOMDlg::OnOncommMscomm1()
{
	// TODO:  �ڴ˴������Ϣ����������
	CString strtemp;
	if (m_mscomm1.get_CommEvent() == 2)				//�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		ole_variant.Attach(m_mscomm1.get_Input());
		safearray_inp = ole_variant;				//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		com_len = safearray_inp.GetOneDimSize();	//�õ���Ч���ݳ���

		for (com_k = 0; com_k<com_len; com_k++)		//ת��ΪBYTE������
		{
			safearray_inp.GetElement(&com_k, rxbuf + buf_count);		
			buf_count++;
		}
		
		for (com_k = 0; com_k < com_len; com_k++)	//������ת��ΪCstring�ͱ��� 
		{
			BYTE bt = *(char*)(rxbuf + com_k + 1);	//���ַ�������ʱ����strtemp���  
			strtemp.Format(_T("%c"), bt);			//������ձ༭���Ӧ�ַ���   
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
		str = "���� 0x";
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
		str = "���� ASCII ";
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
		str = " ���� 0x";
		for (int i = 0; i < com_len ; i++)
			str = str + Dec_2HEX_2bit(buf[i+1]);
	}

	if (m_dataformat.GetCurSel() == 0)
	{
		str = " ���� ASCII ";
		str = str + ASCIIRx;
	}

	str_time = str_time + str;
	m_list.InsertString(-1,str_time);
	m_list.SetCurSel(m_list.GetCount() - 1);
}


void CSSCOMDlg::OnClickedButtonClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_edit.SetWindowText(_T(""));
}


void CSSCOMDlg::OnClickedButtonSend()
{
	//// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			noinput = "���������ݣ������ʲô��";
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
		MessageBox(_T("           �㴮�ڿ��ˣ�"));
		WindowTextArray.RemoveAll();
		varOutput.Clear();
		return;
	}
}


void CSSCOMDlg::OnBnClickedCheckHex()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CSSCOMDlg::OnClickedButtonClearlist()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_list.ResetContent();
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

