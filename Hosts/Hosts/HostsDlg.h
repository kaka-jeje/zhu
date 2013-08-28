
// HostsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <string>


// CHostsDlg �Ի���
class CHostsDlg : public CDialog
{
// ����
public:
	CHostsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOSTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void WriteHostFile(std::vector<std::string>& v_String, char *filePath);
	void WriteHostFile1(std::vector<std::string>& v_String, CString filePath);
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	CButton m_btnOk;
	CButton m_btnCancel;
	afx_msg void OnBnClickedButtonAdd();
};
