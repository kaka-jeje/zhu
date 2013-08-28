
// HostsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Hosts.h"
#include "HostsDlg.h"
#include "opstr.h"
#include "bfcstr.h"
#include <fstream>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostsDlg �Ի���




CHostsDlg::CHostsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHostsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHostsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CHostsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OK, &CHostsDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CHostsDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CHostsDlg::OnBnClickedButtonAdd)
END_MESSAGE_MAP()


// CHostsDlg ��Ϣ�������

BOOL CHostsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHostsDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHostsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHostsDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *filePath = "C:\\Windows\\System32\\drivers\\etc\\hosts";
	char buff[1024];
	std::vector<std::string>  v_string;
	v_string.clear();

	ifstream file(filePath);
	if(file.is_open())
	{
		while(!file.eof())
		{
			memset(buff,0,1024);
			file.getline(buff,1024);
			//v_string.push_back(buff);
			if(buff[0] == '#' && isdigit(buff[1]))
			{	
				std::string strTmp;
				strTmp = &buff[1];
				strTmp += "\n";
				v_string.push_back(strTmp);
			}
			else
			{
				std::string strTmp;
				strTmp = buff;
				strTmp += "\n";
				v_string.push_back(strTmp);
			}
		}
	}
	file.close();
	WriteHostFile(v_string, filePath);
	::MessageBox(this->GetSafeHwnd(), _T("�޸���ɣ�"), _T("host�޸�"), MB_OK);
}

void CHostsDlg::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *filePath = "C:\\Windows\\System32\\drivers\\etc\\hosts";
	char buff[1024];
	std::vector<std::string>  v_string;
	v_string.clear();

	ifstream file(filePath);
	if(file.is_open())
	{
		while(!file.eof())
		{
			memset(buff,0,1024);
			file.getline(buff,1024);
			//v_string.push_back(buff);
			if(isdigit(buff[0]))
			{
				std::string strTmp;
				strTmp = "#";
				strTmp += buff;
				strTmp += "\n";
				v_string.push_back(strTmp);
			}
			else
			{
				std::string strTmp;
				strTmp = buff;
				strTmp += "\n";
				v_string.push_back(strTmp);
			}
		}
	}
	file.close();
	WriteHostFile(v_string, filePath);
	::MessageBox(this->GetSafeHwnd(), _T("�޸���ɣ�"), _T("host�޸�"), MB_OK);
}

void CHostsDlg::WriteHostFile(std::vector<std::string>& v_String, char *filePath)
{
	fstream file;
	file.open(filePath, ios::out|ios::trunc);
	if(file.is_open())
	{
		for(int i = 0; i < (int)v_String.size(); i++)
		{
			//fwrite(v_String[i].c_str(), 1, v_String[i].size(), fp);
			if( i == v_String.size() -1)
			{
				file.write(v_String[i].c_str(), v_String[i].size() - 1);
			}
			else
			{
				file.write(v_String[i].c_str(), v_String[i].size());
			}
		}
	}
	file.close();
}

void CHostsDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPath =  GetModulePathName();
	char buff[1024];
	std::vector<std::string>  v_string;
	strPath += _T("config.txt");

	CString filePath = _T("C:\\Windows\\System32\\drivers\\etc\\hosts");
	/*ifstream file0(filePath);
	if(file0.is_open())
	{
		while(!file0.eof())
		{
			memset(buff,0,1024);
			file0.getline(buff,1024);
			std::string strTmp;
			strTmp = buff;
			strTmp += "\n";
			v_string.push_back(strTmp);
		}
	}
	file0.close();*/

	ifstream file(strPath);
	if(file.is_open())
	{
		while(!file.eof())
		{
			memset(buff,0,1024);
			file.getline(buff,1024);
			std::string strTmp;
			strTmp = buff;
			strTmp += "\n";
			v_string.push_back(strTmp);
		}
	}
	file.close();
	WriteHostFile1(v_string, filePath);
}

void CHostsDlg::WriteHostFile1(std::vector<std::string>& v_String, CString filePath)
{
	fstream file;
	file.open(filePath, ios::out|ios::app);
	if(file.is_open())
	{
		file.write("\n", 1);
		for(int i = 0; i < (int)v_String.size(); i++)
		{
			//fwrite(v_String[i].c_str(), 1, v_String[i].size(), fp);
			if( i == v_String.size() -1)
			{
				file.write(v_String[i].c_str(), v_String[i].size() - 1);
			}
			else
			{
				file.write(v_String[i].c_str(), v_String[i].size());
			}
		}
		::MessageBox(this->GetSafeHwnd(), _T("�޸ĳɹ�"), _T("host�޸�"), MB_OK);
	}
	else
	{
		::MessageBox(this->GetSafeHwnd(), _T("�޸�ʧ�ܣ�û���ҵ������ļ���"), _T("host�޸�"), MB_OK);
	}
	file.close();
}
