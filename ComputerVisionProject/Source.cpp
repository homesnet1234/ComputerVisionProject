#include <iostream>
#include "opencv2/opencv.hpp"
#define endl '\n'
#define THRESHOLD 100
using namespace std;
using namespace cv;

Mat inputImage, outputImage;
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
int midRound(vector<Point> contr);
int roughHead(vector<Point> contr);
int cutShabShab(int mode, vector<Point> con);

// ======================================================

int roughHead(vector<Point> contr) {
	Rect boundRect = boundingRect(contr);

	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);
	int check = -1; //-1 is not this type : 0 is ฃ,1 is ซ, 2 is ท
	bool black = true;
	int countHead = 0;
	int countMiddle = 0;
	int countLow = 0;

	//Find specific type
	for (int i = 0; i <= temp.cols / 3; i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.15), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.15), i) < THRESHOLD && black)
		{
			black = false;
			countHead++;
		}
	}

	black = true;
	for (int i = 0; i <= (int)(temp.cols*0.3); i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.6), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.6), i) < THRESHOLD && black)
		{
			black = false;
			countMiddle++;
		}
	}

	black = true;
	for (int i = 0; i <= (int)(temp.cols - 1); i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.8), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.8), i) < THRESHOLD && black)
		{
			black = false;
			countLow++;
		}
	}

	//Specified
	if (countHead == 2 && countLow == 2)
	{
		if (countMiddle == 1) {
			black = true;
			int tbTest = 0;
			for (int i = 0; i <= (int)(temp.cols / 2); i++)
			{
				if (temp.at<uchar>((int)(temp.rows*0.5), i) > THRESHOLD && !black)
				{
					black = true;
				}
				else if (temp.at<uchar>((int)(temp.rows*0.8), i) < THRESHOLD && black)
				{
					black = false;
					tbTest++;
				}
			}
			if (tbTest == 2)
				check = 2;

		}
		else {
			int ccTest = 0;
			if (temp.at<uchar>((int)(temp.rows*0.22), (int)(temp.cols*0.98)) < THRESHOLD)
				ccTest++;

			if (ccTest == 1) {
				check = 0;

			}
			else
				check = 1;

		}
	}
	return check;
} //-1 is not this type : 0 is ฃ,1 is ซ, 2 is ท, 3 is ด

int midRound(vector<Point> contr) {
	Rect boundRect = boundingRect(contr);

	Mat temp = inputImage(boundRect);
	cvtColor(temp, temp, CV_BGR2GRAY);
	int check = -1; //-1 is not this type : 0 is ข,1 is ช, 2 is ค, 3 is ต, 4 is ด, 5 is ต, 6 is ญ
	bool black = true;
	int countMidHead = 0;
	int countHead = 0;
	int countLow = 0;

	//Find specific type
	for (int i = (int)(temp.cols*0.0); i <= (int)(temp.cols - 1); i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.00), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.00), i) < THRESHOLD && black)
		{
			black = false;
			countHead++;
		}
	}
	//putText(inputImage, to_string(countHead), Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 255), 2);

	black = true;
	for (int i = (int)(temp.cols*0.3); i <= (int)(temp.cols*0.7); i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.45), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.45), i) < THRESHOLD && black)
		{
			black = false;
			countMidHead++;
		}
	}

	//putText(inputImage, to_string(countMidHead), Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 255), 2);

	black = true;
	for (int i = (int)(temp.cols*0.0); i <= (int)(temp.cols*0.7); i++)
	{
		if (temp.at<uchar>((int)(temp.rows*0.75), i) > THRESHOLD && !black)
		{
			black = true;
		}
		else if (temp.at<uchar>((int)(temp.rows*0.75), i) < THRESHOLD && black)
		{
			black = false;
			countLow++;
		}
	}

	//putText(inputImage, to_string(countLow), Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 255), 2);

	if (countMidHead == 1) {
		if (countHead == 1) {
			if (countLow == 3) {
				check = 6; //ญ
			}
			else {
				int ccTest = 0;
				if (temp.at<uchar>((int)(temp.rows*0.22), (int)(temp.cols*0.98)) < THRESHOLD)
					ccTest++;

				if (ccTest == 1) {
					check = 0; //ข
				}
				else
					check = 1; //ฃ
			}

			//putText(inputImage, to_string(check), Point(boundRect.x, boundRect.y), 1, 2, Scalar(0, 0, 255), 2);
		}
	}
	else if (countMidHead == 2) {
		if (countLow == 1) {
			if (countHead == 1)
				check = 2; //ค
			else
				check = 3; //ฅ
		}
		else {
			if (countHead == 1)
				check = 4; //ด
			else
				check = 5; //ต
		}
	}

	if (check == 0) // fix ค
	{
		for (int i = 0; i < temp.rows / 2; i++)
		{
			if (temp.at<uchar>(i, temp.cols * 2 / 3) < THRESHOLD)
			{
				check = 2;
				break;
			}
		}
	}

	if (check == 1) // fix ด
	{
		if (passHorizontalCount(contr, 1.0 / 2.0) != 2)
			check = 4;
	}

	if (check == 3) // add ฃ
	{
		bool chk = true;
		for (int i = 0; i < temp.rows / 2; i++)
		{
			if (temp.at<uchar>(i, temp.cols * 3 / 4) < THRESHOLD)
			{
				chk = false;
				break;
			}
		}
		if (chk)
			check = 7;
	}

	return check;
} //0 is ข, 1 is ช, 2 is ค, 3 is ต, 4 is ด, 5 is ต, 6 is ญ, 7 is ฃ

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

	for (int i = temp.rows - 1; i >= temp.rows / 2; i--)
	{
		if (temp.at<uchar>(i, temp.cols / 2) < THRESHOLD)
		{
			check = 23;
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
		else if (temp.rows >= temp.cols) // จ
		{
			check = 8;
		}
		else
			check = 6;
		//rectangle(inputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	case 4:
		switch (passVerticalCount(contr, 3.0 / 4.0))
		{
		case 1: // ย
			if (NoHead(contr) == 0) // ภ
				check = 7;
			else
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
			check = 3;
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

	return check; // ย : 0, อ : 1, ล : 2 ฐ : 3, ถ = 4, บ = 5, ญ ั = 6, ภ : 7, จ : 8
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
	case 3: // ย : 0, อ : 1, ล : 2, ฐ : 3, ถ : 4, บ : 5, ญ ั : 6, ภ : 7, จ : 8
		switch (passMaxVertical(contr))
		{
		case 0:
			check = 3;
			break;
		case 1:
			check = 4;
			break;
		case 2:
			check = 5;
			break;
		case 3:
			check = 6;
			break;
		case 4:
			check = 7;
			break;
		case 5:
			check = 8;
			break;
		case 6:
			check = 9;
			break;
		case 7:
			check = 10;
			break;
		case 8:
			check = 11;
			break;
		default:
			check = -1;
		}
		//rectangle(outputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		break;
	case 4:
		//rectangle(outputImage, boundRect, Scalar(255, 255, 0), 2, LINE_8, 0);
		switch (roughHead(contr)) // ฃ : 0, ซ = 1, ท = 2
		{
		case 0:
			check = 19;
			break;
		case 1:
			check = 20;
			break;
		case 2:
			check = 21;
			break;
		case 3:
			check = 16;
			break;
		default:
			switch (midRound(contr)) //0 is ข, 1 is ช, 2 is ค, 3 is ฅ, 4 is ด, 5 is ต, 6 is ญ
			{
			case 0:
				check = 12;
				break;
			case 1:
				check = 13;
				break;
			case 2:
				check = 14;
				break;
			case 3:
				check = 15;
				break;
			case 4:
				check = 16;
				break;
			case 5:
				check = 17;
				break;
			case 6:
				check = 18;
				break;
			case 7:
				check = 19;
				break;
			default:
				check = -1;
			}
		}
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
	return check;	/*ฑ : 0, พ : 1, ง : 2, ย : 3, อ : 4, ล : 5 ฐ : 6, ถ = 7, บ = 8, ญ ั = 9,
					ภ : 10, จ : 11, ข : 12, ช : 13, ค : 14, ฅ : 15, ด : 16, ต : 17, ญ = 18, ฃ = 19, ซ : 20, ท : 21*/
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
	int buffer;

	if ((buffer = longTail(contr)) != -1) // ป ฝ ฟ : 0 2 1
	{
		if (buffer == 1)
			check = 30;
		else if (buffer == 2)
			check = 28;
		else if (buffer == 0)
			check = 26;
	}
	else if ((buffer = passOneHorizontal(contr)) != -1) // ร ว : 1 0
	{
		if (buffer == 1)
			check = 34;
		else if (buffer == 0)
			check = 36;
	}
	else if (temp.at<uchar>(temp.rows / 2, 0) < THRESHOLD && temp.at<uchar>(temp.rows - 1, 0) < THRESHOLD) // ผ
	{
		check = 27;
	}
	else if ((buffer = passMaxHorizontal(contr)) != -1) /*ฑ : 0, พ : 1, ง : 2, ย : 3, อ : 4, ล : 5, ฐ : 6, ถ = 7, บ = 8, ญ ั = 9,
														ภ : 10, จ : 11, ข : 12, ช : 13, ค : 14, ฅ : 15, ด : 16, ต : 17, ญ = 18, ฃ = 19, ซ : 20, ท : 21*/
	{
		if (buffer == 0)
			check = 16;
		else if (buffer == 1)
			check = 29;
		else if (buffer == 2)
			check = 6;
		else if (buffer == 3)
			check = 33;
		else if (buffer == 4)
			check = 42;
		else if (buffer == 5)
			check = 35;
		else if (buffer == 6)
			check = 15;
		else if (buffer == 7)
			check = 21;
		else if (buffer == 8)
			check = 25;
		else if (buffer == 9)
			check = 100;
		else if (buffer == 10)
			check = 31;
		else if (buffer == 11)
			check = 7;
		else if (buffer == 12)
			check = 1;
		else if (buffer == 13)
			check = 9;
		else if (buffer == 14)
			check = 3;
		else if (buffer == 15)
			check = 4;
		else if (buffer == 16)
			check = 19;
		else if (buffer == 17)
			check = 20;
		else if (buffer == 18)
			check = 12;
		else if (buffer == 19)
			check = 2;
		else if (buffer == 20)
			check = 10;
		else if (buffer == 21)
			check = 22;
	}

	return check;
}

int cutShabShab(int mode, vector<Point> con) {
	int xs, ys, xf, yf;

	switch (mode) {
	case 1: //check up
		xs = boundingRect(con).x;
		ys = boundingRect(con).y;
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + (boundingRect(con).height * 55 / 100);
		break;
	case 11:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height * 11 / 15);
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 12: case 13: //check down
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height / 2);
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 21:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height * 14 / 15);
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 23:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height * 2 / 3);
		xf = boundingRect(con).x + (boundingRect(con).width);
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 31:
		xs = boundingRect(con).x + (boundingRect(con).width * 2 / 3);
		ys = boundingRect(con).y;
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + (boundingRect(con).height / 3);
		break;
	case 32:
		xs = boundingRect(con).x + (boundingRect(con).width * 2 / 3);
		ys = boundingRect(con).y + (boundingRect(con).height * 2 / 3);
		xf = boundingRect(con).x + boundingRect(con).width;
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 33:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y;
		xf = boundingRect(con).x + (boundingRect(con).width / 2);
		yf = boundingRect(con).y + (boundingRect(con).height / 2);
		break;
	case 34:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y;
		xf = boundingRect(con).x + (boundingRect(con).width / 4);
		yf = boundingRect(con).y + (boundingRect(con).height / 4);
		break;
	case 35:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height * 15 / 21);
		xf = boundingRect(con).x + (boundingRect(con).width / 2);
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 36:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y + (boundingRect(con).height * 2 / 3);
		xf = boundingRect(con).x + (boundingRect(con).width * 2 / 3);
		yf = boundingRect(con).y + boundingRect(con).height;
		break;
	case 37:
		xs = boundingRect(con).x;
		ys = boundingRect(con).y;
		xf = boundingRect(con).x + (boundingRect(con).width / 3);
		yf = boundingRect(con).y + (boundingRect(con).height / 3);
		break;
	}

	Mat temp = inputImage.colRange(xs, xf).rowRange(ys, yf).clone();
	resize(temp, temp, Size(temp.cols * 10, temp.rows * 10), 0, 0, INTER_LINEAR);
	threshold(temp, temp, 150, 255, THRESH_BINARY);
	copyMakeBorder(temp, temp, 50, 50, 50, 50, BORDER_CONSTANT, Scalar(255, 255, 255));

	Mat temp2 = temp.clone();
	cvtColor(temp2, temp2, COLOR_BGR2GRAY);
	bitwise_not(temp2, temp2);
	Mat temp3 = temp2.clone();

	vector<vector<Point>> concon;
	vector<vector<Point>> conconcon;
	findContours(temp2, concon, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	findContours(temp2, conconcon, RETR_TREE, CHAIN_APPROX_NONE);

	if (mode > 20) {
		return (mode < 30) ? (int)concon.size() : (int)conconcon.size();
	}
	else {
		/*if (concon.size() == 2) {
			imshow("s", temp);
			waitKey(0);
		}*/
		return (int)concon.size();
	}
}

int TwoHead(vector<Point> contr)
{
	Rect boundRect = boundingRect(contr);
	int check = cutShabShab(1, contr);
	switch (check) {
	case 1: // { ฎ, ฏ, ศ ,ส }
		check = cutShabShab(11, contr);
		switch (check) {
		case 1:
			if (cutShabShab(21, contr) == 2) //ฎ
				return 13;
			else //ฏ
				return 14;
			break;
		default:
			if (cutShabShab(36, contr) == 2) //ส
				return 39;
			else //ศ
				return 37;
			break;
		}
		break;


	case 2: // { ฆ, ฉ, ฌ, ณ, น, ม, ห, ฮ, ฐาน(ฐ) }
		check = cutShabShab(12, contr);
		if (check == 2) {
			//ณ
			if (cutShabShab(32, contr) == 2)
				return 18;
			else {
				//ห
				if (cutShabShab(31, contr) == 2)
					return 40;
				else
					//ฌ
					if (cutShabShab(33, contr) == 1)
						return 11;
				//ฆ
					else
						return 5;
			}
		}
		else {
			if (cutShabShab(32, contr) == 2) {
				//ฉ
				if (cutShabShab(37, contr) == 1)
					return 8;
				//น
				else
					return 24;
			}
			else
				if (cutShabShab(34, contr) == 1)
					//ม
					if (cutShabShab(35, contr) == 2)
						return 32;
			//ฮ
					else
						return 43;
			//ฐาน ของ ฐ
				else
					return 101;
		}
		break;


	case 3: // { ฒ, ษ, ฬ }
		check = cutShabShab(13, contr);
		// ฒ
		if (check == 2)
			return 17;
		else {
			//ษ
			if (cutShabShab(23, contr) == 1)
				return 38;
			//ฬ
			else
				return  41;
		}
		break;
	}
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
		Rect boundRect = boundingRect(contr[i]);
		string a;
		switch (overall)
		{
			//            case 1:
			//                rectangle(input,boundingRect(contr[i]),Scalar(0,255,0),1,LINE_8,0);
			//                break;
		case 0:
			a = to_string(NoHead(contr[i]));
			putText(outputImage, a, Point(boundRect.x, boundRect.y), 1, 1, Scalar(0, 0, 255), 1.8);
			rectangle(outputImage, boundRect, Scalar(255, 0, 0), 2);
			break;
		case 1:
			a = to_string(OneHead(contr[i]));
			putText(outputImage, a, Point(boundRect.x, boundRect.y), 1, 1, Scalar(0, 0, 255), 1.8);
			rectangle(outputImage, boundRect, Scalar(0, 255, 0), 2);
			break;
		case 2:
			a = to_string(TwoHead(contr[i]));
			putText(outputImage, a, Point(boundRect.x, boundRect.y), 1, 1, Scalar(0, 0, 255), 1.8);
			rectangle(outputImage, boundRect, Scalar(0, 255, 255), 2);
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

	inputImage = imread("test1.png");
	outputImage = inputImage.clone();
	threshold(inputImage, inputImage, THRESHOLD, 255, THRESH_BINARY);

	findOverAllHole();
	imshow("thai catch", inputImage);
	imshow("output", outputImage);
	waitKey(0);
}