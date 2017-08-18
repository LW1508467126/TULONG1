// SpaceWarView.h : CSpaceWarView ��Ľӿ�
//


#pragma once

//��ʱ��ID
#define ID_BULLET	100//���ӵ�
#define ID_MONSTER	101//����dragon
#define ID_MOUVEBK	102//�ƶ�����
#define ID_HERO  103//����Ӣ��dragon


//Ӣ��dragon��dragon
typedef struct img
{
	CImage	png;
	CRect	rect;
	int     life;
	int     type;
	int		speed;
	bool	isOut;
}IMG;

//�ӵ�
typedef struct blt
{
	CImage	png;
	CRect	rect;
	int		speed;
	bool	isOut;
	int		ix;//x�ƶ�����
	int		iy;//y�ƶ�����
}BULLET;

//��ʾ״̬:	  ��ʼ   ��Ϸ��   ����
enum	STATE{START, RUNNING, END,SKILL};


class CSpaceWarView : public CView
{
protected: // �������л�����
	CSpaceWarView();
	DECLARE_DYNCREATE(CSpaceWarView)


// ��д
public:

	int time = 1;
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
public:
	virtual ~CSpaceWarView();



private:
	//��ʾ״̬
	STATE	m_state;

	CDC		m_bufferDC;//����DC
	CBitmap	m_bufferBmp;//����Bmp

	CSize	m_sClient;//�ͻ�����С
	CPoint	m_ptBltOrg;//�ӵ������

	//��ʼ����
	struct sbk
	{
		CImage bk;
		CImage normal;
		CImage selected;
		CRect  rect;
		bool   isSelected;
	}m_startBk;

	//��Ϸ����
	struct gbk
	{
		CImage	fir;//��һ�ű���
		CImage	sec;//�ڶ��ű���
		BOOL	isFir;//�������Ƿ��ǵ�һ��
		int		curx;//��߱�����x����
	}m_gameBk;

	//��������
	struct ebk
	{
		CImage bk;
		CImage gameover;
		CImage againNor;//���¿�ʼ
		CImage againSel;
		CImage exitNor;//�˳�
		CImage exitSel;
		CRect  rGo;//gameover��rect
		CRect  rRes;//restart��rect
		CRect  rExit;//exit��rect
		bool   isExitSel;
		bool   isResSel;
	}m_endBk;

	//��ͣ�����
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
	

	IMG				m_hero;//Ӣ��dragon
	vector<BULLET>	m_bullet;//�ӵ�
	vector<IMG>		m_vecMons;//dragon

	size_t		m_score;//�洢����-ÿ����ɹ�һ��, ��5��

	const int	m_max;//��ǹ���������

//��Ա����
private:
	//��Ϸ������״̬
	void StartUI();//��ʼ
	void RunningUI();//��Ϸ��
	void EndUI();//����

	//��ʼ��Ϸ
	void StartGame();
	//������Ϸ
	void EndGame();
	//��ͣ��Ϸ
	void PauseGame();
	//������Ϸ
	void GoOnGame();

	//���ڵ��ô����ܶ�, ��Ϊ��������
	inline void StickBk();//������
	inline void StickBullet();//���ӵ�
	inline void BulletOut();//�ų�������ӵ�
	inline void MoveMonster();//�ƶ�dragon
	inline void MoveBullet();//�ƶ��ӵ�
	inline int  Beat(int i);//�����ӵ�i�������dregon j

	//��������dragon
	inline void RestartMonster(bool isStart, int i);
	//Ϊ����Ϊi���ӵ���ˮƽ����ֱ�ٶ�
	inline void GetXY(CPoint org, CPoint end, int i);
	
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	
};



