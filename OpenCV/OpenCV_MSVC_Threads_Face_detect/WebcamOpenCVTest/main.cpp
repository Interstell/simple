#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include "emotions.hpp"
extern "C"{
	#include "thread.h"
	#include "mutex.h"
}
#include <Windows.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

const char IMG_NAME[] = "img.jpg";
const char FILLER_NAME[] = "ConsoleFillMSVC.exe";

typedef struct data_s{
	char* exePath;
	char* imgPath;
	char* fillerPath;
	Mat img;
}data_t;



void * analyzeImage(void * args){
	data_t* data = (data_t*)args;
	imwrite(data->imgPath, data->img); //writing current frame to file
	emotions_Analyze(data->exePath, data->imgPath);
	system(data->fillerPath);
	return NULL;
}

int main(int argc, char *argv[])
{
	char* exePath = (char*)malloc((strlen(argv[0]) + 1)*sizeof(char));
	strcpy(exePath, argv[0]);
	for (int i = strlen(exePath) - 1; i >= 0; i--){
		if (exePath[i] == '\\'){
			exePath[i + 1] = '\0';
			break;
		}
	}
	char* imgPath = (char*)malloc((strlen(argv[0]) + 60)*sizeof(char));
	char* fillerPath = (char*)malloc((strlen(argv[0]) + 60)*sizeof(char));
	sprintf(imgPath, "%s\\%s", exePath, IMG_NAME);
	sprintf(fillerPath, "%s\\%s", exePath, FILLER_NAME);
	VideoCapture cap;
	if (!cap.open(0)) //running camera and connecting
		return 0;

	// Load cascate classifier placed in sulution folder
	CascadeClassifier detector;
	string cascadeName = "haarcascade_frontalface_alt.xml";
	bool loaded = detector.load(cascadeName);

	// Parameters of detectMultiscale Cascade Classifier
	int groundThreshold = 2;
	double scaleStep = 1.1;
	Size minimalObjectSize(80, 80);
	Size maximalObjectSize(350, 350);

	// Vector of returned faces
	vector<Rect> found;
	data_t data;
	thread_t* analyzer;
	while (1)
	{
		Mat img; //matrix for frame
		cap >> img;
		if (img.empty()) break; // end of video stream

		// Convert input to greyscale 
		Mat image_grey;
		cvtColor(img, image_grey, CV_BGR2GRAY);

		// why not
		found.clear();

		// Detect faces
		detector.detectMultiScale(image_grey, found, scaleStep, groundThreshold, 0 | 2, minimalObjectSize, maximalObjectSize);
		
		int largestFace = 0;
		if (found.size() > 1)
			for (int i = 1; i < found.size(); i++)
				if (found[i].area() > found[0].area()) largestFace = i;

		// Draw the results into mat retrieved from webcam
		if (found.size() > 0) {
			for (int i = 0; i < found.size(); i++) {
				rectangle(img, found[i].br(), found[i].tl(), Scalar(0, (i == largestFace) ? 255 : 0, 0), 2, 4, 0);
			}
		}

		imshow("EmotionAnalyzer :)  Simple(c)", img);	//showing current frame

		int input = waitKey(20); //getting user input
		if (input == 27){ // stop capturing by pressing ESC 
			break; //exit
		}
		else if (input == 13){ //proceeding emotions
			system("cls");
			cap >> img;
			data.imgPath = imgPath;
			data.fillerPath = fillerPath;
			data.img = img;
			data.exePath = exePath;
			analyzer = thread_create_args(analyzeImage, &data);
			
			/*imwrite(imgPath, img); //writing current frame to file
			emotions_Analyze(exePath, imgPath);
			system(fillerPath);*/
		}
	}
	free(exePath);
	return 0;
}

