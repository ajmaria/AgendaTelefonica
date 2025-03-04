
// AgendaTelefonicaDlg.h : header file
//

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include <string>
#include <afxdb.h>
#include <afx.h>
#include "resource.h"

// CAgendaTelefonicaDlg dialog
class CAgendaTelefonicaDlg : public CDialogEx
{
	// Construction
public:
	CAgendaTelefonicaDlg(CWnd* pParent = nullptr);	// standard constructor
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAdauga();
	void AdaugaContact(CString nume, CString prenume, CString telefon, CString email);
	afx_msg void OnBnClickedOk();
	void AfiseazaContacte();
	afx_msg void OnBnClickedSterge();

	int ObtineContactID(const CString& strNume, const CString& strPrenume);
	afx_msg void OnLvnItemchangedListContacte(NMHDR* pNMHDR, LRESULT* pResult);
	void ModificaContact(int nItem, const CString& nume, const CString& prenume, const CString& telefon, const CString& email);
	bool isDigit(const CString& str);
	void OnBnClickedSalveazaModificari();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGENDATELEFONICA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;
	CListCtrl m_listCtrl;
	int m_nSelectedItem;

	afx_msg void OnBnClickedModifica();
	afx_msg void OnBnClickedCautaContact();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
