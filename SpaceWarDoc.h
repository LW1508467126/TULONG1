// SpaceWarDoc.h : CSpaceWarDoc ��Ľӿ�
//


#pragma once


class CSpaceWarDoc : public CDocument
{
protected: // �������л�����
	CSpaceWarDoc();
	DECLARE_DYNCREATE(CSpaceWarDoc)

// ����
public:

// ����
public:

// ��д
public:
	//virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CSpaceWarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


