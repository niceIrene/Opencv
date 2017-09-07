#include"findcorners.h"
#include"init.h"
#include"carposition.h"

#include"settings.h"
int main()
{
    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	//Ҫ���������õ�����ͷ126�еĲ�������Ϊ1
	CvCapture*cam = cvCaptureFromCAM(1);

	if (!cam)return 0;
	img = cvQueryFrame(cam);
	img = cvQueryFrame(cam);
	trans();
	cvNamedWindow("win2");
	cvWarpPerspective(img, transimg, transmat);
	cvShowImage("win2", transimg);


	//��ֵ��������ʼ�����ȶ�ȡ͸�ӱ仯�õ�ͼƬ
	IplImage *Image = transimg;
	// תΪ�Ҷ�ͼ
	g_pGrayImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);
	cvCvtColor(Image, g_pGrayImage, CV_BGR2GRAY);
	// ������ֵͼ
	g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
	// ������ֵͼ����
	cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);


	// ������
	int nThreshold = 0;
	cvCreateTrackbar("��ֵͼ��ֵ", pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);

	on_trackbar(1);
    cvWaitKey();

   while(1)
    {
        img = cvQueryFrame(cam);
		//trans();
		cvWaitKey(15);
		cvWarpPerspective(img, transimg, transmat);
		carLocate(carPos_head,carPos_tail);
        for(int i=0;i<4;i++)
        cvCircle(transimg, cvPoint(corners[i].x, corners[i].y), 10, cvScalar(255, 0, 0), 2);
        cvNamedWindow("car");
		cvShowImage("car",transimg);
        instruction();


		//cargo();
		/*if(length(carPos_head,sortcorners[0])<ARRIVE) cout<<"!!!arrive1!!!"<<endl;
		if(length(carPos_head,sortcorners[1])<ARRIVE) cout<<"!!!arrive2!!!"<<endl;
		if(length(carPos_head,sortcorners[2])<ARRIVE) cout<<"!!!arrive3!!!"<<endl;
		if(length(carPos_head,sortcorners[3])<ARRIVE) cout<<"!!!arrive4!!!"<<endl;*/
		//instruction();
        cvWaitKey(15);
    }
	cvReleaseCapture(&cam);
	cvDestroyAllWindows();

return 0;
}
