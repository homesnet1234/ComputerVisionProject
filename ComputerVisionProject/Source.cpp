#include <iostream>
#include "opencv2/opencv.hpp"
#define endl '\n'
#define THRESHOLD 150
using namespace std;
using namespace cv;

Mat inputImage;
enum { NONE, UP, DOWN, LEFT, RIGHT };

int findHole(int mode, vector<Point> con)
{
	int xs = boundingRect(con).x;
	int ys = boundingRect(con).y;
	int xf = boundingRect(con).x + boundingRect(con).width;
	int yf = boundingRect(con).y + boundingRect(con).height;

	switch (mode)
	{
	case UP:
		yf = ys + boundingRect(con).height / 2;
		break;
	case DOWN:
		ys = yf - boundingRect(con).height / 2;
		break;
	case LEFT:
		xf = xs + boundingRect(con).width / 2;
		break;
	case RIGHT:
		xs = xf - boundingRect(con).width / 2;
		break;
	}
	vector<vector<Point>> hole;
	Mat temp = inputImage.clone();
	cvtColor(temp, temp, COLOR_BGR2GRAY);
	bitwise_not(temp, temp);
	temp = temp.colRange(xs, xf).rowRange(ys, yf);

	//imshow("temp1", temp);

	findContours(temp, hole, RETR_TREE, CHAIN_APPROX_NONE);
	cvtColor(temp, temp, CV_GRAY2BGR);

	/*if (mode == 1)
	{
		for (int i = 0; i < hole.size(); i++)
			rectangle(temp, boundingRect(hole[i]), Scalar(0, 255, 0), 1, LINE_8, 0);

		cout << hole.size() << endl;
		imshow("temp", temp);
		waitKey(0);
	}*/

	return hole.size();
}

int NoHead(vector<Point> contr)
{
	Rect boundRect = boundingRect(contr);

	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);
	int check = 0; // 0 is ก , 1 is ธ
	imshow("temp", temp);

	for (int i = temp.rows - 1; i >= temp.rows / 2; i--)
	{
		if (temp.at<uchar>(i, temp.cols / 2) < THRESHOLD)
		{
			check = 1;
			break;
		}
	}
	if (check)
		putText(inputImage, "1", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 0), 2);
	else
		putText(inputImage, "0", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 0), 2);
	return check;
}

int longTail(vector<Point> contr)
{
	int check = -1;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	int count = 0;
	int black = -1;

	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int i = 0; i < temp.cols; i++)
	{
		if (temp.at<uchar>(temp.rows / 5, i) < THRESHOLD && (black == -1 || black == 0))
		{
			black = 1;
			count++;
		}
		else if (temp.at<uchar>(temp.rows / 5, i) >= THRESHOLD)
			black = 0;
	}

	for (int i = 0; i < temp.rows / 4; i++)
	{
		if (temp.at<uchar>(i, temp.cols / 2) < THRESHOLD)
		{
			count = 0;
			break;
		}
	}

	if (count == 1)
		rectangle(inputImage, boundRect, Scalar(0, 0, 0), 1, LINE_8, 0);
	else
		return check;

	if (temp.at<uchar>(temp.rows * 3 / 4, 0) < THRESHOLD)
		check = 2;
	else
	{
		black = -1;
		count = 0;
		for (int i = 0; i < temp.cols; i++)
		{
			if (temp.at<uchar>(temp.rows * 3 / 4, i) < THRESHOLD && (black == -1 || black == 0))
			{
				count++;
				black = 1;
			}
			else if (temp.at<uchar>(temp.rows * 3 / 4, i) >= THRESHOLD)
				black = 0;
		}

		if (count == 2)
			check = 0;
		else
			check = 1;
	}

	switch (check)
	{
	case 0:
		putText(inputImage, "0", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 255, 0), 2);
		break;
	case 1:
		putText(inputImage, "1", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 255, 0), 2);
		break;
	case 2:
		putText(inputImage, "2", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 255, 0), 2);
		break;
	}

	return check; // 0 : ป, 1 : ฟ, 2 : ฝ
}

int OneHead(vector<Point> contr)
{
	//rectangle(inputImage,boundingRect(contr[i]),Scalar(0,0,255),1,LINE_8,0);
	int up = findHole(UP, contr);
	int down = findHole(DOWN, contr);
	int left = findHole(LEFT, contr);
	int right = findHole(RIGHT, contr);
	Rect boundRect = boundingRect(contr);
	int check;

	if ((check = longTail(contr) != -1))
	{

	}
	//switch (up)
	//{
	//case 1:
	//	rectangle(inputImage, boundRect, Scalar(0, 0, 0), 1, LINE_8, 0);
	//	//putText(inputImage, "1", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 255, 0), 2);
	//	break;
	//case 2:
	//	rectangle(inputImage, boundRect, Scalar(255, 0, 0), 1, LINE_8, 0);
	//	//putText(inputImage, "2", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 255), 2);
	//	break;
	//case 3:
	//	rectangle(inputImage, boundRect, Scalar(0, 255, 0), 1, LINE_8, 0);
	//	//putText(inputImage, "3", Point(boundRect.x, boundRect.y), 1, 2, Scalar(255, 0, 0), 2);
	//	break;
	//case 4:
	//	rectangle(inputImage, boundRect, Scalar(0, 0, 255), 1, LINE_8, 0);
	//	//putText(inputImage, "4", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 0), 2);
	//	break;
	//default:
	//	rectangle(inputImage, boundRect, Scalar(0, 255, 255), 1, LINE_8, 0);
	//	//putText(inputImage, "x", Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 0), 2);
	//	break;
	//}
	return -1;
}

int TwoHead(vector<Point> contr)
{
	return -1;
}

void findOverAllHole()
{
	Mat temp = inputImage.clone();
	cvtColor(temp, temp, COLOR_BGR2GRAY);
	bitwise_not(temp, temp);
	threshold(temp, temp, 100, 255, THRESH_BINARY);
	vector<vector<Point>> contr;
	findContours(temp, contr, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (int i = 0; i < contr.size(); i++)
	{
		int overall = findHole(NONE, contr[i]) - 1;

		switch (overall)
		{
			//            case 1:
			//                rectangle(input,boundingRect(contr[i]),Scalar(0,255,0),1,LINE_8,0);
			//                break;
		case 0:
			NoHead(contr[i]);
			break;
		case 1:
			//OneHead(contr[i]);
			break;
		case 2:
			TwoHead(contr[i]);
			break;

			//            case 3:
			//                rectangle(input,boundingRect(contr[i]),Scalar(255,0,0),1,LINE_8,0);
			//                break;
		}
	}
}


int main()
{
	ios_base::sync_with_stdio(false);
	//More accuary for big cbaracter
	//resize(input, input, Size(input.cols * 10, input.rows * 10), 0, 0, INTER_LINEAR);

	inputImage = imread("Untitled.png");
	threshold(inputImage, inputImage, THRESHOLD, 255, THRESH_BINARY);

	findOverAllHole();
	imshow("thai catch", inputImage);
	waitKey(0);
}
