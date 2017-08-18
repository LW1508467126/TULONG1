// SpaceWarView.h : CSpaceWarView 类的接口
//


#pragma once

//计时器ID
#define ID_BULLET	100//发子弹
#define ID_MONSTER	101//处理dragon
#define ID_MOUVEBK	102//移动背景
#define ID_HERO  103//处理英雄dragon


//英雄dragon和dragon
typedef struct img
{
	CImage	png;
	CRect	rect;
	int     life;
	int     type;
	int		speed;
	bool	isOut;
}IMG;

//子弹
typedef struct blt
{
	CImage	png;
	CRect	rect;
	int		speed;
	bool	isOut;
	int		ix;//x移动坐标
	int		iy;//y移动坐标
}BULLET;

//显示状态:	  开始   游戏中   结束
enum	STATE{START, RUNNING, END,SKILL};


class CSpaceWarView : public CView
{
protected: // 仅从序列化创建
	CSpaceWarView();
	DECLARE_DYNCREATE(CSpaceWarView)


// 重写
public:

	int time = 1;
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
public:
	virtual ~CSpaceWarView();



private:
	//显示状态
	STATE	m_state;

	CDC		m_bufferDC;//缓冲DC
	CBitmap	m_bufferBmp;//缓冲Bmp

	CSize	m_sClient;//客户区大小
	CPoint	m_ptBltOrg;//子弹的起点

	//开始背景
	struct sbk
	{
		CImage bk;
		CImage normal;
		CImage selected;
		CRect  rect;
		bool   isSelected;
	}m_startBk;

	//游戏背景
	struct gbk
	{
		CImage	fir;//第一张背景
		CImage	sec;//第二张背景
		BOOL	isFir;//标记左边是否是第一张
		int		curx;//左边背景的x坐标
	}m_gameBk;

	//结束背景
	struct ebk
	{
		CImage bk;
		CImage gameover;
		CImage againNor;//重新开始
		CImage againSel;
		CImage exitNor;//退出
		CImage exitSel;
		CRect  rGo;//gameover的rect
		CRect  rRes;//restart的rect
		CRect  rExit;//exit的rect
		bool   isExitSel;
		bool   isResSel;
	}m_endBk;

	//暂停与继续
	struct sg
	{
		CImage	img;
		BOOL	isStop;
		CRect	rect;
	}m_stopGoOn;
	struct sk
	{
		CImage img;
		int isSKIL;
		CRect rect;
	}SKILL;
	

	IMG				m_hero;//英雄dragon
	vector<BULLET>	m_bullet;//子弹
	vector<IMG>		m_vecMons;//dragon

	size_t		m_score;//存储分数-每射击成功一次, 加5分

	const int	m_max;//标记怪物最多个数

//成员函数
private:
	//游戏的三种状态
	void StartUI();//开始
	void RunningUI();//游戏中
	void EndUI();//结束

	//开始游戏
	void StartGame();
	//结束游戏
	void EndGame();
	//暂停游戏
	void PauseGame();
	//继续游戏
	void GoOnGame();

	//由于调用次数很多, 设为内联函数
	inline void StickBk();//贴背景
	inline void StickBullet();//贴子弹
	inline void BulletOut();//排除出界的子弹
	inline void MoveMonster();//移动dragon
	inline void MoveBullet();//移动子弹
	inline int  Beat(int i);//返回子弹i射击到的dregon j

	//重新启动dragon
	inline void RestartMonster(bool isStart, int i);
	//为索引为i的子弹求水平、垂直速度
	inline void GetXY(CPoint org, CPoint end, int i);
	
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	
};



