// SpaceWarView.cpp : CSpaceWarView 类的实现
//

#include "stdafx.h"
#include "SpaceWar.h"

#include "SpaceWarDoc.h"
#include "SpaceWarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceWarView

IMPLEMENT_DYNCREATE(CSpaceWarView, CView)

BEGIN_MESSAGE_MAP(CSpaceWarView, CView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CSpaceWarView 构造/析构
							 //初始化m_max、m_score
CSpaceWarView::CSpaceWarView():m_max(6), m_score(0)
{
	// TODO: 在此处添加构造代码

	//初始化状态为开始
	m_state = START;

	//为了方便, 在这里直接该处窗口的大小
	m_sClient.SetSize(850, 480);

	/************************开始界面工作********************/
	//加载png
	m_startBk.bk.Load(_T("res\\startbg.png"));
	m_startBk.normal.Load(_T("res\\startNormal.png"));
	m_startBk.selected.Load(_T("res\\startSelected.png"));

	//初始化rect
	m_startBk.rect.left = (m_sClient.cx - m_startBk.normal.GetWidth()) / 2;
	m_startBk.rect.right = m_startBk.rect.left + m_startBk.normal.GetWidth();
	m_startBk.rect.top = (m_sClient.cy - m_startBk.normal.GetHeight()) / 2;
	m_startBk.rect.bottom = m_startBk.rect.top + m_startBk.normal.GetHeight();

	//一开始默认未选择
	m_startBk.isSelected	 = false;



	/************************游戏界面工作********************/

	//加载背景
	m_gameBk.fir.Load(_T("res\\gamebg0.png"));
	m_gameBk.sec.Load(_T("res\\gamebg1.png"));
	m_gameBk.isFir	= TRUE;
	m_gameBk.curx	= 0;


	


	//加载子弹
	m_bullet.resize(10);
	for(int i=0; i<10; i++)
	{
		m_bullet[i].png.Load(_T("res\\bullet.png"));
	}
	
	//加载dragon
	m_vecMons.resize(m_max);
	for (int i = 0; i<m_max; i++)
	{
		int t = 1 + rand() % 10;
		t = t * 10;
		if (t >= 0 && t < 88) {
			m_vecMons[i].png.Load(_T("res\\long1.png"));
			 m_vecMons[i].life = 1;
			 m_vecMons[i].type = 1;
		}

		else {
			m_vecMons[i].png.Load(_T("res\\long2.png"));
			m_vecMons[i].life = 2;
			m_vecMons[i].type = 2;
			m_vecMons[i].speed = 1;
		}

	}

	//初始化暂停、继续位图
	m_stopGoOn.img.Load(_T("res\\play.bmp"));
	m_stopGoOn.isStop = FALSE;
	m_stopGoOn.rect.SetRect(0, 0, m_stopGoOn.img.GetWidth(),
		m_stopGoOn.img.GetHeight() / 2);
	//技能
	SKILL.img.Load(_T("res\\dianhu.bmp"));
	SKILL.isSKIL = 2;
	SKILL.rect.SetRect(200, 0, SKILL.img.GetWidth()+200, SKILL.img.GetHeight());



	/************************结束界面准备工作********************/
	m_endBk.bk.Load(_T("res\\overbg.png"));
	//m_endBk.gameover.Load(_T("res\\gameover1.png"));
	m_endBk.againNor.Load(_T("res\\againNor.bmp"));
	m_endBk.againSel.Load(_T("res\\againSel.bmp"));
	m_endBk.exitNor.Load(_T("res\\exitNor.bmp"));
	m_endBk.exitSel.Load(_T("res\\exitSel.bmp"));


	m_endBk.rRes.left = (m_sClient.cx - m_endBk.againNor.GetWidth()) / 2-50;
	m_endBk.rRes.right = m_endBk.rRes.left + m_endBk.againNor.GetWidth();
	m_endBk.rRes.top = (m_sClient.cy - m_endBk.againNor.GetHeight()) / 2 + 40;
	m_endBk.rRes.bottom = m_endBk.rRes.top + m_endBk.againNor.GetHeight();

	//初始化rExit
	m_endBk.rExit.left = m_endBk.rRes.left + 90;
	m_endBk.rExit.right = m_endBk.rExit.left + m_endBk.exitNor.GetWidth();
	m_endBk.rExit.top = m_endBk.rRes.top;
	m_endBk.rExit.bottom = m_endBk.rRes.bottom;

	//开始默认为未选择
	m_endBk.isExitSel = false;
	m_endBk.isResSel = false;
}

CSpaceWarView::~CSpaceWarView()
{
}

//贴背景
void CSpaceWarView::StickBk()
{
	//如果到了左边界，就换图
	if(m_gameBk.curx == -m_gameBk.fir.GetWidth())
	{
		m_gameBk.curx	= 0;
		m_gameBk.isFir	= m_gameBk.isFir? 0 : 1;
	}

	int width = m_gameBk.fir.GetWidth();

	//如果第一张图在前面，就先绘制第一张
	if(m_gameBk.isFir)
	{
		m_gameBk.fir.BitBlt(m_bufferDC, 
			m_gameBk.curx, 0, SRCCOPY);

		m_gameBk.sec.BitBlt(m_bufferDC, 
			m_gameBk.curx + width, 0, SRCCOPY);
	}
	//第二张图在前面，先绘制第二张
	else
	{
		m_gameBk.sec.BitBlt(m_bufferDC, 
			m_gameBk.curx, 0, SRCCOPY);

		m_gameBk.fir.BitBlt(m_bufferDC, 
			m_gameBk.curx + width, 0, SRCCOPY);
	}
}

//贴子弹
void CSpaceWarView::StickBullet()
{
	for(size_t i=0; i<m_bullet.size(); i++)
	{
		//如果为被移除, 就贴
		if(!m_bullet[i].isOut)
		{
			m_bullet[i].png.TransparentBlt(m_bufferDC, 
				m_bullet[i].rect, RGB(255, 255, 255));
		}
	}
}

//排除出界的子弹
void CSpaceWarView::BulletOut()
{
	for(size_t i=0; i<m_bullet.size() && !m_bullet[i].isOut; i++)
	{
		//如果移到客户区边界, 就移除
		if(m_bullet[i].rect.left < 0 ||
		   m_bullet[i].rect.right > m_sClient.cx ||
		   m_bullet[i].rect.top < 0 ||
		   m_bullet[i].rect.bottom > m_sClient.cy)
		{
			m_bullet[i].isOut = true;
		}
	}
}

//移动子弹
void CSpaceWarView::MoveBullet()
{
	for(size_t i=0; i<m_bullet.size(); i++)
	{
		if(!m_bullet[i].isOut)
		{
			//返回当前子弹射击到的dragon索引
			int index_beat = Beat(i);
			
			//如果子弹射击到了dragon
			if(index_beat != -1)
			{
				//计分
				m_vecMons[index_beat].life--;
				m_vecMons[index_beat].png.Load(_T("res\\long5.png"));
				if (m_vecMons[index_beat].life <= 0) {
					
					if (m_vecMons[index_beat].type ==2) {
						m_score += 10;
						
					}
						
					if (m_vecMons[index_beat].type == 1) {
						m_score += 5;
					}
						
					RestartMonster(false, index_beat);
				}
				if (m_vecMons[index_beat].life == 1&&m_vecMons[index_beat].type==2) {
					m_vecMons[index_beat].speed+=8;
				}
			//	m_score += 5;

				//重新启动当前dragon
				

				//标记当前子弹出界了
				m_bullet[i].isOut = true;

				//异步播放爆炸音乐
				
				continue;
			}

			//移动子弹
			m_bullet[i].rect.left += m_bullet[i].ix;
			m_bullet[i].rect.right += m_bullet[i].ix;
			m_bullet[i].rect.top += m_bullet[i].iy;
			m_bullet[i].rect.bottom += m_bullet[i].iy;
		}
	}
}

//返回子弹i射击到的dragonj(-1表示未射击到)
int CSpaceWarView::Beat(int i)
{
	CRect r = m_bullet[i].rect;

	for(size_t j=0; j<m_vecMons.size(); j++)
	{
		if(m_vecMons[j].rect.PtInRect(CPoint(r.left, r.top)) ||
			m_vecMons[j].rect.PtInRect(CPoint(r.right, r.top)) ||
			m_vecMons[j].rect.PtInRect(CPoint(r.left, r.bottom)) ||
			m_vecMons[j].rect.PtInRect(CPoint(r.right, r.bottom)) )
		{
			if (m_vecMons[j].type == 1) {
				PlaySound(TEXT("res\\music\\die.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (m_vecMons[j].type == 2) {
				PlaySound(TEXT("res\\music\\hou.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			return j;
		}
	}

	return -1;
}

//为索引为i的子弹求水平、垂直速度
void CSpaceWarView::GetXY(CPoint org, CPoint end, int i)
{
	//求得子弹的中心位置
	org.x += m_bullet[i].png.GetWidth();
	org.y += m_bullet[i].png.GetHeight();

	//如果两点在一条垂直直线上
	if(org.x == end.x)
	{
		m_bullet[i].ix = 0;
		m_bullet[i].iy = m_bullet[i].speed;

		if(org.y < end.y)
			m_bullet[i].iy = - m_bullet[i].iy;
	}
	//如果两点在一条水平直线上
	else if(org.y == end.y)
	{
		m_bullet[i].iy = 0;
		m_bullet[i].ix = m_bullet[i].speed;

		if(org.x > end.x)
			m_bullet[i].ix = - m_bullet[i].ix;
	}
	else
	{
		//求斜率k(正切)、倾斜角angle、cos(余弦)、sin(正弦)
		double k		= (org.y - end.y) * 1.0 / (org.x - end.x);
		double angle	= atan(k);
		double cosine	= cos(angle);
		double sine		= sin(angle);

		/*
			将直线上的速度speed分解为x、y轴上的分量
			分别表示x上的移动速度、y上的移动速度
		*/
		m_bullet[i].ix = (int)fabs(m_bullet[i].speed * cosine);

		if(org.x > end.x)
			m_bullet[i].ix = - m_bullet[i].ix;

		m_bullet[i].iy = (int)fabs(m_bullet[i].speed * sine);

		if(org.y > end.y)
			m_bullet[i].iy = - m_bullet[i].iy;
	}
}

//重新启动怪物机
void CSpaceWarView::RestartMonster(bool isStart, int i)
{
	int x, y;

	if(isStart)
	{
		x = rand()%300 + m_sClient.cx;
	}
	else
	{
		x = m_sClient.cx;
		
	}

	y = rand()%(m_sClient.cy - m_vecMons[i].png.GetHeight());
	int t = 1 + rand() % 10;
	t = t * 10;
	if (t >= 0 && t < 88) {
		m_vecMons[i].png.Load(_T("res\\long1.png"));
		m_vecMons[i].life = 1;
		m_vecMons[i].type = 1;
	}

	else {
		m_vecMons[i].png.Load(_T("res\\long2.png"));
		m_vecMons[i].life = 2;
		m_vecMons[i].type = 2;
		m_vecMons[i].speed = 1;
	}
	m_vecMons[i].rect.SetRect(x, y, 
		x + m_vecMons[i].png.GetWidth(),
		m_vecMons[i].png.GetHeight()+ y);

	//根据当前的分数来决定怪物移动的速度
	if (m_vecMons[i].type == 1) {
		if (m_score < 100)
			m_vecMons[i].speed = rand() % 3 + 2;
		else if (m_score < 500)
			m_vecMons[i].speed = rand() % 4 + 3;
		else if (m_score < 1500)
			m_vecMons[i].speed = rand() % 5 + 4;
		else if (m_score < 3000)
			m_vecMons[i].speed = rand() % 6 + 4;
		else
			m_vecMons[i].speed = rand() % 7 + 6;
	}
}

//在移动怪物机过程中判断怪物机是否出了边界
void CSpaceWarView::MoveMonster()
{
	for(size_t i=0; i<m_vecMons.size(); i++)
	{
		//水平移动怪物机只需要变化rect的left和right
		m_vecMons[i].rect.left -= m_vecMons[i].speed;

		//如果怪物机出了边界, 进入游戏结束状态
		if(m_vecMons[i].rect.left < -m_vecMons[i].rect.Width())
		{
			m_state = END;

			EndGame();

			InvalidateRect(NULL, FALSE);
			break;
		}

		m_vecMons[i].rect.right -= m_vecMons[i].speed;
	}
}


//开始游戏
void CSpaceWarView::StartGame()
{
	
	//先关闭开始、结束背景音乐
	mciSendString(_T("Close startbg"), NULL, 0, NULL);
	mciSendString(_T("Close overbg1"), NULL, 0, NULL);

	//开始游戏, 分数为0
	m_score = 0;

	//初始化英雄dragon
	int top = (m_sClient.cy - m_hero.png.GetHeight())/2 ;
	m_hero.rect.SetRect(0, top,
		m_hero.png.GetWidth(), top + m_hero.png.GetHeight());

	//初始化子弹
	for(int i=0; i<10; i++)
	{
		m_bullet[i].isOut = true;
	}
	
	int x = m_hero.rect.right - 15;
	//int y = (m_sClient.cy - m_hero.png.GetHeight());
	int y = (m_sClient.cy - m_bullet[0].png.GetHeight()) / 2 + 30;

	//初始化dragon
	srand(GetTickCount());

	for(int i=0; i<m_max; i++)
	{
		RestartMonster(true, i);
	}

	//设置子弹的初始起点
	m_ptBltOrg.SetPoint(x, y);

	//设置定时器
	SetTimer(ID_MOUVEBK, 20, NULL);
	SetTimer(ID_MONSTER, 20, NULL);
	SetTimer(ID_BULLET, 20, NULL);
	SetTimer(ID_HERO, 200, NULL);

	//打开背景音乐
	mciSendString(_T("Open res\\music\\gamebg1.mp3  alias gamebg1"), NULL, 0, NULL);
	//播放背景音乐(重复播放)
	mciSendString(_T("Play gamebg1 repeat"), NULL, 0, NULL);
}

//结束游戏
void CSpaceWarView::EndGame()
{
	//关闭计时器
	KillTimer(ID_MOUVEBK);
	KillTimer(ID_MONSTER);
	KillTimer(ID_BULLET);
	KillTimer(ID_HERO);

	//关闭音乐
	mciSendString(_T("Close attack"), NULL, 0, NULL);
	mciSendString(_T("Close die"), NULL, 0, NULL);
	mciSendString(_T("Close gamebg1"), NULL, 0, NULL);
}

//暂停游戏
void CSpaceWarView::PauseGame()
{
	//关闭计时器
	KillTimer(ID_MOUVEBK);
	KillTimer(ID_MONSTER);
	KillTimer(ID_HERO);
	KillTimer(ID_BULLET);

	//暂停音乐
	mciSendString(_T("Pause attack"), NULL, 0, NULL);
	mciSendString(_T("Pause die"), NULL, 0, NULL);
	mciSendString(_T("Pause gamebg1"), NULL, 0, NULL);
}

//继续游戏
void CSpaceWarView::GoOnGame()
{
	//设置定时器
	SetTimer(ID_MOUVEBK, 20, NULL);
	SetTimer(ID_MONSTER, 20, NULL);
	SetTimer(ID_BULLET, 20, NULL);
	SetTimer(ID_HERO, 200, NULL);


	//播放音乐
	mciSendString(_T("Play gamebg1 repeat"), NULL, 0, NULL);
}

//开始界面
void CSpaceWarView::StartUI()
{
	

	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\startbg.mp3 alias startbg"), NULL, 0, NULL);
	mciSendString(_T("Play startbg repeat"), NULL, 0, NULL);


	//贴开始背景
	m_startBk.bk.BitBlt(m_bufferDC, 0, 0, SRCCOPY);

	//贴"开始"样字的png
	if(m_startBk.isSelected)
	{
		m_startBk.selected.TransparentBlt(m_bufferDC, 
			m_startBk.rect, RGB(255, 255, 255));
	}
	else
	{
		m_startBk.normal.TransparentBlt(m_bufferDC, 
			m_startBk.rect, RGB(255, 255, 255));
	}
}

//游戏界面
void CSpaceWarView::RunningUI()
{

	//贴背景
	StickBk();

	//贴dragon
	for(size_t i=0; i<m_vecMons.size(); i++)
		m_vecMons[i].png.TransparentBlt(m_bufferDC, 
			m_vecMons[i].rect, RGB(255, 255, 255));

	//贴子弹
	StickBullet();

	//贴英雄dragon
	m_hero.png.TransparentBlt(m_bufferDC, m_hero.rect, RGB(255, 255, 255));
	//m_hero.png.BitBlt(m_bufferDC,)

	//贴暂停、继续位图
	int w = m_stopGoOn.img.GetWidth();
	int h = m_stopGoOn.img.GetHeight() / 2;
	m_stopGoOn.img.TransparentBlt(m_bufferDC, 0, 0, w, h,
		0, m_stopGoOn.isStop * h, w, h, RGB(255, 255, 255));

	int w1 = SKILL.img.GetWidth();
	int h1 = SKILL.img.GetHeight() ;
	SKILL.img.TransparentBlt(m_bufferDC,SKILL.rect, RGB(255, 255, 255));



	//***************贴分数*********************
	CString str;
	CString str2;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight     = -25;

	CFont font;
	font.CreateFontIndirectW(&lf);

	str.Format(_T("成绩: %d"), m_score);
	str2.Format(_T("%d"), SKILL.isSKIL);

	//设置文本属性
	m_bufferDC.SelectObject(font);
	m_bufferDC.SetTextColor(RGB(255, 127, 39));
	m_bufferDC.SetBkMode(TRANSPARENT);
	
	m_bufferDC.TextOutW(m_stopGoOn.img.GetWidth() + 20, 0, str);
	m_bufferDC.TextOutW(SKILL.img.GetWidth() + 220, 0, str2);

	//释放字体资源
	font.DeleteObject();

}

//结束界面
void CSpaceWarView::EndUI()
{
	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\overbg1.mp3 alias overbg1"), NULL, 0, NULL);
	mciSendString(_T("Play overbg1"), NULL, 0, NULL);

	//贴背景
	m_endBk.bk.BitBlt(m_bufferDC, 0, 0, SRCCOPY);

	//***************贴分数*********************
	CString str;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight     = -25;

	CFont font;
	font.CreateFontIndirectW(&lf);

	str.Format(_T("你的成绩: %d"), m_score);
	

	//设置文本属性
	m_bufferDC.SelectObject(font);
	m_bufferDC.SetTextColor(RGB(255, 127, 139));
	m_bufferDC.SetBkMode(TRANSPARENT);
	
	m_bufferDC.TextOutW(m_endBk.rRes.left, m_endBk.rRes.top - 50, str);
	

	//释放字体资源
	font.DeleteObject();

	//贴"游戏结束"字样的png
	//m_endBk.gameover.TransparentBlt(m_bufferDC, m_endBk.rGo, RGB(255, 255, 255));

	//贴重玩图片
	if(m_endBk.isResSel)
	{
		m_endBk.againSel.TransparentBlt(m_bufferDC, 
			m_endBk.rRes, RGB(255, 255, 255));
	}
	else
	{
		m_endBk.againNor.TransparentBlt(m_bufferDC,
			m_endBk.rRes, RGB(255, 255, 255));
	}

	//贴退出图片
	if(m_endBk.isExitSel)
	{
		m_endBk.exitSel.TransparentBlt(m_bufferDC,
			m_endBk.rExit, RGB(255, 255, 255));
	}
	else
	{
		m_endBk.exitNor.TransparentBlt(m_bufferDC,
			m_endBk.rExit, RGB(255, 255, 255));
	}
}

BOOL CSpaceWarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSpaceWarView 绘制

void CSpaceWarView::OnDraw(CDC* pDC)
{
	
	//加载英雄机
	if (time % 6 == 1)
		m_hero.png.Load(_T("res\\hero11.png"));
	else if(time % 6 == 2)
		m_hero.png.Load(_T("res\\hero12.png"));
	else if (time % 6== 3)
		m_hero.png.Load(_T("res\\hero13.png"));
	else if (time % 6 == 4)
		m_hero.png.Load(_T("res\\hero14.png"));
	else if (time % 6== 5)
		m_hero.png.Load(_T("res\\hero15.png"));
	else
		m_hero.png.Load(_T("res\\hero16.png"));
		
	//设置技能图标变化
	if(SKILL.isSKIL>0)
		SKILL.img.Load(_T("res\\dianhu.bmp"));
	else
		SKILL.img.Load(_T("res\\dianhu1.bmp"));

	// TODO: 在此处为本机数据添加绘制代码

	//创建内存位图、内存DC以便在内存中绘图, 实现双缓冲
	m_bufferDC.CreateCompatibleDC(NULL);
	m_bufferBmp.CreateCompatibleBitmap(pDC,
		m_gameBk.fir.GetWidth() * 2, m_gameBk.fir.GetHeight());

	m_bufferDC.SelectObject(m_bufferBmp);

	switch(m_state)
	{
	//绘制开始界面
	case START:
		StartUI();
		break;

	//绘制游戏界面
	case RUNNING:
		RunningUI();
		break;

	//绘制结束界面
	case END:
		EndUI();
		break;
	}

	//将内存中的图贴到客户区中
	pDC->BitBlt(0, 0, m_sClient.cx, m_sClient.cy, &m_bufferDC, 0, 0, SRCCOPY);

	//释放内存位图和内存DC
	m_bufferBmp.DeleteObject();
	m_bufferDC.DeleteDC();
}


// CSpaceWarView 诊断

#ifdef _DEBUG
void CSpaceWarView::AssertValid() const
{
	CView::AssertValid();
}

void CSpaceWarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpaceWarDoc* CSpaceWarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceWarDoc)));
	return (CSpaceWarDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpaceWarView 消息处理程序

void CSpaceWarView::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch(m_state)
	{
	case START:
		{
			//如果点击了开始
			if(m_startBk.rect.PtInRect(point))
			{
				//进入游戏状态
				m_state = RUNNING;

				StartGame();
			}
		}
		break;

	case RUNNING:
		{
		if (SKILL.rect.PtInRect(point)) {
			if (SKILL.isSKIL>0) {
				for (size_t j = 0; j < m_vecMons.size(); j++)
				{
					RestartMonster(false, j);
					
				}
				SKILL.isSKIL--;
			}
			
		}
			//如果点击暂停或继续
			else if(m_stopGoOn.rect.PtInRect(point))
			{
				if(m_stopGoOn.isStop)
				{
					GoOnGame();
					m_stopGoOn.isStop = FALSE;
				}
				else
				{
					PauseGame();
					m_stopGoOn.isStop = TRUE;
				}
				InvalidateRect(NULL, FALSE);
			}

			else
			{
				
				//如果没有暂停游戏
				 if(!m_stopGoOn.isStop)
				{
					//按下鼠标左键, 发送子弹
					for(size_t i=0; i<m_bullet.size(); i++)
					{
						if(m_bullet[i].isOut)
						{
							m_bullet[i].speed = 20;

							int x = m_ptBltOrg.x;
							int y = m_ptBltOrg.y;
							m_bullet[i].rect.SetRect(x, y, 
								m_bullet[i].png.GetWidth() + x,
								m_bullet[i].png.GetHeight() + y);

							GetXY(m_ptBltOrg, point, i);

							m_bullet[i].isOut = false;
							break;
						}
					}
					//异步播放射击音乐
					PlaySound (TEXT ("res\\music\\attack.wav"), 
						NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			

		}
		break;

	case END:
		{
			if(m_endBk.rRes.PtInRect(point))
			{
				m_state = RUNNING;
				StartGame();
			}

			else if(m_endBk.rExit.PtInRect(point))
			{
				if(IDYES == MessageBox(_T("\t确认退出?"), _T("屠龙"), MB_YESNO))
				{
					//退出程序
					exit(0);
				}
			}
		}
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}

//响应计时器消息
void CSpaceWarView::OnTimer(UINT_PTR nIDEvent)
{
	

	switch(nIDEvent)
	{
	case ID_MOUVEBK:
		m_gameBk.curx -= 10;
		break;

	case ID_MONSTER:
		MoveMonster();
		break;

	case ID_BULLET:
		BulletOut();
		MoveBullet();
		break;
	case ID_HERO:
		time++;
		break;
	}
	//Invalidate();
	InvalidateRect(NULL,FALSE);

	CView::OnTimer(nIDEvent);
}


void CSpaceWarView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//如果是在开始状态移动鼠标(未点击), 就变换"开始"png
	if(START == m_state)
	{
		if(m_startBk.rect.PtInRect(point))
		{
			m_startBk.isSelected = true;
		}
		else
		{
			m_startBk.isSelected = false;
		}

		InvalidateRect(NULL, FALSE);
	}
	//变换重玩、退出位图
	else if(END == m_state)
	{
		if(m_endBk.rRes.PtInRect(point))
		{
			m_endBk.isResSel = true;
		}
		else
		{
			m_endBk.isResSel = false;
		}

		if(m_endBk.rExit.PtInRect(point))
		{
			m_endBk.isExitSel = true;
		}
		else
		{
			m_endBk.isExitSel = false;
		}

		InvalidateRect(NULL, FALSE);
	}

	CView::OnMouseMove(nFlags, point);
}


//善后工作
void CSpaceWarView::OnDestroy()
{
	CView::OnDestroy();

	/*
	程序退出之前, 释放所有内存资源
	*/

	EndGame();

	//释放开始界面资源
	m_startBk.bk.ReleaseGDIPlus();
	m_startBk.normal.ReleaseGDIPlus();
	m_startBk.selected.ReleaseGDIPlus();

	//释放游戏界面资源
	m_gameBk.fir.ReleaseGDIPlus();
	m_gameBk.sec.ReleaseGDIPlus();

	m_hero.png.ReleaseGDIPlus();

	for(size_t i=0; i<m_bullet.size(); i++)
	{
		m_bullet[i].png.ReleaseGDIPlus();
	}
	
	for(size_t i=0; i<m_vecMons.size(); i++)
	{
		m_vecMons[i].png.ReleaseGDIPlus();
	}

	//释放结束界面资源
	m_endBk.bk.ReleaseGDIPlus();
	m_endBk.gameover.ReleaseGDIPlus();
}