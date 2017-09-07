#include"findcorners.h"
#include"init.h"
#include"carposition.h"

#include"settings.h"
int main()
{
    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	//要是想用外置的摄像头126行的参数设置为1
	CvCapture*cam = cvCaptureFromCAM(1);

	if (!cam)return 0;
	img = cvQueryFrame(cam);
	img = cvQueryFrame(cam);
	trans();
	cvNamedWindow("win2");
	cvWarpPerspective(img, transimg, transmat);
	cvShowImage("win2", transimg);


	//二值化操作开始，首先读取透视变化好的图片
	IplImage *Image = transimg;
	// 转为灰度图
	g_pGrayImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);
	cvCvtColor(Image, g_pGrayImage, CV_BGR2GRAY);
	// 创建二值图
	g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
	// 创建二值图窗口
	cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);


	// 滑动条
	int nThreshold = 0;
	cvCreateTrackbar("二值图阈值", pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);

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
