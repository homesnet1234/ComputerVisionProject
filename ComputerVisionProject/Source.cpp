#include <iostream>
#include "opencv2/opencv.hpp"
#define endl '\n'
#define THRESHOLD 150
using namespace std;
using namespace cv;

Mat inputImage;
enum { NONE, UP, DOWN, LEFT, RIGHT };

// ============= Protoype ================================
int findHole(int mode, vector<Point> con);
int NoHead(vector<Point> contr);
int longTail(vector<Point> contr);
int passOneHorizontal(vector<Point> contr);
int passHorizontalCount(vector<Point> contr, float factor);
int passVerticalCount(vector<Point> contr, float factor);
int passMaxVertical(vector<Point> contr);
int passMaxHorizontal(vector<Point> contr);
int OneHead(vector<Point> contr);
int TwoHead(vector<Point> contr);
void findOverAllHole();
int passMinHorizontal(vector<Point> contr);

// ======================================================

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

	rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
	cvtColor(temp, temp, CV_BGR2GRAY);
	int check = 0; // 0 is ก , 1 is ธ

	for (int i = temp.rows - 1; i >= temp.rows / 2; i--)
	{
		if (temp.at<uchar>(i, temp.cols / 2) < THRESHOLD)
		{
			check = 1;
			break;
		}
	}
	
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

	if (count != 1)
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

	return check; // 0 : ป, 1 : ฟ, 2 : ฝ
}

int passOneHorizontal(vector<Point> contr)
{
	int check = -1;
	int count = 0;
	int black = -1;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int i = 0; i < temp.cols; i++)
	{
		if (temp.at<uchar>(temp.rows / 2, i) < THRESHOLD && (black == -1 || black == 0))
		{
			black = 1;
			count++;
		}
		else if (temp.at<uchar>(temp.rows / 2, i) >= THRESHOLD)
			black = 0;
	}

	if (count != 1)
		return check;

	count = 0;
	black = -1;

	for (int i = 0; i < temp.rows / 2; i++)
	{
		if (temp.at<uchar>(i, temp.cols / 2) < THRESHOLD && (black == -1 || black == 0))
		{
			black = 1;
			if (++count == 2)
				break;
		}
		else if (temp.at<uchar>(i, temp.cols / 2) >= THRESHOLD)
			black = 0;
	}

	switch (count)
	{
	case 1:
		check = 0;
		break;
	case 2:
		check = 1;
		break;
	default:
		check = -1;
	}
	return check; // 0 : ว, 1 : ร
}

int passHorizontalCount(vector<Point> contr, float factor)
{
	int check = -1;
	int count = 0;
	int black = -1;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int i = 0; i < temp.cols; i++)
	{
		if (temp.at<uchar>(temp.rows * factor, i) < THRESHOLD && (black == -1 || black == 0))
		{
			black = 1;
			count++;
		}
		else if (temp.at<uchar>(temp.rows * factor, i) >= THRESHOLD)
			black = 0;
	}

	return count;
}

int passVerticalCount(vector<Point> contr, float factor)
{
	int check = -1;
	int count = 0;
	int black = -1;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int i = 0; i < temp.rows; i++)
	{
		if (temp.at<uchar>(i, temp.cols * factor) < THRESHOLD && (black == -1 || black == 0))
		{
			black = 1;
			count++;
		}
		else if (temp.at<uchar>(i, temp.cols * factor) >= THRESHOLD)
			black = 0;
	}

	return count;
}

int passMaxVertical(vector<Point> contr)
{
	int check = -1;
	int max = 0;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int j = 0; j < temp.cols; j++)
	{
		int count = 0;
		int black = -1;
		for (int i = 0; i < temp.rows; i++)
		{
			if (temp.at<uchar>(i, j) < THRESHOLD && (black == -1 || black == 0))
			{
				black = 1;
				count++;
			}
			else if (temp.at<uchar>(i, j) >= THRESHOLD)
				black = 0;
		}
		if (count > max)
			max = count;
	}

	switch (max)
	{
	case 2:
		if (temp.rows >= temp.cols) // บ
		{
			check = 5;
			//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		}
		else // ญ ั
		{
			check = 6;
			//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		}
		break;
	case 3:
		if (NoHead(contr) == 0) // ภ
		{
			check = 7;
		}
		else // จ
		{
			check = 8;
		}
		//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	case 4:
		switch (passVerticalCount(contr, 3.0 / 4.0)) 
		{
		case 1: // ย
			check = 0;
			//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
			break;
		case 2:
			if (temp.at<uchar>(temp.rows - 1, temp.cols / 2) < THRESHOLD) // อ
			{
				check = 1;
				//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
			}
			else // ล
			{
				check = 2;
				//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
			}
			break;
		case 3: // ฐ
			check = 4;
			//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
			break;
		}
		break;
	case 5: // ถ
		check = 4;
		//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	default:
		check = -1;
	}

	return check; // ย : 0, อ : 1, จ : 2 ฐ : 3, ถ = 4, บ = 5, ญ ั = 6, ภ : 7, จ : 8
}

int passMaxHorizontal(vector<Point> contr)
{
	int check = -1;
	int max = 0;
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	for (int j = 0; j < temp.rows; j++)
	{
		int black = -1;
		int count = 0;
		for (int i = 0; i < temp.cols; i++)
		{
			if (temp.at<uchar>(j, i) < THRESHOLD && (black == -1 || black == 0))
			{
				black = 1;
				count++;
			}
			else if (temp.at<uchar>(j, i) >= THRESHOLD)
				black = 0;
		}
		if (count > max)
			max = count;
	}

	switch (max)
	{
	case 2: // ง
		check = 2;
		//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	case 3:
		switch (passMaxVertical(contr)) // ย : 0, อ : 1, จ : 2 ฐ : 3, ถ = 4, บ = 5, ญ ั = 6, ภ : 7, จ : 8
		{
		case 0:
			check = 5;
			break;
		case 1:
			check = 6;
			break;
		case 2:
			check = 7;
			break;
		case 3:
			check = 8;
			break;
		case 4:
			check = 9;
			break;
		case 5:
			check = 10;
			break;
		case 6:
			check = 11;
			break;
		case 7:
			check = 12;
			break;
		case 8:
			check = 13;
			break;
		default:
			check = -1;
		}
		check = 9;
		//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	case 4:
		rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		check = 20;
		break;
	case 5: // พ : 4, ฑ : 3
		if (passHorizontalCount(contr, 3.0 / 4.0) == 3)
			check = 0;
		else
			check = 1;
		break;
	default:
		check = -1;
	}

	return check; // ฑ : 0, พ : 1, ง : 2, ฑ : 3, พ : 4, ย : 5, อ : 6, จ : 7 ฐ : 8, ถ = 9, บ = 10, ญ ั = 11, ภ : 12, จ : 13
}

int OneHead(vector<Point> contr)
{
	int up = findHole(UP, contr);
	int down = findHole(DOWN, contr);
	int left = findHole(LEFT, contr);
	int right = findHole(RIGHT, contr);
	Rect boundRect = boundingRect(contr);
	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);

	int check = -1;

	if ((check = longTail(contr)) != -1) // ป ฝ ฟ : 0 2 1
	{
		if (check == 1)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
		if (check == 2)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
		if (check == 0)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
	}
	else if ((check = passOneHorizontal(contr)) != -1) // ร ว : 1 0
	{
		if (check == 1)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
		if (check == 0)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
	}
	else if (temp.at<uchar>(temp.rows / 2, 0) < THRESHOLD && temp.at<uchar>(temp.rows - 1, 0) < THRESHOLD) // ผ
	{
		rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
	}
	else if ((check = passMaxHorizontal(contr)) != -1) // ฑ : 0, พ : 1, ง : 2, ฑ : 3, พ : 4, ย : 5, อ : 6, จ : 7 ฐ : 8, ถ = 9, บ = 10, ญ ั = 11, ภ : 12, จ : 13
	{
		if (check >= 0 && check <= 13)
			rectangle(inputImage, boundRect, Scalar(0, 0, 255), 2);
	}

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
			OneHead(contr[i]);
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
