// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "SpaceWar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	//���Ĵ������
	CSize size;

	//��ȡ��Ļ���ش�С
	size.cx = ::GetSystemMetrics(SM_CXSCREEN);
	size.cy = ::GetSystemMetrics(SM_CYSCREEN);

	//���ڷ��
	cs.style	= WS_POPUPWINDOW;
	//ȥ���˵�
	cs.hMenu	= NULL;
	//���ڱ�������
	cs.lpszName = _T("Game");

	//�Զ��崰�ڴ�С, �����ھ�����ʾ
	cs.x		= (size.cx - 856) / 2;
	cs.y		= (size.cy - 486) / 2;
	cs.cx		= 856;
	cs.cy		= 486;

	return TRUE;
}


// CMainFrame ���




// CMainFrame ��Ϣ�������



