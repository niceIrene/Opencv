#ifndef CARPOSITION_H_INCLUDED
#define CARPOSITION_H_INCLUDED
#include<iostream>
#include"init.h"
using namespace std;
void getcarposition()
{
    IplImage* cap = cvCloneImage(transimg); //Ŀ��ͼ��
		IplImage* redcap = cvCreateImage(cvGetSize(cap), 8, 1);//��ɫͨ��ͼ��
		IplImage* blackredcap1 = cvCreateImage(cvGetSize(cap), 8, 1);//��ɫͨ��ͼ���ֵ��ͼ��
		IplImage* greencap = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* blackredcap2 = cvCreateImage(cvGetSize(cap), 8, 1);//��ɫͨ��ͼ���ֵ��ͼ��
		IplImage* bluecap = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* graycap = cvCreateImage(cvGetSize(cap), 8, 1);

		IplImage* blackgreencap1 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* blackgreencap2 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* blackbluecap1 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* blackbluecap2 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* blackgraycap = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* redandgreen1 = cvCreateImage(cvGetSize(cap), 8, 1);//���̶�ֵ��ͼ��ȡ��
		IplImage* redandgreen2 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* and1 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* and2 = cvCreateImage(cvGetSize(cap), 8, 1);

		cvSplit(cap, NULL, NULL, redcap, NULL);//��ȡBGRͼ��ĸ�ͨ�����Ҷ�ͨ��
		cvSplit(cap, NULL, greencap, NULL, NULL);
		cvSplit(cap, bluecap, NULL, NULL, NULL);
		cvCvtColor(cap, graycap, CV_BGR2GRAY);

		const char *windowandcap = "�ϲ�����1";
		cvNamedWindow(windowandcap);

		int red1 = 124, green1 = 63, blue1 = 57, red2 = 76, green2 = 89, blue2 = 196;//��ɫ��ֵ
																					 //cvNamedWindow("windowblackredcap");
																					 //cvNamedWindow("windowblackgreencap");
																					 //cvNamedWindow("windowblackbluecap");

																					 // תΪ��ֵͼ
		cvThreshold(redcap, blackredcap1, red1, 255, CV_THRESH_BINARY);//������ֵ��תΪ���ֵ������תΪ0
		cvThreshold(redcap, blackredcap2, red2, 255, CV_THRESH_BINARY_INV);
		// ��ʾ��ֵͼ
		//cvShowImage("windowblackredcap", blackredcap);
		// תΪ��ֵͼ
		cvThreshold(greencap, blackgreencap1, green1, 255, CV_THRESH_BINARY_INV);//������ֵ��תΪ0������תΪ���ֵ
		cvThreshold(greencap, blackgreencap2, green2, 255, CV_THRESH_BINARY);
		// ��ʾ��ֵͼ
		//cvShowImage("windowblackgreencap", blackgreencap);
		// תΪ��ֵͼ
		cvThreshold(bluecap, blackbluecap1, blue1, 255, CV_THRESH_BINARY_INV);
		cvThreshold(bluecap, blackbluecap2, blue2, 255, CV_THRESH_BINARY_INV);
		// ��ʾ��ֵͼ
		//cvShowImage("windowblackbluecap", blackbluecap);


		cvAnd(blackredcap1, blackgreencap1, redandgreen1, NULL);
		cvAnd(blackredcap2, blackgreencap2, redandgreen2, NULL);
		cvAnd(redandgreen1, blackbluecap1, and1, NULL);
		cvAnd(redandgreen2, blackbluecap2, and2, NULL);
		cvReleaseImage(&redcap);
		cvReleaseImage(&blackredcap1);
		cvReleaseImage(&blackredcap2);
		cvReleaseImage(&greencap);
		cvReleaseImage(&blackgreencap1);
		cvReleaseImage(&blackgreencap2);
		cvReleaseImage(&bluecap);
		cvReleaseImage(&blackbluecap1);
		cvReleaseImage(&blackbluecap2);
		cvReleaseImage(&graycap);
		cvReleaseImage(&blackgraycap);
		cvReleaseImage(&redandgreen1);
		cvReleaseImage(&redandgreen2);


		cvNamedWindow("cap");
		cvShowImage("cap", cap);

			int Sumx1 = 0, Sumy1 = 0, Sumx2 = 0, Sumy2 = 0, n = 0, m = 0;
	int propo = 2;//���ű���
	int and1height = cvGetSize(and1).height, and1width = cvGetSize(and1).width;
	IplImage* small1 = cvCreateImage(cvSize(and1width / propo, and1height / propo), 8, 1);
	IplImage* small2 = cvCreateImage(cvSize(and1width / propo, and1height / propo), 8, 1);
	CvMat* transmatfromand1tosmall1 = cvCreateMat(3, 3, CV_32FC1);  //����һ��3*3�ĵ�ͨ��32λ������󱣴�任����
	CvPoint2D32f originpointsofand1[4]; //�����ĸ����ԭʼ����
	CvPoint2D32f newpointsofsmall1[4]; //�������ĸ����������
	originpointsofand1[0] = cvPoint2D32f(0, 0);
	newpointsofsmall1[0] = cvPoint2D32f(0, 0);
	originpointsofand1[1] = cvPoint2D32f(and1width, 0);
	newpointsofsmall1[1] = cvPoint2D32f(and1width / propo, 0);
	originpointsofand1[2] = cvPoint2D32f(0, and1height);
	newpointsofsmall1[2] = cvPoint2D32f(0, and1height / propo);
	originpointsofand1[3] = cvPoint2D32f(and1width, and1height);
	newpointsofsmall1[3] = cvPoint2D32f(and1width / propo, and1height / propo);
	cvGetPerspectiveTransform(originpointsofand1, newpointsofsmall1, transmatfromand1tosmall1); //�����ĸ������任����
	cvWarpPerspective(and1, small1, transmatfromand1tosmall1); //���ݱ任�������ͼ��ı任
	cvWarpPerspective(and2, small2, transmatfromand1tosmall1); //���ݱ任�������ͼ��ı任
	BwImage lei1(small1);
	BwImage lei2(small2);
	for (int i = 1; i <= and1height / propo - 1; i++)
		for (int j = 1; j <= and1width / propo - 1; j++)
		{
			if (lei1[i][j] >= 1)
			{
				//std::cout << i << '\t' << j << std::endl;
				Sumx1 = Sumx1 + j; Sumy1 = Sumy1 + i; n++;
			}
			if (lei2[i][j] >= 1)
			{
				//std::cout << i << '\t' << j << std::endl;
				Sumx2 = Sumx2 + j; Sumy2 = Sumy2 + i; m++;
			}

		}//����ͼ��������������ͼ�����ص���࣬���Կ���������
	Sumx1 = Sumx1 / n;
	Sumx1 = Sumx1 * propo;
	Sumy1 = Sumy1 / n;
	Sumy1 = Sumy1 * propo;
	Sumx2 = Sumx2 / m;
	Sumx2 = Sumx2 * propo;
	Sumy2 = Sumy2 / m;
	Sumy2 = Sumy2 * propo;
	//�ֱ��ȡ����������ɫ���������Ĳ����

		CvPoint point1 = cvPoint(Sumx1 - 20, Sumy1 - 2);
		CvPoint point2 = cvPoint(Sumx1 + 20, Sumy1 + 2);
		CvPoint point3 = cvPoint(Sumx1 - 2, Sumy1 - 20);
		CvPoint point4 = cvPoint(Sumx1 + 2, Sumy1 + 20);
		CvPoint point5 = cvPoint(Sumx2 - 20, Sumy2 - 2);
		CvPoint point6 = cvPoint(Sumx2 + 20, Sumy2 + 2);
		CvPoint point7 = cvPoint(Sumx2 - 2, Sumy2 - 20);
		CvPoint point8 = cvPoint(Sumx2 + 2, Sumy2 + 20);
		CvPoint pt1 = cvPoint(Sumx1, Sumy1);
		CvPoint pt2 = cvPoint(Sumx2, Sumy2);
		cvRectangle(and1, point1, point2, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(and1, point3, point4, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(and2, point5, point6, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(and2, point7, point8, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);

		cvShowImage(windowandcap, and1);
		cvNamedWindow("�ϲ�����2");
		cvShowImage("�ϲ�����2", and2);

		IplImage* look = cvCreateImage(cvGetSize(cap), 8, 1);
		cvSet(look, cvScalar(0, 0, 0, 0), 0);
		cvRectangle(look, point1, point2, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(look, point3, point4, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(look, point5, point6, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvRectangle(look, point7, point8, cvScalar(255, 255, 255, 255), CV_FILLED, 8, 0);
		cvLine(look, pt1, pt2, cvScalar(255, 255, 255, 255), 1, 8, 0);
		cvNamedWindow("ɵ��");
		cvShowImage("ɵ��", look);

		IplImage* oppositeand1 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* oppositeand2 = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* oppositefinal = cvCreateImage(cvGetSize(cap), 8, 1);
		IplImage* final = cvCreateImage(cvGetSize(cap), 8, 1);

		cvThreshold(and1, oppositeand1, 1, 255, CV_THRESH_BINARY_INV);
		cvThreshold(and2, oppositeand2, 1, 255, CV_THRESH_BINARY_INV);
		cvAnd(oppositeand1, oppositeand2, oppositefinal, NULL);
		cvThreshold(oppositefinal, final, 1, 255, CV_THRESH_BINARY_INV);
		cvLine(final, pt1, pt2, cvScalar(255, 255, 255, 255), 1, 8, 0);

		cvNamedWindow("����������������¿");
		cvShowImage("����������������¿", final);

		cvReleaseImage(&oppositeand1);
		cvReleaseImage(&oppositeand2);
		cvReleaseImage(&oppositefinal);
}
bool carLocate(CvPoint &carPos_head, CvPoint &carPos_tail)
{
	//IplImage *Color_frame_Logitech;
	//Color_frame_Logitech=img;
	int bChannel=0, gChannel=1, rChannel=2, height, width, step, channels;
	uchar *data;
	cvSaveImage("1.png", transimg);
	height=transimg->height;
	width=transimg->width;
	step=transimg->widthStep;
	channels=transimg->nChannels;
	data=(uchar *)transimg->imageData;
	int red_x=0, red_y=0, sumHeight=0, sumWidth=0, nRedPoint=0, nGreenPoint=0;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			if ((data[i*step+j*channels+rChannel]-data[i*step+j*channels+gChannel])>50 && (data[i*step+j*channels+rChannel]-data[i*step+j*channels+bChannel])>40) {
				nRedPoint++;
				sumHeight=sumHeight+i;
				sumWidth=sumWidth+j;
			}
		}
	}
	if (nRedPoint) {
		sumHeight=sumHeight/nRedPoint;
		sumWidth=sumWidth/nRedPoint;
		red_x=sumHeight;
		red_y=sumWidth;
		cvCircle(transimg, cvPoint(sumWidth, sumHeight), 10, cvScalar(0, 0, 255), 2);
		carPos_head=cvPoint(sumWidth, sumHeight);
	}
	sumHeight=sumWidth=0;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			if ((data[i*step+j*channels+gChannel]-data[i*step+j*channels+rChannel])>45 && sqrt(pow(i-red_x, 2)+pow(j-red_y, 2))<100) {
				nGreenPoint++;
				sumHeight=sumHeight+i;
				sumWidth=sumWidth+j;
			}
		}
	}
	if (nGreenPoint) {
		sumHeight/=nGreenPoint;
		sumWidth/=nGreenPoint;
		cvCircle(transimg, cvPoint(sumWidth, sumHeight), 10, cvScalar(0, 255, 0), 2);
		carPos_tail=cvPoint(sumWidth, sumHeight);
		return true;
	}
	return false;
}
double length(CvPoint p1,CvPoint2D32f p2)
{
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)*1.0+(p1.y-p2.y)*(p1.y-p2.y)*1.0);
}

#endif // CARPOSITION_H_INCLUDED
