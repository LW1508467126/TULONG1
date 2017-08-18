// MainFrm.cpp : CMainFrame 类的实现
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


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//更改窗口外观
	CSize size;

	//获取屏幕像素大小
	size.cx = ::GetSystemMetrics(SM_CXSCREEN);
	size.cy = ::GetSystemMetrics(SM_CYSCREEN);

	//窗口风格
	cs.style	= WS_POPUPWINDOW;
	//去掉菜单
	cs.hMenu	= NULL;
	//窗口标题名称
	cs.lpszName = _T("Game");

	//自定义窗口大小, 并窗口居中显示
	cs.x		= (size.cx - 856) / 2;
	cs.y		= (size.cy - 486) / 2;
	cs.cx		= 856;
	cs.cy		= 486;

	return TRUE;
}


// CMainFrame 诊断




// CMainFrame 消息处理程序



