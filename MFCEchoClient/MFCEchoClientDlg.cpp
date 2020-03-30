// MFCEchoClientDlg.cpp : implementation file
//
#include <ctime>
#include <iostream>
#include <string>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>
#include <chrono>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <map>

#include "stdafx.h"
#include "MFCEchoClient.h"
#include "MFCEchoClientDlg.h"
#include "afxdialogex.h"
#include "pch.h"
#include "Card.h"
#include "Deck.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace SuitNamespace;
using namespace FaceNamespace;

enum { max_length = 1024 };

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

wchar_t *GetWC(const char *c, size_t length) {
	wchar_t* wc = new wchar_t[length-2];
	mbstowcs(wc, c, length-2);
	return wc;
}

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
END_MESSAGE_MAP()


// CMFCEchoClientDlg dialog



CMFCEchoClientDlg::CMFCEchoClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCECHOCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCEchoClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCEchoClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(ipAddressText, &CMFCEchoClientDlg::OnStnClickedipaddresstext)
	ON_BN_CLICKED(connectBtn, &CMFCEchoClientDlg::OnBnClickedconnectbtn)
END_MESSAGE_MAP()


// CMFCEchoClientDlg message handlers

BOOL CMFCEchoClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCEchoClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCEchoClientDlg::OnPaint()
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
HCURSOR CMFCEchoClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCEchoClientDlg::OnStnClickedipaddresstext()
{
	// TODO: Add your control notification handler code here
}


void CMFCEchoClientDlg::OnBnClickedconnectbtn()
{

	//LPTSTR text = new TCHAR[25];
	CString text;
	GetDlgItemText(IpAddressTbox, text);

	Deck deck = Deck();
	try
	{
		string port = "300";
		//string ip = "localhost";
		string ip(CW2A(text.GetString()));

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), ip, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		tcp::socket s(io_service);
		boost::asio::connect(s, iterator);

		int i = 0;
		//while (i < 5)
		//{
			char request[max_length] = "Conection Sucess";
			size_t request_length = strlen(request);
			boost::asio::write(s, boost::asio::buffer(request, request_length));

			char reply[max_length];
			size_t reply_length = boost::asio::read(s,
				boost::asio::buffer(reply, request_length));
		//	i++;
		//} 
			enum { t = reply_length };
			char newReply[t] = reply[reply_length];
			LPTSTR testReply = GetWC(newReply, reply_length);
			//if (reply == "hello") {
				SetDlgItemText(tboxReply, testReply);
			//}
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}
}

