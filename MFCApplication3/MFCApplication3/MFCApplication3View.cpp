
// MFCApplication3View.cpp: CMFCApplication3View 클래스의 구현
//

#include "stdafx.h"
#include "resource.h"
#include "Resource.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"
#include "MFCApplication3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include"User.h"
#include"Message.h"
#include <stdlib.h>
#include"openssl/bn.h"
#include"openssl/sha.h"
#include"openssl/aes.h"
#include<time.h>
#include<Windows.h>
#include"openssl/dh.h"
#include"openssl/engine.h"

User *user[10];
int temp = sizeof(user);
User *login_user;
bool login_state = FALSE;

Message *msg[100];
int msg_temp = sizeof(msg);

int idx;

// CMFCApplication3View

IMPLEMENT_DYNCREATE(CMFCApplication3View, CFormView)

BEGIN_MESSAGE_MAP(CMFCApplication3View, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CMFCApplication3View::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_JOIN, &CMFCApplication3View::OnBnClickedButtonJoin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CMFCApplication3View::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCApplication3View::OnBnClickedButtonSend)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MSG1, &CMFCApplication3View::OnNMClickListMsg1)
	ON_BN_CLICKED(IDC_LIST_BUTTON, &CMFCApplication3View::OnBnClickedListButton)
END_MESSAGE_MAP()

// CMFCApplication3View 생성/소멸

CMFCApplication3View::CMFCApplication3View()
	: CFormView(IDD_MFCAPPLICATION3_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCApplication3View::~CMFCApplication3View()
{
}

void CMFCApplication3View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ID, m_combo_id);
	DDX_Control(pDX, IDC_LOGIN_PW, m_login_pw);
	DDX_Control(pDX, IDC_JOIN_ID, m_join_id);
	DDX_Control(pDX, IDC_JOIN_PW1, m_join_pw1);
	DDX_Control(pDX, IDC_JOIN_PW2, m_join_pw2);
	DDX_Control(pDX, IDC_LIST_MSG1, m_list_msg1);
}

BOOL CMFCApplication3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMFCApplication3View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_list_msg1.InsertColumn(0, _T("Title"), NULL, 300);
	m_list_msg1.InsertColumn(1, _T("Sender"), NULL, 100);
	m_list_msg1.InsertColumn(2, _T("Receiver"), NULL, 100);
	
}


// CMFCApplication3View 인쇄

BOOL CMFCApplication3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCApplication3View::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CMFCApplication3View 진단

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication3View 메시지 처리기


void CMFCApplication3View::OnBnClickedButtonLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString input_id, input_pw;
	GetDlgItemText(IDC_LOGIN_ID, input_id);
	GetDlgItemText(IDC_LOGIN_PW, input_pw);
	

	for (int i = 0; i < sizeof(user); i++)
	{
		if (user[i]->id == input_id) 
		{
			login_user = user[i];

			if (login_user->login(input_pw))
			{
				login_state = TRUE;
				GetDlgItem(IDC_LOGIN_ID)->EnableWindow(FALSE);
				GetDlgItem(IDC_LOGIN_PW)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_JOIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(TRUE);
				GetDlgItem(IDC_JOIN_ID)->EnableWindow(FALSE);
				GetDlgItem(IDC_JOIN_PW1)->EnableWindow(FALSE);
				GetDlgItem(IDC_JOIN_PW2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
				SetDlgItemText(IDC_LOGIN_PW, _T(""));
				MessageBox(_T("Success!"));
				break;
			}
			else
			{
				MessageBox(_T("Check your Password !"), _T("error"), MB_ICONERROR);
				SetDlgItemText(IDC_LOGIN_PW, _T(""));
				break;
			}
		}
		
	}
}


void CMFCApplication3View::OnBnClickedButtonJoin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString id, pw1, pw2;
	GetDlgItemText(IDC_JOIN_ID, id);
	GetDlgItemText(IDC_JOIN_PW1, pw1);
	GetDlgItemText(IDC_JOIN_PW2, pw2);

	if (pw1 == pw2 && ((pw1, pw2, id).IsEmpty()==FALSE) && (temp>0)) {
		user[sizeof(user) - temp] = new User();
		user[sizeof(user) - temp]->join(id, pw1);
		
		SetDlgItemText(IDC_LOGIN_ID, id);
		temp--;
		m_combo_id.AddString(id);
		MessageBox(_T("Success!"));
		SetDlgItemText(IDC_JOIN_ID, _T(""));
		SetDlgItemText(IDC_JOIN_PW1, _T(""));
		SetDlgItemText(IDC_JOIN_PW2, _T(""));
		//SetDlgItemText(IDC_LIST_CONTENT,CStringA(user[sizeof(user) - temp]->pw));
	}
	else {
		MessageBox(_T("Fail !"), _T("error"), MB_ICONERROR);
		SetDlgItemText(IDC_JOIN_PW1, _T(""));
		SetDlgItemText(IDC_JOIN_PW2, _T(""));
	};
}


void CMFCApplication3View::OnBnClickedButtonLogout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	login_state = FALSE;

	GetDlgItem(IDC_LOGIN_ID)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOGIN_PW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_JOIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_JOIN_ID)->EnableWindow(TRUE);
	GetDlgItem(IDC_JOIN_PW1)->EnableWindow(TRUE);
	GetDlgItem(IDC_JOIN_PW2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOGIN_PW)->EnableWindow(TRUE);
	SetDlgItemText(IDC_LOGIN_ID, _T(""));
	MessageBox(_T("Good bye!"));

}


void CMFCApplication3View::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (msg_temp>0)
	{
		CString sendid, receiveid, title, content;
		Message *sendmsg;

		m_combo_id.GetLBText(m_combo_id.GetCurSel(), receiveid);

		User *receive_user;
		GetDlgItemText(IDC_LOGIN_ID, sendid);
		GetDlgItemText(IDC_EDIT_TITLE, title);
		GetDlgItemText(IDC_EDIT_CONTENT, content);
		
		for (int i = 0; i < sizeof(user); i++)
		{
			if (user[i]->id == receiveid) {
				receive_user = user[i];
				break;
			}
		}
	
		msg[sizeof(msg) - msg_temp] = new Message;
		sendmsg = msg[sizeof(msg) - msg_temp];
		sendmsg->send(login_user, receive_user,title,content);

		m_list_msg1.InsertItem(sizeof(msg) - msg_temp, title);
		m_list_msg1.SetItemText(sizeof(msg) - msg_temp, 1, sendid);
		m_list_msg1.SetItemText(sizeof(msg) - msg_temp, 2, receiveid);
		
		SetDlgItemText(IDC_LIST_CONTENT, sendmsg -> M_content);

		msg_temp--;
		
		MessageBox(_T("Send message!"));
	}
	else
	{
		MessageBox(_T("Maximum msg Box"));
	}
	



}


void CMFCApplication3View::OnNMClickListMsg1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	idx = pNMListView->iItem;
	CString receiver;

	SetDlgItemText(IDC_LIST_CONTENT, msg[idx]->M_content);
	SetDlgItemText(IDC_LIST_CONTENT2,_T(""));
	*pResult = 0;
}


void CMFCApplication3View::OnBnClickedListButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString receiver;
	User *receive_user;

	receiver = m_list_msg1.GetItemText(idx, 2);
	for (int i = 0; i < sizeof(user); i++)
	{
		if (user[i]->id == receiver) {
			receive_user = user[i];
			break;
		}
	}

	msg[idx]->lookup(login_user, receive_user);
	SetDlgItemText(IDC_LIST_CONTENT2, msg[idx]->M_dec);

}
