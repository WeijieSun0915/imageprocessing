typedef struct
{
	BYTE b;
	BYTE g;
	BYTE r;
}RGB;
class MyBitmap
{
public:
	LONG height=0, width =0;
	RGB* img;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	CFile file;
	MyBitmap();
	~MyBitmap();
};
class BmpPicView : public CView
{
public:
	int drawBitmapMode = 0;
protected:
	BmpPicView();
	DECLARE_DYNCREATE(BmpPicView)

public:
	BmpPic* GetDocument() const;

public:
	void drawBitmap(CDC* pointer1, BmpPic* pointer2);
	void DrawGrayScale(CDC* pointer1, BmpPic* pointer2);
	void drawRBGHistogram(CDC* pointer1, BmpPic* pointer2);
	void drawBrighterBitmap(CDC* pointer1, BmpPic* pointer2);
	void DrawDithering(CDC* pointer1, BmpPic* pointer2);

public:
	virtual void OnDraw(CDC* pointer1);  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pointer1, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pointer1, CPrintInfo* pInfo);

public:
	virtual ~BmpPicView();
protected:

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


