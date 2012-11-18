// KerRulePage.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "KerRulePage.h"

#include "KerRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKerRulePage property page

IMPLEMENT_DYNCREATE(CKerRulePage, CPropertyPage)

CKerRulePage::CKerRulePage() : CPropertyPage(CKerRulePage::IDD)
{
	//{{AFX_DATA_INIT(CKerRulePage)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKerRulePage::~CKerRulePage()
{
}

void CKerRulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKerRulePage)
	DDX_Control(pDX, IDC_KERRULES, m_kerrules);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKerRulePage, CPropertyPage)
	//{{AFX_MSG_MAP(CKerRulePage)
	ON_BN_CLICKED(IDC_KERADD, OnKeradd)
	ON_BN_CLICKED(IDC_KERDEL, OnKerdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKerRulePage message handlers

BOOL CKerRulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// init the windows text
	m_kerrules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_kerrules.InsertColumn(0, L"ԴIP��ַ", LVCFMT_LEFT, sizeof(L"ԴIP��ַ")*8, 0);
	m_kerrules.InsertColumn(1, L"Դ����λ", LVCFMT_LEFT, sizeof( L"Դ����λ")*8, 1);
	m_kerrules.InsertColumn(2, L"Դ�˿�", LVCFMT_LEFT, sizeof(L"Դ�˿�")*8, 2);
	m_kerrules.InsertColumn(3, L"Ŀ��IP��ַ", LVCFMT_LEFT, sizeof(L"ĿIP��ַ")*8, 3);
	m_kerrules.InsertColumn(4, L"Ŀ������λ", LVCFMT_LEFT, sizeof(L"Ŀ����λ")*8, 4);
	m_kerrules.InsertColumn(5, L"Ŀ�Ķ˿�", LVCFMT_LEFT, sizeof(L"Ŀ�˿�")*8, 5);
	m_kerrules.InsertColumn(6, L"Э��", LVCFMT_LEFT, sizeof(L"Э��")*8, 6);
	m_kerrules.InsertColumn(7, L"����", LVCFMT_LEFT, sizeof(L"����")*8, 7);

	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CKerRulePage::UpdateList()
{
	// clear the rule items
	m_kerrules.DeleteAllItems();
	// ���б�����ӹ���
	for(int i=0; i<(int)g_RuleFile.m_header.ulKerRuleCount; i++)
		AddRuleToList(&g_RuleFile.m_pKerRules[i]);
}

void CKerRulePage::AddRuleToList(PPassthruFilter pItem, int nEditIndex)
{
	int nIndex = m_kerrules.GetItemCount();
	if(nEditIndex >= 0)
		nIndex = nEditIndex;
	else
		m_kerrules.InsertItem(nIndex, L"", 0);

	WCHAR wszTemp[6];

	BYTE *pByte ;
	WCHAR wszIP[32];

	// source ip
	pByte = (BYTE*)&pItem->sourceIP;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	CString s = (pItem->sourceIP == 0) ? L"����" : wszIP;
	m_kerrules.SetItemText(nIndex, 0, s);
	// source mask
	pByte = (BYTE*)&pItem->sourceMask;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	m_kerrules.SetItemText(nIndex, 1, wszIP);
	// source port
	s = (pItem->sourcePort == 0) ? L"����" : ::_itot(pItem->sourcePort, wszTemp, 10);
	m_kerrules.SetItemText(nIndex, 2, s);
	// destinion's IP
	pByte = (BYTE*)&pItem->destinationIP;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	s = (pItem->destinationIP == 0) ? L"����" : wszIP;
	m_kerrules.SetItemText(nIndex, 3, s);
	// destinatio's MASK
	pByte = (BYTE*)&pItem->destinationMask;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	m_kerrules.SetItemText(nIndex, 4, wszIP);
	// destination port
	s = (pItem->destinationPort == 0) ? L"����" : ::_itot(pItem->destinationPort, wszTemp, 10);
	m_kerrules.SetItemText(nIndex, 5, s);
	// protocol
	if(pItem->protocol == 1)
		m_kerrules.SetItemText(nIndex, 6, L"ICMP");
	else if(pItem->protocol == 6)
		m_kerrules.SetItemText(nIndex, 6, L"TCP");
	else if(pItem->protocol == 17)
		m_kerrules.SetItemText(nIndex, 6, L"UDP");
	else
		m_kerrules.SetItemText(nIndex, 6, L"����");
	// action
	s = pItem->bDrop ? L"����" : L"����";
	m_kerrules.SetItemText(nIndex, 7, s);
}

int CKerRulePage::InitAddRule()
{
	// set default rule
	CKerRuleDlg::m_RuleItem.destinationMask = -1;
	CKerRuleDlg::m_RuleItem.sourceMask = -1;
	CKerRuleDlg::m_RuleItem.bDrop = FALSE;

	// popup dialog
	CKerRuleDlg dlg;
	if(dlg.DoModal() == IDCANCEL)
	{
		return -1;
	}

	if(!g_RuleFile.AddKerRules(&CKerRuleDlg::m_RuleItem, 1))
	{
		AfxMessageBox(L"��� ���Ĳ� �������");
		return -1;
	}

	AddRuleToList(&CKerRuleDlg::m_RuleItem);
	return 0;
}

void CKerRulePage::OnKeradd()				// add a rule
{
	if(InitAddRule() != 0)
		return;

	// apply buttion
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

void CKerRulePage::OnKerdel()				// delete a rule
{	
	POSITION pos = m_kerrules.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox(L"��ѡ��һ������");
		return;
	}

	int nIndex = m_kerrules.GetNextSelectedItem(pos);
	g_RuleFile.DelKerRule(nIndex);
	m_kerrules.DeleteItem(nIndex);

	// apply button
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}
