// SerialCommDoc.h : interface of the CSerialCommDoc class
//


#pragma once


class CSerialCommDoc : public CDocument
{
protected: // create from serialization only
	CSerialCommDoc();
	DECLARE_DYNCREATE(CSerialCommDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSerialCommDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


