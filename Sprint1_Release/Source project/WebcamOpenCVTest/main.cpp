#include <opencv2\opencv.hpp>
#include <stdio.h>
#include "emotions.hpp"

const char IMG_NAME[] = "img.jpg";
const char FILLER_NAME[] = "ConsoleFillMSVC.exe";

using namespace cv;

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
	while (1)
	{
		Mat frame; //matrix for frame
		cap >> frame;
		if (frame.empty()) break; // end of video stream
		imshow("EmotionAnalyzer :)  Simple(c)", frame);	//showing current frame
		int input = waitKey(1); //getting user input
		if (input == 27){ // stop capturing by pressing ESC 
			break; //exit
		}
		else if (input == 13){ //proceeding emotions
			system("cls");
			imwrite(imgPath, frame); //writing current frame to file
			emotions_Analyze(exePath, imgPath);
			system(fillerPath);
		}
		 
	}

	free(exePath);
	return 0;
}
