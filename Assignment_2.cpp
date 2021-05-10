#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat imgOriginal;
Mat imgHSV;
Mat imgThresholded;
Mat imgYellow, imgRed, imgBlue;
vector<Vec3f> balls;
Vec3b PixelMean;
string imageTitle;

int i = 0;

int YHLow, YHHigh, YSLow, YSHigh, YVLow, YVHigh;
int RHLow, RHHigh, RSLow, RSHigh, RVLow, RVHigh;
int BHLow, BHHigh, BSLow, BSHigh, BVLow, BVHigh;
int ROIxMin, ROIxMax, ROIyMin, ROIyMax;
int Red1, Red2, Red3;
int Blue1, Blue2, Blue3;
int Yellow1, Yellow2, Yellow3;
int HThres, LThres;
int minDis, minRad, maxRad;

void imageProcess();

int ImageData[5][36] = {    
	{31, 0, 142, 60, 255, 255,0, 186, 69, 179, 213, 255,53, 206, 51, 179, 255, 255,300, 700, 50, 470,100, 100, 100,90, 50, 30,100, 100, 100,150, 50,15,10,40},
	{153, 148, 128, 179, 227,255,90,121,0, 114,255, 255,22, 63, 148, 89, 198,255,260,700,170, 470,110, 100, 100, 100, 130, 50, 140, 180, 110,150,50,15,10,40},
	{21,142,146,43, 255,255, 0, 148, 75, 7, 255, 255, 92, 178, 8, 150, 255, 225, 260, 700, 50, 470, 50, 40, 70,20,30,20, 10, 160, 160, 126, 42,15,5,40},
	{ 18, 194, 57, 35, 255, 255, 0, 190, 69, 6, 255, 255, 107, 176, 45, 112, 255, 255, 260, 700, 50, 470, 60, 65, 67, 34, 85, 88, 100, 100, 100, 82, 34,15,10,40},
	{ 17, 190, 99, 49, 255, 255, 0, 144, 81, 8, 215, 190, 105, 168, 24, 157, 255, 204, 260, 600, 170, 470, 100, 100, 100, 66, 70, 61, 100, 100, 100, 100,33,30,10,40} 
};

int* MainArray[36] = {
	&YHLow, &YSLow, &YVLow, &YHHigh, &YSHigh, &YVHigh,
	&RHLow, &RSLow, &RVLow, &RHHigh, &RSHigh, &RVHigh,
	&BHLow, &BSLow, &BVLow, &BHHigh, &BSHigh, &BVHigh,
	&ROIxMin, &ROIxMax, &ROIyMin, &ROIyMax,
	&Red1, &Red2, &Red3,
	&Blue1, &Blue2, &Blue3,
	&Yellow1, &Yellow2,&Yellow3,
	&HThres, &LThres,
	&minDis, &minRad, &maxRad
};

int main(int argc, char** argv)
{
	while (true){
		namedWindow("waitkey window", CV_WINDOW_NORMAL);

		if (imageTitle == ""){
			cout << "please select an image?\npress 1 for image 1\npress 2 for image 2\npress 3 for image 3\npress 4 for image 4\npress 5 for image 5" << endl;
		}
		else{
			cout << "The last selected image was " << imageTitle << endl << "please select another image, or press Esc to finish\n" << endl << "press 1 for image 1" << endl << "press 2 for image 2" << endl << "press 3 for image 3" << endl << "press 4 for image 4" << endl << "press 5 for image 5" << endl;
		}
		switch (waitKey(0)){
		case 49: //'t' has been pressed. this will toggle tracking
			imageTitle = "Image1";
			for (int k = 0; k < 36; k++){
				*MainArray[k] = ImageData[0][k];
			}
			imageProcess();
			break;
		case 50:
			imageTitle = "Image2";
			for (int k = 0; k < 36; k++){
				*MainArray[k] = ImageData[1][k];
			}
			imageProcess();
			break;
		case 51:
			imageTitle = "Image3";
			for (int k = 0; k < 36; k++){
				*MainArray[k] = ImageData[2][k];
			}
			imageProcess();
			break;
		case 52:
			imageTitle = "Image4";
			for (int k = 0; k < 36; k++){
				*MainArray[k] = ImageData[3][k];
			}
			imageProcess();
			break;
		case 53:
			imageTitle = "Image5";
			for (int k = 0; k < 36; k++){
				*MainArray[k] = ImageData[4][k];
			}
			imageProcess();
			break;
		case 27: //'esc' key has been pressed, exit program.
			return 0;
		}
	}
	return 0;
}

void imageProcess(){
	while (1){

		cout << endl << "Press Esc to select another Image\n" << endl;
		imgOriginal = imread(imageTitle, CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'
		resize(imgOriginal, imgOriginal, Size(), 0.3, 0.3, CV_INTER_AREA);

		if (imgOriginal.empty()) //check whether the image is loaded or not
		{
			cout << "Error : Image cannot be loaded..!!" << endl;
			system("pause"); //wait for a key press
			break;
		}

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	
		//Thresholding
		inRange(imgHSV, Scalar(YHLow, YSLow, YVLow), Scalar(YHHigh, YSHigh, YVHigh), imgYellow);
		inRange(imgHSV, Scalar(RHLow, RSLow, RVLow), Scalar( RHHigh, RSHigh, RVHigh), imgRed);
		inRange(imgHSV, Scalar(BHLow, BSLow, BVLow), Scalar( BHHigh, BSHigh,  BVHigh), imgBlue);
		add(imgBlue, imgRed, imgThresholded);
		add(imgThresholded, imgYellow, imgThresholded);

		//morphological Opening was not needed.
		//morphological closing to full holes in balls
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// smooth the processed image, this will make it easier for the next function to pick out the circles
		GaussianBlur(imgThresholded, imgThresholded, Size(7, 7), 2, 2);

		// fill sequential structure with all circles in processed image
		HoughCircles(imgThresholded,		// input image, nothe that this has to be grayscale (no color)
			balls,			    // provide function with memory storage, makes function return a pointer to a CvSeq
			CV_HOUGH_GRADIENT,	// two-pass algorithm for detecting circles, this is the only choice available
			2,					// size of image / 2 = "accumulator resolution", i.e. accum = res = size of image / 2
			minDis,	            // min distance in pixels between the centers of the detected circles
			HThres,             // high threshold of Canny edge detector, called by cvHoughCircles
			LThres,	            // low threshold of Canny edge detector, called by cvHoughCircles
			minRad,	  		    // min circle radius, in pixels
			maxRad);

		for (size_t i = 0; i < balls.size(); i++){
			Point center(cvRound(balls[i][0]), cvRound(balls[i][1]));
			int y = center.y;
			int x = center.x;

			if ((ROIxMin < x) && (x < ROIxMax) && (ROIyMin < y) && (y < ROIyMax)){ 
				
				for (int m = x - 5; m < x + 5; m++){
					for (int n = y - 5; n < y + 5; n++){
						Vec3b Pixelcolor = imgOriginal.at<Vec3b>(n, m);
						PixelMean.val[0] = (Pixelcolor.val[0] + PixelMean.val[0]) / 2;
						PixelMean.val[1] = (Pixelcolor.val[1] + PixelMean.val[1]) / 2;
						PixelMean.val[2] = (Pixelcolor.val[2] + PixelMean.val[2]) / 2;
					}
				}
				int radius = int(balls[i][2]);
				//cout << PixelMean << endl;
				if (PixelMean.val[0] < Red1 && PixelMean.val[1] < Red2 && PixelMean.val[2] > Red3){
					string text = "RED";
					int fontFace = FONT_HERSHEY_PLAIN;
					double fontScale = 0.75;
					int thickness = 2;
					int X = center.x - 20;
					int Y = center.y;
					Point textOrg(X, Y);
					putText(imgOriginal, text, textOrg, fontFace, fontScale, Scalar(0, 0, 255), thickness, 8);
					circle(imgOriginal, center, 3, Scalar(0, 0, 255), -1, 8, 0);
					circle(imgOriginal, center, radius, Scalar(0, 0, 255), 3, 8, 0);
				}
				if ((PixelMean.val[0] > Blue1 && PixelMean.val[1] < Blue2 && PixelMean.val[2] < Blue3) 
					|| (PixelMean.val[0] == 1 && PixelMean.val[1] == 252  && PixelMean.val[2] ==251)){ 
					string text = "BLUE";
					int fontFace = FONT_HERSHEY_PLAIN;
					double fontScale = 0.75;
					int thickness = 2;
					int X = center.x - 20;
					int Y = center.y;
					Point textOrg(X, Y);
					putText(imgOriginal, text, textOrg, fontFace, fontScale, Scalar(255, 0, 0), thickness, 8);
					circle(imgOriginal, center, 3, Scalar(255, 0, 0), -1, 8, 0);
					circle(imgOriginal, center, radius, Scalar(255, 0, 0), 3, 8, 0);
				}
				if (PixelMean.val[0] < Yellow1 && PixelMean.val[1] > Yellow2 && PixelMean.val[2] > Yellow3 
					&& (PixelMean.val[0] != 1 && PixelMean.val[1] != 252 && PixelMean.val[2] != 251)){
					string text = "YELLOW";
					int fontFace = FONT_HERSHEY_PLAIN;
					double fontScale = 0.75;
					int thickness = 2;
					int X = center.x - 30;
					int Y = center.y;
					Point textOrg(X, Y);
					putText(imgOriginal, text, textOrg, fontFace, fontScale, Scalar(0, 255, 255), thickness, 8);
					circle(imgOriginal, center, 3, Scalar(0, 255, 255), -1, 8, 0);
					circle(imgOriginal, center, radius, Scalar(0, 255, 255), 3, 8, 0);
				}
			}
		}

		imshow("Thresholded Image4", imgThresholded); //show the thresholded image
		imshow(imageTitle, imgOriginal); //show the original image

		if (waitKey(0) == 27) {//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			cout << "esc key is pressed by user\n" << endl;
			destroyAllWindows();
			//i++;
			break;
		}
	}
}
