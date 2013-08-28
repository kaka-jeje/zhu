
// HostsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <string>


// CHostsDlg 对话框
class CHostsDlg : public CDialog
{
// 构造
public:
	CHostsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HOSTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
