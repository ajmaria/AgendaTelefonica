
// AgendaTelefonicaDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "AgendaTelefonica.h"
#include "AgendaTelefonicaDlg.h"
#include "afxdialogex.h"
#include <afxdb.h>
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CAgendaTelefonicaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADAUGA, &CAgendaTelefonicaDlg::OnBnClickedAdauga)
	ON_BN_CLICKED(IDOK, &CAgendaTelefonicaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STERGE, &CAgendaTelefonicaDlg::OnBnClickedSterge)
END_MESSAGE_MAP()


// CAgendaTelefonicaDlg dialog



CAgendaTelefonicaDlg::CAgendaTelefonicaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGENDATELEFONICA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSelectedItem = -1;
}

void CAgendaTelefonicaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// Leaga controlul IDC_LIST_CONTACTE la variabila m_listCtrl
	DDX_Control(pDX, IDC_LIST_CONTACTE, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CAgendaTelefonicaDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONTACTE, &CAgendaTelefonicaDlg::OnLvnItemchangedListContacte)
	ON_BN_CLICKED(IDC_BUTTON_ADAUGA, &CAgendaTelefonicaDlg::OnBnClickedAdauga)
	ON_BN_CLICKED(IDC_BUTTON_STERGE, &CAgendaTelefonicaDlg::OnBnClickedSterge)
	ON_BN_CLICKED(IDC_BUTTON_MODIFICA, &CAgendaTelefonicaDlg::OnBnClickedModifica)
	ON_BN_CLICKED(IDC_BUTTON_CAUTA_CONTACT, &CAgendaTelefonicaDlg::OnBnClickedCautaContact)
END_MESSAGE_MAP()


// CAgendaTelefonicaDlg message handlers

BOOL CAgendaTelefonicaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// Initializare coloane pentru lista
	m_listCtrl.InsertColumn(0, _T("Nume"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("Prenume"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(2, _T("Telefon"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, _T("Email"), LVCFMT_LEFT, 150);

	AfiseazaContacte();

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	return TRUE;
}

void CAgendaTelefonicaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAgendaTelefonicaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAgendaTelefonicaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAgendaTelefonicaDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CAgendaTelefonicaDlg::OnBnClickedAdauga()
{

	//Valorile din controalele de intrare
	CString strNume, strPrenume, strTelefon, strEmail;

	//Textul din controalele de editare
	GetDlgItemText(IDC_EDIT_NUME, strNume);
	GetDlgItemText(IDC_EDIT_PRENUME, strPrenume);
	GetDlgItemText(IDC_EDIT_TELEFON, strTelefon);
	GetDlgItemText(IDC_EDIT_EMAIL, strEmail);

	// Validare: campurile sunt completate
	if (strNume.IsEmpty() || strPrenume.IsEmpty() || strTelefon.IsEmpty() || strEmail.IsEmpty())
	{
		AfxMessageBox(_T("Toate campurile trebuie completate!"));
		return;
	}

	// Adauga contactul in baza de date cu valorile obtinute
	AdaugaContact(strNume, strPrenume, strTelefon, strEmail);

	// Afisare lista de contacte cu noul contact
	AfiseazaContacte();

	// Stergere date din campurile de editare
	SetDlgItemText(IDC_EDIT_NUME, _T(""));
	SetDlgItemText(IDC_EDIT_PRENUME, _T(""));
	SetDlgItemText(IDC_EDIT_TELEFON, _T(""));
	SetDlgItemText(IDC_EDIT_EMAIL, _T(""));
}

void CAgendaTelefonicaDlg::AdaugaContact(CString nume, CString prenume, CString telefon, CString email)
{
	CDatabase database;
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	// Deschide conexiunea la baza de date
	if (!database.OpenEx(sDsn, CDatabase::noOdbcDialog))
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
		return;
	}

	// Interogarea SQL
	CString sql;
	sql.Format(_T("INSERT INTO Contacte (Nume, Prenume, Telefon, Email) VALUES ('%s', '%s', '%s', '%s');"),
		nume, prenume, telefon, email);


	try
	{
		// Executa interogarea SQL pentru a adauga contactul
		database.ExecuteSQL(sql);

		// Afisare mesaj
		AfxMessageBox(_T("Contact adaugat cu succes!"));
	}
	catch (CDBException* e)
	{
		// Erori in cazul in care exista o exceptie
		AfxMessageBox(_T("Eroare la adaugarea contactului!"));
		e->Delete();
	}

	// Inchidere conexiune la baza de date
	database.Close();
}

void CAgendaTelefonicaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void CAgendaTelefonicaDlg::AfiseazaContacte()
{
	// Sterge toate elementele vechi din lista
	m_listCtrl.DeleteAllItems();

	CDatabase database;
	CRecordset recordset(&database);
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	if (database.OpenEx(sDsn, CDatabase::noOdbcDialog))
	{
		// Deschide recordset-ul pentru a obtine contactele
		recordset.Open(CRecordset::forwardOnly, _T("SELECT Nume, Prenume, Telefon, Email FROM Contacte;"));

		while (!recordset.IsEOF())
		{
			CString nume, prenume, telefon, email;
			recordset.GetFieldValue(_T("Nume"), nume);
			recordset.GetFieldValue(_T("Prenume"), prenume);
			recordset.GetFieldValue(_T("Telefon"), telefon);
			recordset.GetFieldValue(_T("Email"), email);

			// Adauga un element in CListCtrl pentru fiecare contact
			int index = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), nume);
			m_listCtrl.SetItemText(index, 1, prenume); 
			m_listCtrl.SetItemText(index, 2, telefon); 
			m_listCtrl.SetItemText(index, 3, email); 

			recordset.MoveNext();
		}
	}
	else
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
	}
}

void CAgendaTelefonicaDlg::OnBnClickedSterge()
{
	// Obtine indexul elementului selectat
	int selIndex = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	// Verifica daca exista un contact selectat
	if (selIndex == -1) {
		AfxMessageBox(_T("Nu a fost selectat niciun contact pentru stergere."));
		return;
	}

	// Obtine informatiile despre contactul selectat
	CString nume = m_listCtrl.GetItemText(selIndex, 0);
	CString prenume = m_listCtrl.GetItemText(selIndex, 1);
	CString telefon = m_listCtrl.GetItemText(selIndex, 2);
	CString email = m_listCtrl.GetItemText(selIndex, 3);

	// Confirmare inainte de stergere
	int response = AfxMessageBox(_T("Sigur vrei sa stergi acest contact?"), MB_YESNO);

	if (response == IDYES) {
		// Conexiune la baza de date
		CDatabase database;
		CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

		try {
			// Deschide conexiunea la baza de date
			if (database.OpenEx(sDsn, CDatabase::noOdbcDialog)) {
				// Interogarea SQL pentru stergerea contactului
				CString query;
				query.Format(_T("DELETE FROM Contacte WHERE Nume='%s' AND Prenume='%s' AND Telefon='%s' AND Email='%s'"),
					nume, prenume, telefon, email);

				// Executa interogarea SQL
				database.ExecuteSQL(query);

				// Elimina contactul din lista
				m_listCtrl.DeleteItem(selIndex);

				// Afiseaza un mesaj de succes
				AfxMessageBox(_T("Contactul a fost sters cu succes."));
			}
			else {
				AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
			}
		}
		catch (CException* e) {
			TCHAR szErr[1024];
			e->GetErrorMessage(szErr, 1024);
			// Afiseaza mesajul de eroare in caz de exceptie
			AfxMessageBox(szErr);
		}
	}
}

int CAgendaTelefonicaDlg::ObtineContactID(const CString& strNume, const CString& strPrenume)
{
	CDatabase database;
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	// Deschide conexiunea la baza de date
	if (database.OpenEx(sDsn, CDatabase::noOdbcDialog) != TRUE)
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
		return -1;
	}

	try
	{
		// Interogarea SQL pentru a cauta contactul dupa nume si prenume
		CString query;
		query.Format(_T("SELECT ID FROM Contacte WHERE Nume = '%s' AND Prenume = '%s'"), strNume, strPrenume);

		// Creeaza un recordset pentru a executa interogarea
		CRecordset rs(&database);
		rs.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);

		// Verifica daca exista un rezultat
		if (!rs.IsEOF())
		{
			CString strID;
			rs.GetFieldValue(_T("ID"), strID);  // Obtine valoarea ID ca un CString
			rs.Close();
			database.Close();
			return _ttoi(strID);  // Convertire ID-ul la int
		}
		else
		{
			AfxMessageBox(_T("Contactul nu a fost gasit!"));
			rs.Close();
			database.Close();
			return -1;
		}
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("Eroare la executarea interogarii SQL!"));
		e->Delete();
		database.Close();
		return -1;
	}
}

void CAgendaTelefonicaDlg::OnLvnItemchangedListContacte(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMListView = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

	// Verificare daca elementul a fost selectat
	if (pNMListView->uNewState & LVIS_SELECTED) {
		// Obtine indexul elementului selectat
		int nItem = pNMListView->iItem;

		// Obtine informatiile din contactul selectat
		CString strNume = m_listCtrl.GetItemText(nItem, 0);  
		CString strPrenume = m_listCtrl.GetItemText(nItem, 1);  
		CString strTelefon = m_listCtrl.GetItemText(nItem, 2);  
		CString strEmail = m_listCtrl.GetItemText(nItem, 3);  

		// Populam campurile de editare cu datele contactului selectat
		SetDlgItemText(IDC_EDIT_NUME, strNume);
		SetDlgItemText(IDC_EDIT_PRENUME, strPrenume);
		SetDlgItemText(IDC_EDIT_TELEFON, strTelefon);
		SetDlgItemText(IDC_EDIT_EMAIL, strEmail);

		// Salvare indexul contactului selectat pentru actualizare
		m_nSelectedItem = nItem;
	}

	*pResult = 0; 
}

void CAgendaTelefonicaDlg::ModificaContact(int nItem, const CString& nume, const CString& prenume, const CString& telefon, const CString& email)
{
	CDatabase database;
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	// Deschide conexiunea la baza de date
	if (!database.OpenEx(sDsn, CDatabase::noOdbcDialog))
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
		return;
	}

	// Interogarea SQL pentru a actualiza contactul
	CString sql;
	sql.Format(_T("UPDATE Contacte SET Nume='%s', Prenume='%s', Telefon='%s', Email='%s' WHERE ID=%d;"),
		nume, prenume, telefon, email, m_nSelectedItem);  // Presupunem că ID-ul contactului este stocat în `m_nSelectedItem`

	try
	{
		// Executa interogarea SQL pentru a actualiza contactul
		database.ExecuteSQL(sql);

		// Afiseaza un mesaj de succes
		AfxMessageBox(_T("Contactul a fost actualizat cu succes!"));
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Eroare la actualizarea contactului!"));
		e->Delete();
	}

	// Inchide conexiunea
	database.Close();
}

// Functie care verifica daca un sir de caractere contine doar cifre
bool CAgendaTelefonicaDlg::isDigit(const CString& str)
{
	for (int i = 0; i < str.GetLength(); ++i)
	{
		if (!_istdigit(str[i])) 
			return false;
	}
	return true; // Returneaza true daca toate caracterele sunt cifre
}

void CAgendaTelefonicaDlg::OnBnClickedSalveazaModificari()
{
	CString strNume, strPrenume, strTelefon, strEmail;

	// Obtine datele din campurile de editare
	GetDlgItemText(IDC_EDIT_NUME, strNume);
	GetDlgItemText(IDC_EDIT_PRENUME, strPrenume);
	GetDlgItemText(IDC_EDIT_TELEFON, strTelefon);
	GetDlgItemText(IDC_EDIT_EMAIL, strEmail);

	if (strNume.IsEmpty() || strPrenume.IsEmpty() || strTelefon.IsEmpty() || strEmail.IsEmpty())
	{
		AfxMessageBox(_T("Toate campurile trebuie completate!"));
		return;
	}

	// Actualizeaza contactul in baza de date
	ModificaContact(m_nSelectedItem, strNume, strPrenume, strTelefon, strEmail);

	AfiseazaContacte();

	// Sterge campurile de editare
	SetDlgItemText(IDC_EDIT_NUME, _T(""));
	SetDlgItemText(IDC_EDIT_PRENUME, _T(""));
	SetDlgItemText(IDC_EDIT_TELEFON, _T(""));
	SetDlgItemText(IDC_EDIT_EMAIL, _T(""));
}

void CAgendaTelefonicaDlg::OnBnClickedModifica()
{
	// Verifica daca un contact este selectat
	int selIndex = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);  // Obtine indexul elementului selectat

	if (selIndex == -1) {
		AfxMessageBox(_T("Nu a fost selectat niciun contact pentru modificare."));
		return;
	}

	// Obtine valorile din campurile de editare
	CString strNume, strPrenume, strTelefon, strEmail;
	GetDlgItemText(IDC_EDIT_NUME, strNume);       
	GetDlgItemText(IDC_EDIT_PRENUME, strPrenume); 
	GetDlgItemText(IDC_EDIT_TELEFON, strTelefon); 
	GetDlgItemText(IDC_EDIT_EMAIL, strEmail);     

	if (strNume.IsEmpty() || strPrenume.IsEmpty() || strTelefon.IsEmpty() || strEmail.IsEmpty()) {
		AfxMessageBox(_T("Toate campurile trebuie completate!"));
		return;
	}

	CDatabase database;
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	// Deschide conexiunea la baza de date
	if (!database.OpenEx(sDsn, CDatabase::noOdbcDialog))
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
		return;
	}

		// Citeste numele si prenumele de la indexul selectat
	const CString strNume1 = m_listCtrl.GetItemText(selIndex, 0);   
	const CString strPrenume1 = m_listCtrl.GetItemText(selIndex, 1); 

	int contactID = ObtineContactID(strNume1, strPrenume1);

	CString strSQL;
	strSQL.Format(_T("UPDATE Contacte SET Nume='%s', Prenume='%s', Telefon='%s', Email='%s' WHERE ID=%d"),
		strNume, strPrenume, strTelefon, strEmail, ObtineContactID(strNume1, strPrenume1));

	try {
		// Executa interogarea
		database.ExecuteSQL(strSQL);
		AfxMessageBox(_T("Contactul a fost actualizat in baza de date."));
	}
	catch (CException* e) {
		AfxMessageBox(_T("Eroare la actualizarea bazei de date."));
		e->Delete();
	}

	// Inlocuieste contactul in lista
	m_listCtrl.SetItemText(selIndex, 0, strNume);       
	m_listCtrl.SetItemText(selIndex, 1, strPrenume);    
	m_listCtrl.SetItemText(selIndex, 2, strTelefon);    
	m_listCtrl.SetItemText(selIndex, 3, strEmail);      

	// Goleste campurile de editare
	SetDlgItemText(IDC_EDIT_NUME, _T(""));
	SetDlgItemText(IDC_EDIT_PRENUME, _T(""));
	SetDlgItemText(IDC_EDIT_TELEFON, _T(""));
	SetDlgItemText(IDC_EDIT_EMAIL, _T(""));

	// Inchide conexiunea la baza de date
	database.Close();
}

void CAgendaTelefonicaDlg::OnBnClickedCautaContact()
{
	// Obtine textul introdus de utilizator
	CString searchText;
	GetDlgItemText(IDC_EDIT_CAUTA, searchText);
	searchText.Trim();

	// Verifica daca a fost introdus un text de cautare
	if (searchText.IsEmpty())
	{
		AfxMessageBox(_T("Introduceti un criteriu de cautare!"));
		return;
	}

	// Deschide conexiunea la baza de date
	CDatabase database;
	CString sDsn = _T("DRIVER={SQL Server};SERVER=localhost;DATABASE=AgendaTelefonica;Trusted_Connection=Yes;");

	if (database.OpenEx(sDsn, CDatabase::noOdbcDialog) != TRUE)
	{
		AfxMessageBox(_T("Eroare la conectarea la baza de date!"));
		return;
	}

	try
	{
		// Interogarea SQL pentru cautare
		CString query;

		if (isDigit(searchText))  // Verifica daca textul cautat este un numar
		{
			query.Format(_T("SELECT ID, Nume, Prenume, Telefon, Email FROM Contacte "
				"WHERE ID = '%s' OR Telefon LIKE '%%%s%%'"),
				searchText, searchText);
		}
		else
		{
			CString searchTextNormalized = searchText;
			searchTextNormalized.MakeLower();  // Convertire text

			// Interogarea SQL
			query.Format(_T("SELECT ID, Nume, Prenume, Telefon, Email FROM Contacte "
				"WHERE LOWER(Nume) LIKE '%%%s%%' OR LOWER(Prenume) LIKE '%%%s%%' OR "
				"Telefon LIKE '%%%s%%' OR Email LIKE '%%%s%%'"),
				searchTextNormalized, searchTextNormalized, searchText, searchText);
		}

		// Executa interogarea
		CRecordset rs(&database);
		rs.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);

		// Verifica daca exista rezultate
		if (!rs.IsEOF())
		{
			CString result;
			result = _T("Contacte gasite:\n\n");

			while (!rs.IsEOF())
			{
				CString strID, strNume, strPrenume, strTelefon, strEmail;
				rs.GetFieldValue(_T("ID"), strID);
				rs.GetFieldValue(_T("Nume"), strNume);
				rs.GetFieldValue(_T("Prenume"), strPrenume);
				rs.GetFieldValue(_T("Telefon"), strTelefon);
				rs.GetFieldValue(_T("Email"), strEmail);

				CString contactInfo;
				contactInfo.Format(_T("ID: %s\nNume: %s\nPrenume: %s\nTelefon: %s\nEmail: %s\n\n"),
					strID, strNume, strPrenume, strTelefon, strEmail);

				result += contactInfo;

				rs.MoveNext();
			}

			AfxMessageBox(result);
		}
		else
		{
			AfxMessageBox(_T("Niciun contact nu a fost gasit!"));
		}

		rs.Close();
		database.Close();
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("Eroare la executarea interogarii SQL!"));
		e->Delete();
		database.Close();
	}
}





