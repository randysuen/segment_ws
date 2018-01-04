#include <iostream>
#include <fstream>
#include <bitset>
#include <queue>
#include <stack>
#include <algorithm>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>

//#define _MIN_
//#define _SIM_
#define	MM	  50   //�ֲ���ֵ�㱣������
#define COE	  0.2  //ϵ��


using namespace cv;
using namespace std;


struct node {
	Point2i coord;
	uchar value;
	friend bool operator< (node n1, node n2) {
#ifdef _MIN_
		return n1.value < n2.value;
#else
		return n2.value < n1.value;
#endif
	}
};

Vec3b Render(int value)   // <span style = "line-height: 20.8px; font-family: sans-serif;">//���������ɫ����</span>  
{
	value = value % 255;  //����0~255�������  
	RNG rng;
	int aa = rng.uniform(0, value);
	int bb = rng.uniform(0, value);
	int cc = rng.uniform(0, value);
	return Vec3b(aa, bb, cc);
}

int main() {

#ifdef _SIM_
	Mat imageGray = (Mat_<uchar>(11, 10) << 
		3, 5, 5, 2, 8, 8, 8, 11, 10, 10,
		5, 5, 11, 11, 8, 11, 11, 8, 10, 10,
		11, 5, 11, 11, 9, 9, 9, 9, 8, 10,
		11, 11, 11, 7, 7, 7, 7, 9, 9, 8,
		11, 11, 11, 11, 11, 9, 7, 10, 8, 10,
		11, 10, 11, 9, 7, 7, 9, 9, 10, 8,
		11, 10, 11, 9, 11, 9, 10, 10, 8, 10,
		11, 11, 11, 8, 8, 8, 8, 8, 10, 10,
		11, 11, 11, 11, 10, 10, 10, 10, 10, 10,
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
		11, 11, 11, 11, 10, 10, 10, 10, 10, 10);
	//resize(imageGray, imageGray, Size(0, 0), 10, 10, INTER_AREA);
	Mat oChainCode1Right = (Mat_<uchar>(11, 10) <<
		0, 5, 1, 0, 5, 0, 0, 5, 6, 0,
		3, 4, 2, 3, 4, 4, 4, 0, 5, 6,
		3, 0, 4, 7, 6, 6, 6, 6, 0, 5,
		2, 3, 4, 0, 0, 0, 0, 5, 3, 0,
		8, 7, 2, 3, 4, 4, 0, 4, 0, 3,
		1, 0, 1, 1, 0, 0, 3, 4, 3, 0,
		2, 0, 8, 2, 3, 4, 4, 1, 0, 3,
		2, 3, 1, 0, 0, 0, 0, 0, 3, 4,
		7, 6, 2, 3, 4, 4, 4, 4, 4, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		3, 4, 4, 4, 0, 0, 0, 0, 0, 0
		);
	Mat iChainCode1Right = (Mat_<uchar>(11, 10) <<
		28, 0, 0, 31, 8, 8, 16, 0, 0, 0,
		4, 8, 0, 0, 0, 0, 0, 48, 0, 0,
		0, 14, 0, 0, 0, 0, 0, 0, 52, 0,
		0, 0, 0, 110, 40, 32, 56, 0, 0, 4,
		0, 0, 0, 0, 0, 0, 12, 0, 4, 0,
		0, 195, 0, 1, 15, 8, 0, 0, 0, 4,
		0, 6, 0 ,0, 0, 0, 0,0 ,13, 0,
		0, 0, 0, 143, 8, 8, 8, 8, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		108, 8, 8, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		);
	Mat oChainCode2Right = (Mat_<uchar>(11, 10) <<
		0, 5, 1, 0, 5, 5, 5, 5, 6, 5,
		3, 4, 2, 3, 4, 4, 4, 4, 5, 6,
		3, 0, 4, 7, 6, 6, 6, 6, 4, 5,
		2, 3, 4, 0, 0, 0, 0, 5, 3, 4,
		8, 7, 2, 3, 4, 4, 0, 4, 2, 3,
		1, 0, 1, 1, 0, 0, 3, 4, 3, 4,
		2, 0, 8, 2, 3, 4, 4, 1, 2, 3,
		2, 3, 1, 1, 1, 1, 1, 2, 3, 4,
		7, 6, 2, 3, 4, 4, 4, 4, 4, 4,
		1, 1, 1, 2, 3, 4, 4, 4, 4, 4,
		3, 4, 4, 4, 4, 4, 4, 4, 4, 4
		);
	Mat iChainCode2Right = (Mat_<uchar>(11, 10) <<
		28, 0, 0, 31, 24, 24, 24, 0, 16, 0,
		12, 8, 0, 0, 0, 0, 0, 56, 0, 0,
		0, 14, 0, 0, 0, 0, 0, 0, 60, 0,
		0, 0, 0, 110, 40, 32, 56, 0, 0, 6,
		0, 0, 0, 0, 0, 0, 12, 0, 12, 0,
		0, 195, 0, 1, 15, 8, 0, 0, 0, 6,
		0, 6, 0, 0, 0, 0, 0, 0, 15, 0,
		0, 0, 0, 143, 9, 9, 9, 9, 8, 0,
		0, 0, 0, 0, 14, 8, 8, 8, 8, 0,
		108, 9, 9, 9, 8, 8, 8, 8, 8, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		);
	Mat oChainCode3Right = (Mat_<uchar>(11, 10) <<
		0, 5, 1, 0, 5, 5, 5, 5, 6, 5,
		3, 4, 2, 3, 4, 4, 4, 4, 5, 6,
		3, 4, 4, 7, 6, 6, 6, 6, 4, 5,
		2, 3, 4, 0, 5, 5, 5, 5, 3, 4,
		8, 7, 2, 3, 4, 4, 3, 4, 2, 3,
		1, 0, 1, 1, 1, 2, 3, 4, 3, 4,
		2, 3, 8, 2, 3, 4, 4, 1, 2, 3,
		2, 3, 1, 1, 1, 1, 1, 2, 3, 4,
		7, 6, 2, 3, 4, 4, 4, 4, 4, 4,
		1, 1, 1, 2, 3, 4, 4, 4, 4, 4,
		3, 4, 4, 4, 4, 4, 4, 4, 4, 4
		);
	Mat iChainCode3Right = (Mat_<uchar>(11, 10) <<
		28, 0, 0, 31, 24, 24, 24, 0, 16, 0,
		12, 8, 0, 0, 0, 0, 0, 56, 0, 0,
		0, 14, 0, 0, 0, 0, 0, 0, 60, 0,
		0, 0, 0, 126, 56, 48, 60, 0, 0, 6,
		0, 0, 0, 0, 0, 0, 14, 0, 12, 0,
		0, 199, 0, 1, 15, 9, 0, 0, 0, 6,
		0, 6, 0, 0, 0, 0, 0, 0, 15, 0,
		0, 0, 0, 143, 9, 9, 9, 9, 8, 0,
		0, 0, 0, 0, 14, 8, 8, 8, 8, 0,
		108, 9, 9, 9, 8, 8, 8, 8, 8, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		);
#else
	Mat image = imread("data\\toy.jpg");
	if (!image.data)
		return 0;
	resize(image, image, Size(0, 0), 0.05, 0.05, INTER_AREA);
	imshow("Raw Image", image);
	Mat imageGray;
	cvtColor(image, imageGray, COLOR_RGB2GRAY);
	GaussianBlur(imageGray, imageGray, Size(5, 5), 2);   //��˹�˲�  
	imshow("Gray Image", imageGray);

#endif
	ofstream ofile;
	ofile.open("data\\imageGray.csv");
	ofile << format(imageGray, "CSV") << endl;
	ofile.close();

	//��ʼ��ָ�������ָ������
	Mat oChainCode(imageGray.size(), CV_8UC1, Scalar(0));
	Mat iChainCode(imageGray.size(), CV_8UC1, Scalar(0x00));
	//Mat flagFIFO(imageGray.size(), CV_8UC1, Scalar(0)); //��¼�����Ƿ���FIFO��
	int nr = oChainCode.size().height;
	int nc = oChainCode.size().width;

	//��չ�Ҷ�ͼ�߽�+1
	copyMakeBorder(imageGray, imageGray, 1, 1, 1, 1, BORDER_REPLICATE);

	//8����ָ�������ָ�������
	uchar oChainCodeTable[8]{ 4, 3, 2, 1, 5, 6, 7, 8};
	uchar iChainCodeTable[8]{ 0x80, 0x40, 0x20, 0x10, 0x01, 0x02, 0x04, 0x08};
	//�����Ͻǿ�ʼ��˳ʱ��ת��Ȧ������ʱ��ת��Ȧ����һ���������кţ�y�����ڶ������кţ�x��
	int dirTable[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
	//�ֲ���ֵ�㻺��
	queue<Point2i> localMMFIFO;
	//Mat localMMFlag(iChainCode.size(), CV_8UC1, Scalar(0));
	//��һ��ɨ��ͼ�񣬼�¼��Чָ�������ָ������
	for (int i = 1; i <= nr; i++) {
		for (int j = 1; j <= nc; j++){
			Rect roi(j - 1, i - 1, 3, 3);
			Mat window;
			imageGray(roi).copyTo(window);
			double minValue, maxValue;
			//��ֲ�����ֵ
			minMaxLoc(window, &minValue, &maxValue);
			/*
			ע�⣺��Mat.at<>(i,j)��������ʱ��i���кţ�j���к�
			*/
			//�������ֵС�ھֲ�����ֵ�����ָ������
			bool a;
#ifdef _MIN_
			a = imageGray.at<uchar>(i, j) > minValue;
#else
			a = imageGray.at<uchar>(i, j) < maxValue;
#endif
			//if (imageGray.at<uchar>(i, j) < maxValue) {
			if (a) {
				for (int s = 0; s < 8; s++) {
					if (i == 1 && j == 1)
						//if (s >= 1 && s <= 5)
						if (s<=2 || s==4 || s==5)
							continue;
					if (i == 1 && j != 1 && j != nc)
						//if (s <=4 && s >= 2)
						if (s<=2)
							continue;
					if (i == 1 && j == nc)
						//if (s <= 3 || s == 7)
						if ((s<=3 || s==7))
							continue;
					if (i != 1 && i != nr&&j == 1)
						//if (s >= 3 && s <= 5)
						if (s==0 || s==4 || s==5)
							continue;
					if (i != 1 && i != nr&&j == nc)
						//if (s <= 1 || s == 7)
						if (s==2 || s==3 || s==7)
							continue;
					if (i == nr && j == 1)
						//if (s >= 3 && s <= 7)
						if (s>=4 || s==0)
							continue;
					if (i == nr && j != 1 && j != nc)
						//if (s >= 5)
						if (s>=5 && s<=7)
							continue;
					if (i == nr && j == nc)
						//if (s >= 5 || s <= 1)
						if (s==2 || s==3 || (s>=5 && s<=7))
							continue;
					bool b;
#ifdef _MIN_
					b = imageGray.at<uchar>(i + dirTable[s][0], j + dirTable[s][1]) == minValue;
#else
					b = imageGray.at<uchar>(i + dirTable[s][0], j + dirTable[s][1]) == maxValue;
#endif
					if (b) {
						//��¼�������ص�ָ������
						oChainCode.at<uchar>(i - 1, j - 1) = oChainCodeTable[s];
						//�����������ص�ָ������
						iChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1]) |= iChainCodeTable[7-s];
						break;
					}
				}
			}
			else {
				//�����ھӣ��Ǹ���ǰ���أ����뼫ֵ�㻺��
				Point2i localMM;
				localMM = Point2i(j-1, i-1);
				//localMM.value = 0;
				localMMFIFO.push(localMM);
			}
		}
	}
#ifdef _SIM_

	cout << "non zero of out chain code after first scan : " << countNonZero(oChainCode) << endl;
	cout << "number of local min or max pixels: " << localMMFIFO.size() << endl;
	cout << "ochaincode1: " << endl;
	cout << oChainCode << endl;

	cout << "ochaincode1right: " << endl;
	cout << oChainCode1Right << endl;

	cout << "ichaincode1: " << endl;
	cout << iChainCode << endl;

	cout << "ichaincode1right: " << endl;
	cout << iChainCode1Right << endl;

	cout << "--------------------------" << endl;
#endif

	//�ڶ���ɨ�裬�������ƽ̨�Ϳ�ֵ�����ָ�������ָ������
	queue<Point2i> seedFIFO;
	int lMMFSize = localMMFIFO.size();
	for (int index = 0; index < lMMFSize; index++) {
		Point2i localMM = localMMFIFO.front();
		localMMFIFO.pop();
		int i = localMM.y + 1;
		int j = localMM.x + 1;
		//�����֦��ʵ���ᴥ��
		if (static_cast<int>(oChainCode.at<uchar>(i - 1, j - 1)) != 0){
			//cout << "break" << endl;
			continue;
		}
		for (int s = 0; s < 8; s++) {
			if (i == 1 && j == 1)
				if (s <= 2 || s == 4 || s == 5)
					continue;
			if (i == 1 && j != 1 && j != nc)
				if (s <= 2)
					continue;
			if (i == 1 && j == nc)
				if ((s <= 3 || s == 7))
					continue;
			if (i != 1 && i != nr&&j == 1)
				if (s == 0 || s == 4 || s == 5)
					continue;
			if (i != 1 && i != nr&&j == nc)
				if (s == 2 || s == 3 || s == 7)
					continue;
			if (i == nr && j == 1)
				if (s >= 4 || s == 0)
					continue;
			if (i == nr && j != 1 && j != nc)
				if (s >= 5 && s <= 7)
					continue;
			if (i == nr && j == nc)
				if (s == 2 || s == 3 || (s >= 5 && s <= 7))
					continue;
			//�������ֵ��ͬ�����ѱ��ָ��������ھ�
			if (imageGray.at<uchar>(i, j) == imageGray.at<uchar>(i + dirTable[s][0], j + dirTable[s][1])) {
				if (static_cast<int>(oChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1])) != 0){
					Point2i seed;
					seed.x = j - 1 + dirTable[s][1];
					seed.y = i - 1 + dirTable[s][0];
					//���ھӴ������
					seedFIFO.push(seed);
					//cout << "if ���� : " << i << "," << j << endl;
					//cout << "s = " << s << endl;
					break;
				}
			}
		}
		//���������һֱ����
		if (static_cast<int>(oChainCode.at<uchar>(i - 1, j - 1)) == 0) {
			//cout << "��������� : " << i << "," << j << endl;
			localMMFIFO.push(localMM);
		}
	}
	//������зǿ�
	while (!seedFIFO.empty()){
		Point2i seed = seedFIFO.front();
		//�Ӷ���ͷȡ��һ������
		seedFIFO.pop();
		int i = seed.y + 1;
		int j = seed.x + 1;
		for (int s = 0; s < 8; s++){
			if (i == 1 && j == 1)
				if (s <= 2 || s == 4 || s == 5)
					continue;
			if (i == 1 && j != 1 && j != nc)
				if (s <= 2)
					continue;
			if (i == 1 && j == nc)
				if ((s <= 3 || s == 7))
					continue;
			if (i != 1 && i != nr&&j == 1)
				if (s == 0 || s == 4 || s == 5)
					continue;
			if (i != 1 && i != nr&&j == nc)
				if (s == 2 || s == 3 || s == 7)
					continue;
			if (i == nr && j == 1)
				if (s >= 4 || s == 0)
					continue;
			if (i == nr && j != 1 && j != nc)
				if (s >= 5 && s <= 7)
					continue;
			if (i == nr && j == nc)
				if (s == 2 || s == 3 || (s >= 5 && s <= 7))
					continue;
			//�������δ��ʶָ��������ھӣ���ֵ�͵�ǰ������ͬ
			if (static_cast<int>(oChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1])) == 0){
				//cout << "δ��ʶ���ھ� : " << i + dirTable[s][0] << "," << j + dirTable[s][1] << endl;
				if (imageGray.at<uchar>(i, j) == imageGray.at<uchar>(i + dirTable[s][0], j + dirTable[s][1])){
					//�����ھӵ�ָ������ָ��ǰ����
					oChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1]) = oChainCodeTable[7 - s];
					//ͬʱ���µ�ǰ���ص�ָ������
					iChainCode.at<uchar>(i - 1, j - 1) |= iChainCodeTable[s];
					seed.x = j - 1 + dirTable[s][1];
					seed.y = i - 1 + dirTable[s][0];
					//�����ھӴ������
					seedFIFO.push(seed);
				}
			}
		}
	}
#ifdef _SIM_
	cout << "non zero of out chain code after second scan : " << countNonZero(oChainCode) << endl;
	cout << "number of local min or max pixels: " << localMMFIFO.size() << endl;
	cout << "ochaincode2: " << endl;
	cout << oChainCode << endl;

	cout << "ochaincode2right: " << endl;
	cout << oChainCode2Right << endl;

	cout << "ichaincode2: " << endl;
	cout << iChainCode << endl;

	cout << "ichaincode2right: " << endl;
	cout << iChainCode2Right << endl;

	cout << "--------------------------" << endl;
#endif
	
	//������ɨ�裬��Ǿֲ���ֵ�����ڵ�ָ�������ָ������
	stack<Point2i> seedStack;
	int numOfMM = 0;
	float thMM;
#ifdef _MIN_
	thMM = 0;
#else
	thMM = 255;
#endif

	lMMFSize = localMMFIFO.size();

	priority_queue<node> localMMPQ;
	for (int index = 0; index < lMMFSize; index++) {
		Point2i localMM = localMMFIFO.front();
		localMMFIFO.pop();
		int i = localMM.y + 1;
		int j = localMM.x + 1;
		if (static_cast<int>(oChainCode.at<uchar>(i - 1, j - 1)) != 0){
			continue;
		}
		for (int s = 0; s < 8; s++) {
			if (i == 1 && j == 1)
				if (s <= 2 || s == 4 || s == 5)
					continue;
			if (i == 1 && j != 1 && j != nc)
				if (s <= 2)
					continue;
			if (i == 1 && j == nc)
				if ((s <= 3 || s == 7))
					continue;
			if (i != 1 && i != nr&&j == 1)
				if (s == 0 || s == 4 || s == 5)
					continue;
			if (i != 1 && i != nr&&j == nc)
				if (s == 2 || s == 3 || s == 7)
					continue;
			if (i == nr && j == 1)
				if (s >= 4 || s == 0)
					continue;
			if (i == nr && j != 1 && j != nc)
				if (s >= 5 && s <= 7)
					continue;
			if (i == nr && j == nc)
				if (s == 2 || s == 3 || (s >= 5 && s <= 7))
					continue;
			//�����ڻҶ�ֵ��ͬ
			if (imageGray.at<uchar>(i, j) == imageGray.at<uchar>(i + dirTable[s][0], j + dirTable[s][1])){
				//��δ��ʶ��ָ��������ھ�����
				if (static_cast<int>(oChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1])) == 0){
					Point2i seed;
					//�����ھӵ�ָ������ָ��ǰ����
					oChainCode.at<uchar>(i - 1 + dirTable[s][0], j - 1 + dirTable[s][1]) = oChainCodeTable[7 - s];
					//ͬʱ���µ�ǰ���ص�ָ������
					iChainCode.at<uchar>(i - 1, j - 1) |= iChainCodeTable[s];
					seed.x = j - 1 + dirTable[s][1];
					seed.y = i - 1 + dirTable[s][0];
					//�����ھ�����ѹ���ջ
					seedStack.push(seed);
				}
			}
		}
		
		//�����ջ�ǿ�
		while (!seedStack.empty()){
			//�Ӷ�ջ��������һ������
			Point2i seed = seedStack.top();
			seedStack.pop();
			int m = seed.y + 1;
			int n = seed.x + 1;
			for (int s = 0; s < 8; s++){
				if (m == 1 && n == 1)
					if (s <= 2 || s == 4 || s == 5)
						continue;
				if (m == 1 && n != 1 && n != nc)
					if (s <= 2)
						continue;
				if (m == 1 && n == nc)
					if ((s <= 3 || s == 7))
						continue;
				if (m != 1 && m != nr&&n == 1)
					if (s == 0 || s == 4 || s == 5)
						continue;
				if (m != 1 && m != nr&&n == nc)
					if (s == 2 || s == 3 || s == 7)
						continue;
				if (m == nr && n == 1)
					if (s >= 4 || s == 0)
						continue;
				if (m == nr && n != 1 && n != nc)
					if (s >= 5 && s <= 7)
						continue;
				if (m == nr && n == nc)
					if (s == 2 || s == 3 || (s >= 5 && s <= 7))
						continue;
				
				//cout << "---m, n = " << m << "," << n << endl;
				//cout << "---s = " << s << endl;
				//�������δ��ʶָ��������ھ�
				if (static_cast<int>(oChainCode.at<uchar>(m - 1 + dirTable[s][0], n - 1 + dirTable[s][1])) == 0) {
					//������ھ���֮ǰforѭ������Ǹ���ǰ���أ�������
					if (m - 1 + dirTable[s][0] == i-1 && n - 1 + dirTable[s][1] == j-1){
						//cout << "--jump one = " << m + dirTable[s][0] << "," << n + dirTable[s][1] << endl;
						continue;
					}
					//������ǣ������ھӺ͵�ǰ���ػҶ���ͬ
					if (imageGray.at<uchar>(m, n) == imageGray.at<uchar>(m + dirTable[s][0], n + dirTable[s][1])){
						//�����ھӵ�ָ������ָ��ǰ����
						oChainCode.at<uchar>(m - 1 + dirTable[s][0], n - 1 + dirTable[s][1]) = oChainCodeTable[7 - s];
						//ͬʱ���µ�ǰ���ص�ָ������
						iChainCode.at<uchar>(m - 1, n - 1) |= iChainCodeTable[s];
						seed.x = n - 1 + dirTable[s][1];
						seed.y = m - 1 + dirTable[s][0];
						//�����ھ�ѹ���ջ
						seedStack.push(seed);
						//cout << "��ջ�ھӣ�" << seed.y + 1 << "," << seed.x + 1 << endl;
					}
				}
			}
		}
		if (static_cast<int>(oChainCode.at<uchar>(i - 1, j - 1)) == 0) {
			node temp;
			temp.coord = localMM;
			temp.value = imageGray.at<uchar>(i, j);
			//localMMFIFO.push(localMM);
			//cout << "pixel : " << static_cast<int>(imageGray.at<uchar>(i, j)) << endl;
			//�������оֲ���ֵ�㣬���ջҶ����򣬼�¼���С����MM��
			if (numOfMM < MM){
				localMMPQ.push(temp);
				numOfMM++;
			}
			else {
				
#ifdef _MIN_
				if (localMMPQ.top().value > temp.value)
					localMMPQ.push(temp);
#else
				if (localMMPQ.top().value < temp.value)
					localMMPQ.push(temp);
#endif
			//	cout << "temp = " << static_cast<int>(temp.value) << endl;
			//	cout << "localMMPQ.top = " << static_cast<int>(localMMPQ.top().value) << endl;
				localMMPQ.pop();
			}
		//	cout << "numOfMM: " << numOfMM << endl;
		//	cout << "thMM : " << static_cast<int>(localMMPQ.top().value) << endl;
		//	cout << "******" << endl;
		}
	}
	

//	cout << "ochaincode3: " << endl;
//	cout << oChainCode << endl;
//	cout << "non zero of out chain code after third scan : " << countNonZero(oChainCode) << endl;
//	cout << "number of local min or max pixels: " << localMMPQ.size() << endl;
#ifdef _SIM_
	cout << "non zero of out chain code after third scan : " << countNonZero(oChainCode) << endl;
	cout << "number of local min or max pixels: " << localMMFIFO.size() << endl;
	cout << "number of needed local min or max: " << numOfMM << endl;
	cout << "gray scale threshold: " << thMM << endl;

	cout << "ochaincode3: " << endl;
	cout << oChainCode << endl;

	cout << "ochaincode3right: " << endl;
	cout << oChainCode3Right << endl;

	cout << "ichaincode3: " << endl;
	cout << iChainCode << endl;

	cout << "ichaincode3right: " << endl;
	cout << iChainCode3Right << endl;

	cout << "---------------" << endl;
#endif
	
	//�Ӿֲ���ֵ�㣨δ���ָ�����룩��ʼ������ָ��������Եõ���ͨ��
	int curLabel = 1;
	float localMax = 0;
	float localMin = 255;
	Mat conDomain(iChainCode.size(),CV_32S,Scalar(0));  //��ͨ��
	stack<Point2i> backTrace;
//	lMMFSize = localMMPQ.size();
//	for (int index = 0; index < lMMFSize; index++) {
	while (!localMMPQ.empty()) {
		node localMM = localMMPQ.top();
		localMMPQ.pop();
		int i = localMM.coord.y + 1;
		int j = localMM.coord.x + 1;
		if (static_cast<int>(oChainCode.at<uchar>(i - 1, j - 1)) != 0){
			continue;
		}
		queue<Point2i> conDomFIFO;
#ifdef _MIN_
		localMin = static_cast<float>(localMM.value);
#else
		localMax = static_cast<float>(localMM.value);
#endif
		conDomFIFO.push(Point2i(j - 1, i - 1));
		//conDomain.at<int>(i - 1, j - 1) = curLabel;
		uchar  inCode1 = iChainCode.at<uchar>(i - 1, j - 1);
		for (int s = 0; s < 8; s++){
			if (static_cast<bool>((inCode1 >> s) & 1)) {//��inCode���ҵ���λȡ����
				Point2i seed;
				if (s < 4){
					seed.x = j - 1 + dirTable[s + 4][1];
					seed.y = i - 1 + dirTable[s + 4][0];
				}
				else {
					seed.x = j - 1 + dirTable[7 - s][1];
					seed.y = i - 1 + dirTable[7 - s][0];
				}
#ifdef _MIN_
				localMax = static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) > localMax ? static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) : localMax;
#else
				localMin = static_cast<float>(imageGray.at<uchar>(seed.y+1, seed.x+1)) < localMin ? static_cast<float>(imageGray.at<uchar>(seed.y+1, seed.x+1)) : localMin;
#endif
				conDomFIFO.push(seed);
				backTrace.push(seed);
			}
		}
		while (!backTrace.empty()){
			Point2i seed = backTrace.top();
			backTrace.pop();
			int m = seed.y + 1; //��
			int n = seed.x + 1; //��
			uchar inCode2 = iChainCode.at<uchar>(m - 1, n - 1);
			for (int s = 0; s < 8; s++){
				if ((bool)((inCode2 >> s) & 1)) {//��inCode���ҵ���λȡ����
					Point2i seed;
					if (s < 4){
						seed.x = n - 1 + dirTable[s + 4][1];
						seed.y = m - 1 + dirTable[s + 4][0];
					}
					else {
						seed.x = n - 1 + dirTable[7 - s][1];
						seed.y = m - 1 + dirTable[7 - s][0];
					}
#ifdef _MIN_
					localMax = static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) > localMax ? static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) : localMax;
#else
					localMin = static_cast<float>(imageGray.at<uchar>(seed.y+1, seed.x+1)) < localMin ? static_cast<float>(imageGray.at<uchar>(seed.y+1, seed.x+1)) : localMin;
#endif
					conDomFIFO.push(seed);
					backTrace.push(seed);
				}
			}
		}
		while (!conDomFIFO.empty()){
			Point2i seed = conDomFIFO.front();
			conDomFIFO.pop();
			bool b;
#ifdef _MIN_
			b = static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) < localMax - (localMax - localMin)*COE;
#else
			b = static_cast<float>(imageGray.at<uchar>(seed.y + 1, seed.x + 1)) > localMin + (localMax - localMin)*COE;
#endif
			if (b) {
				conDomain.at<int>(seed.y, seed.x) = curLabel;
			}
		}
		curLabel++;
	}
	ofile.open("data\\conDomain.csv");
	ofile << format(conDomain, "CSV") << endl;
	ofile.close();
#ifdef _SIM_
	cout << "connected domain: " << endl;
	cout << conDomain << endl;
#endif
	

	//��ÿһ�����������ɫ���  
	Mat PerspectiveImage = Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i<nr; i++)
	{
		for (int j = 0; j<nc; j++)
		{
			int curLabel = conDomain.at<int>(i, j);
			if (conDomain.at<int>(i, j) == 0)
			{
				PerspectiveImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
			else
			{
				PerspectiveImage.at<Vec3b>(i, j) = Render(curLabel);
			}
		}
	}
	imshow("After ColorFill", PerspectiveImage);

	//�ָ�����ɫ�Ľ����ԭʼͼ���ں�  
	Mat wshed;
	addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);
	imshow("AddWeighted Image", wshed);


	waitKey();
	return 1;
}