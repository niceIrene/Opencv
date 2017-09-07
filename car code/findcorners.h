#ifndef FINDCORNERS_H_INCLUDED
#define FINDCORNERS_H_INCLUDED
#include"init.h"
using namespace std;
void mouse(int mouseevent, int x, int y, int flags, void *param)
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);



	if (mouseevent == CV_EVENT_LBUTTONDOWN&&times<5)
	{
		CvPoint pt = cvPoint(x, y);
		char temp[16];
		sprintf(temp, "(%d,%d)", pt.x, pt.y);
		cvPutText(timg[times], temp, pt, &font, cvScalar(255, 255, 255, 0));
		cvCircle(timg[times], pt, 2, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		cvShowImage("tmp", timg[times]);
		if (times < 4) { cvCopy(timg[times], timg[times + 1]); }
		originpoints[times++ - 1] = cvPoint2D32f(x, y);
	}


	if (mouseevent == CV_EVENT_RBUTTONDOWN&&times>1)
	{
		originpoints[--times - 1] = cvPoint2D32f(0, 0);
		cvCopy(timg[times - 1], timg[times]);
		cvShowImage("tmp", timg[times]);
	}
}

void getpoints(CvPoint2D32f *Points, IplImage *img)
{
	cvNamedWindow("tmp");
	cvShowImage("tmp", img);
	while (1) {
		cvSetMouseCallback("tmp", mouse);
		cvWaitKey(10);
		if (times == 5) break;
	}
	return;
}


void sortpointbyx()
{
    for( int j=4;j>=0;j--){
       for( i=0;i<5;i++)
         {if(corners[i].x>min1&&corners[i].x<mymax[j+1]){mymax[j]=corners[i].x;  sequence[j]=i;}}
    }//max数组存储角点的坐标，sequence数组存储角点的序号
    for(int k=0;k<5;++k){
     cout<<"the x of corners ["<<k<<"] is"<<mymax[k];//从起点到终点依次输出每个角点的坐标
    }
}



//细化的操作函数
void cvThin(IplImage* src, IplImage* dst, int iterations = 1) {
	cvCopyImage(src, dst);
	BwImage dstdat(dst);
	IplImage* t_image = cvCloneImage(src);
	BwImage t_dat(t_image);
	for (int n = 0; n < iterations; n++)
		for (int s = 0; s <= 1; s++) {
			cvCopyImage(dst, t_image);
			for (int i = 0; i < src->height; i++)
				for (int j = 0; j < src->width; j++)
					if (t_dat[i][j]) {
						int a = 0, b = 0;
						int d[8][2] = { { -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 },
						{ 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 } };
						int p[8];
						p[0] = (i == 0) ? 0 : t_dat[i - 1][j];
						for (int k = 1; k <= 8; k++) {
							if (i + d[k % 8][0] < 0 || i + d[k % 8][0] >= src->height ||
								j + d[k % 8][1] < 0 || j + d[k % 8][1] >= src->width)
								p[k % 8] = 0;
							else p[k % 8] = t_dat[i + d[k % 8][0]][j + d[k % 8][1]];
							if (p[k % 8]) {
								b++;
								if (!p[k - 1]) a++;
							}
						}
						if (b >= 2 && b <= 6 && a == 1)
							if (!s && !(p[2] && p[4] && (p[0] || p[6])))
								dstdat[i][j] = 0;
							else if (s && !(p[0] && p[6] && (p[2] || p[4])))
								dstdat[i][j] = 0;
					}
		}
	cvReleaseImage(&t_image);
}
void sortCorners()
{
    int max1=0;int index1=0;
    for(int i=0;i<4;i++)
   {if(corners[i].x*corners[i].y>max1) {max1=corners[i].x*corners[i].y;index1=i;}}
   sortcorners[1]=corners[index1];

    int max2=0;int index2=0;
    for(int i=0;i<4;i++)
   {if(corners[i].x*corners[i].y>max2&&corners[i].x*corners[i].y<max1) {max2=corners[i].x*corners[i].y;index2=i;}}
    sortcorners[2]=corners[index2];

    int max3=0;int index3=0;
    for(int i=0;i<4;i++)
   {if(corners[i].x*corners[i].y>max3&&corners[i].x*corners[i].y<max2) {max3=corners[i].x*corners[i].y;index3=i;}}
    sortcorners[0]=corners[index3];

    int max4=0;int index4=0;
    for(int i=0;i<4;i++)
   {if(corners[i].x*corners[i].y>max4&&corners[i].x*corners[i].y<max3) {max4=corners[i].x*corners[i].y;index4=i;}}
    sortcorners[3]=corners[index4];
    for(int k=0;k<4;k++)
   cout<<"sort"<<k<<" is"<<sortcorners[k].x<<' '<<sortcorners[k].y<<endl;
}


//这个是透视变化取坐标点的函数
void trans()
{
	for (int j = 0; j < 5; j++)
	{
		timg[j] = cvCloneImage(img);
	}
	CvPoint2D32f newpoints[4];
	getpoints(originpoints, img);
	newpoints[0] = cvPoint2D32f(20, 20);
	newpoints[1] = cvPoint2D32f(380, 20);
	newpoints[2] = cvPoint2D32f(20, 380);
	newpoints[3] = cvPoint2D32f(380, 380);
	cvGetPerspectiveTransform(originpoints, newpoints, transmat);
	return;
}
void on_trackbar(int pos)
{
    bool finish=0;
	if(!finish)
	{

	// 转为二值图
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY_INV);
	// 显示二值图
	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
	//轨道的细化
	thin = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
	cvThin(g_pBinaryImage, thin);
	//cvNamedWindow("thin", CV_WINDOW_AUTOSIZE);
	//cvShowImage("thin", thin);

	//对细化后的图像寻找角点
	IplImage* eig_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
	IplImage* temp_img = cvCloneImage(eig_img);


	int corner_count = MAX_CORNERS;
	double quality_level = 0.01;//or 0.01
	double min_distance = 100;
	cvGoodFeaturesToTrack(thin, eig_img, temp_img, corners, &corner_count, quality_level, min_distance);

	//画角点
	img_copy = cvCloneImage(g_pGrayImage);
	for (int i = 0; i < corner_count; i++)
	{
		cvCircle(img_copy, cvPoint((int)corners[i].x, (int)corners[i].y),10, CV_RGB(255, 0, 0), 2, 8);
		cout<<"the position of "<<i<<" is "<<corners[i].x<<' '<<corners[i].y<<endl;
		//fprintf(fp,"\t%f,%f\n",corners[i].x,corners[i].y);
	}
	//cout << "检测到角点个数为：" << corner_count;
	cvNamedWindow("角点检测", CV_WINDOW_AUTOSIZE);
	cvShowImage("角点检测", img_copy);
	cvSaveImage("img.png",img_copy);
	sortCorners();

	//角点的保存

	finish=1;
	}
}


#endif // FINDCORNERS_H_INCLUDED
