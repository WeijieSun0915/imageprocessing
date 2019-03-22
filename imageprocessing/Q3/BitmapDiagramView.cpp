
#include "BitmapDiagramView.h"
#include <stdlib.h>
#include <cmath> 
#include <iostream> 
#include  <string> 
#include <stdio.h> 


using namespace std;

// CWaveDiagramView

BmpPicView::BmpPicView()
{
}

BmpPicView::~BmpPicView()
{
}

void BmpPic::ReadBitmap(CString f)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	CFile file;
	file.Open(f, CFile::modeReadWrite);

	//Read the Bitmap file header;  
	file.Read(&fileHeader, sizeof(BITMAPFILEHEADER));
	//Read the Bitmap info header;  
	file.Read(&infoHeader, sizeof(BITMAPINFOHEADER));
	

	height = infoHeader.biHeight;
	width = infoHeader.biWidth;
	if (infoHeader.biBitCount >= 1)
	{
		int size = height * width;
		image1 = new RGB[size];
		file.Read(image1, size * sizeof(RGB));
	}
	file.Flush();
	file.Close();
	CMainFrame *Mainfrm = (CMainFrame*)AfxGetMainWnd();
	Mainfrm->GetActiveView()->Invalidate();
}


void BmpPicView::drawRBGHistogram(CDC* pointer1, BmpPic* pointer2)
{
	LONG i=0;
	LONG j=0;
	LONG height= pointer2->height;
	LONG width = pointer2->width;;
	RGB *image1;
	image1 = pointer2->image1;
	int r[256], g[256], b[256]
	int Value1=0;
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			r[image1[width*i + j].r] =r[image1[width*i + j].r]+ 1;
			g[image1[width*i + j].g] =g[image1[width*i + j].g]+ 1;
			b[image1[width*i + j].b] =b[image1[width*i + j].b]+ 1;
		}
	}
	CPen rpen(PS_SOLID, 1, RGB(200, 0, 0));
	pointer1->SelectObject(&rpen);
	for (int i = 0; i < 256; i++)
	{

		pointer1->MoveTo(i,500);						// mouse move to position , line draw to position
		pointer1->LineTo(i, 500-r[i] / Value1);
		}
	CPen gpen(PS_SOLID, 1, RGB(0, 200, 0)); //draw solid line
	pointer1->SelectObject(&gpen);
	for (int i = 0; i < 256; i++)
	{

		pointer1->MoveTo(i, 500);
		pointer1->LineTo(i,500 - g[i]  / Value1);
	}
	CPen bpen(PS_SOLID, 1, RGB(0, 0, 200));
	pointer1->SelectObject(&bpen);
	for (int i = 0; i < 256; i++)
	{

		pointer1->MoveTo( i, 500);
		pointer1->LineTo(i, 500 - b[i] / Value1);
	}
}
void BmpPicView::drawBrighterBitmap(CDC* pointer1, BmpPic* pointer2)
{
	LONG i, j;
	LONG height = pointer2->height; 
	LONG width = pointer2->width;
	RGB *image1;
	image1 = pointer2->image1;
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			byte r, g, b;
			if (image1[width*i + j].r*1.5 < 255) //BRIGHTER 1.5 TIMIES
			{									// r,g,b * 1.5
												// if >5 then = 255
				r = image1[width*i + j].r*1.5;
			}
			else
			{
				r = 255;
			}
			if (image1[width*i + j].g*1.5 < 255)
			{
				g = image1[width*i + j].g*1.5;
			}
			else
			{
				g = 255;
			}
			if (image1[width*i + j].b*1.5 < 255)
			{
				b = image1[width*i + j].b*1.5;
			}
			else
			{
				b = 255;
			}
			pointer1->SetPixel( j, 50 + height - i, RGB(r, g, b));
		}
	}
}
void BmpPicView::DrawDithering(CDC* pointer1, BmpPic* pointer2)
{
	LONG i, j; 
	LONG height, width;
	RGB *image1;
	byte *Gray,*dither;
	int bayer[8][8] = { {  0, 32,  8, 40,  2, 34, 10, 42},
					    { 48, 16, 56, 24, 50, 18, 58, 26 },
						{ 12, 44,  4, 36, 14, 46,  6, 38},
						{ 60, 28, 52, 20, 62, 30, 54, 22},
						{  3, 35, 11, 43,  1, 33,  9, 41},
						{ 51, 19, 59, 27, 49, 17, 57, 25},
						{ 15, 47,  7, 39, 13, 45,  5, 37},
						{ 63, 31, 55, 23, 61, 29, 53, 21 }}; //8*8 matrix for testing

	int matrix[4][4] = {	{ 0, 12,  3, 15},
							{ 8, 4, 11, 7 },
							{ 2, 14,  1, 13},
							{ 10, 6, 9, 5} }; //4*4 matrix for testing discussion


	int matrix1[2][2] = {	{ 0, 2 },
							{ 3, 1 } };//2*2 matrix for testing discussion
							
							


	height = pointer2->height;
	width = pointer2->width;
	image1 = pointer2->image1;
	Gray = new byte[height*width];
	dither= new byte[height*width];
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			Gray[width*i + j] = (image1[width*i + j].r * 299 + image1[ j+width*i].g * 587 + image1[j+width*i].b * 114) / 1000;
		}
	}
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{	
				if ((Gray[j+width*i ]/4) > bayer[j % 8][i % 8]){//DIVIDE 4 BY USING 8*8 MATRIX
					//DIVIDE 16 BY USING 4*4 MATRIX
					// DIVIDE 64 BY USING 2*2 MATRIX
					dither[j+width*i ] = 255;		// BIGGER --> PRINT DOT
				}
				else { dither[j+width*i] = 0; }
		}
	}
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pointer1->SetPixel(j, 50 + height - i, RGB(dither[j+width*i], dither[j+width*i], dither[j+width*i]));
		}
	}
}
void BmpPicView::DrawGrayScale(CDC* pointer1, BmpPic* pointer2)
{
	LONG i, j;
	LONG height = pointer2->height; 
	LONG width = pointer2->width;
	RGB *image1;
	image1 = pointer2->image1;
	for (i = 0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			byte Gray = (image1[width*i + j].r * 299 + image1[width*i + j].g * 587 + image1[width*i + j].b * 114) / 1000; //convert TO Y
			byte GrayIMG = RGB(Gray, Gray, Gray);
			pointer1->SetPixel(j, 50 + height - i, GrayIMG);
		}
	}
}
void BmpPicView::OnDraw(CDC* pointer1)
{
	BmpPic* pointer2 = GetDocument();
	ASSERT_VALID(pointer2);
	if (!pointer2)
		return;
	CRect rect;
	char buffer[100];
	sprintf_s(buffer, "Open");
	CString s2(buffer);
	pointer1->TextOut(600, 20, s2);
	GetClientRect(&rect);
	
	if (pointer2->type == 2)
	{
		drawBitmap(pointer1, pointer2);
	}
	
}
void BmpPicView::drawBitmap(CDC* pointer1, BmpPic* pointer2)
{
	CRect rect;
	GetClientRect(&rect);
	
	LONG i, j;
	LONG height, width;
	RGB *image1;

	height= pointer2->height;
	width = pointer2->width;
	image1 = pointer2->image1;
	char buffer[1024];
	sprintf_s(buffer, "height:%d;width:%d", height, width);
	CString s(buffer);
	pointer1->TextOut(20, 20, s);
	sprintf_s(buffer, "Next");
	CString s1(buffer);
	pointer1->TextOut(400, 20, s1);
	

	drawRBGHistogram(pointer1, pointer2);
	drawBrighterBitmap(pointer1, pointer2);
	DrawGrayScale(pointer1, pointer2);
	DrawDithering(pointer1, pointer2);
	for (i =0; i <height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pointer1->SetPixel(j, 50+height- i, RGB(image1[width*i + j].r, image1[width*i + j].g, image1[width*i + j].b));
		}
	}
}