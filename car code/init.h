#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED
#define ARRIVE 40
#include<iostream>
#include "cv.h"
#include "highgui.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
HANDLE hComm;
OVERLAPPED m_ov;

COMSTAT comstat;
DWORD m_dwCommEvents;
CvCapture *pCapture;
IplImage *img;
IplImage *timg[5];
IplImage *transimg = cvCreateImage(cvSize(400,400), IPL_DEPTH_8U, 3);
CvPoint2D32f originpoints[4];
int times = 1;
IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
IplImage *thin = NULL;
CvMat* transmat = cvCreateMat(3, 3, CV_32FC1);
const int MAX_CORNERS = 4;//定义角点个数最大值
CvPoint2D32f* corners = new CvPoint2D32f[MAX_CORNERS];//分配保存角点的空间
CvPoint2D32f* sortcorners = new CvPoint2D32f[MAX_CORNERS];//分配保存角点的空间
CvPoint carPos_head,carPos_tail,carPos_mid,p1,p2;
IplImage* img1 = cvCreateImage(cvSize(400, 400), 8, 1);
IplImage*img_copy=NULL;
//CvPoint &carPos_head; CvPoint &carPos_tail;
int i=0,mymax[6]={0,0,0,0,0,10000000},min1=0,sequence[5];

const char *pstrWindowsBinaryTitle = "binary";


//图像类的定义
template<class T> class Image {
private:
	IplImage* imgp;
public:
	Image(IplImage* img = 0) { imgp = img; }
	~Image() { imgp = 0; }
	inline T* operator[](const int rowIndx) {
		return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));
	}
};
typedef struct {
	unsigned char b, g, r;
} RgbPixel;
typedef Image<RgbPixel> RgbImage;
typedef Image<unsigned char> BwImage;


#endif // INIT_H_INCLUDED
