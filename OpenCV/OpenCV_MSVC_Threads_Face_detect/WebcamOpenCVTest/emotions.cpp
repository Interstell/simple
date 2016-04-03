#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jansson\jansson.h"

//RESULTS ARE SAVED TO emotions.txt

//# - no face detected
//@ - failed to upload image (wrong link)

void emotions_Analyze(char* path, char* link){
	//getting full path to .exe directory
	
	//requesting data from Emotion API using curl, saving JSON response to html
	char cmd[600];
	char exportFileName[400];
	sprintf(exportFileName, "%s\\emotions.json", path);
	char resultFileName[400];
	sprintf(resultFileName, "%s\\emotions.txt", path);
	sprintf(cmd, "%s\\curl.exe -o %s -XPOST https://api.projectoxford.ai/emotion/v1.0/recognize?subscription-key=349f1c7ff1e642498d333cb2fe5fab25 -k --data-binary @%s -H \"Content-Type:application/octet-stream\"", path, exportFileName, link);
	system(cmd);// WARNING! NO UPDATE IF COMMENTED
	system("cls");
	//parsing and structurizing JSON response, writing to stream
	FILE* fJSON = fopen(exportFileName, "r");
	FILE* output = fopen(resultFileName, "w");
	json_error_t error;
	json_t* root = json_loadf(fJSON, 0, &error);
	json_t* wrongURL = json_object_get(root, "error");
	if (json_is_object(wrongURL)){ //failed to download photo by API
		fprintf(output, "%s", "@"); 
		fprintf(stdout, "%s", "Download fail");
	}
	else if (json_array_size(root) == 0){//no face detected
		fprintf(output, "%s", "#"); 
		fprintf(stdout, "%s", "No faces on photo. Come back!");
	}

	int biggestFace = 0;
	int biggestArea = 0;

	for (int i = 0; i < json_array_size(root); i++){
		json_t* data = json_array_get(root, i);
		json_t* faceRectangle = json_object_get(data, "faceRectangle");
		json_t* width = json_object_get(faceRectangle, "width");
		json_t* height = json_object_get(faceRectangle, "height");
		int area = json_integer_value(width) * json_integer_value(height);
		if (area > biggestArea) {
			biggestArea = area;
			biggestFace = i;
		}
	}

	json_t* data = json_array_get(root, biggestFace);
	json_t* scores = json_object_get(data, "scores");
	json_t* anger = json_object_get(scores, "anger");
	json_t* contempt = json_object_get(scores, "contempt");
	json_t* disgust = json_object_get(scores, "disgust");
	json_t* fear = json_object_get(scores, "fear");
	json_t* happiness = json_object_get(scores, "happiness");
	json_t* neutral = json_object_get(scores, "neutral");
	json_t* sadness = json_object_get(scores, "sadness");
	json_t* surprise = json_object_get(scores, "surprise");
	puts("Your emotions:\n");
	fprintf(stdout, "Anger: %.3lf %%\n", json_real_value(anger)*100);
	fprintf(stdout, "Contempt: %.3lf %%\n", json_real_value(contempt) * 100);
	fprintf(stdout, "Disgust: %.3lf %%\n", json_real_value(disgust) * 100);
	fprintf(stdout, "Fear: %.3lf %%\n", json_real_value(fear) * 100);
	fprintf(stdout, "Happiness: %.3lf %%\n", json_real_value(happiness) * 100);
	fprintf(stdout, "Neutral: %.3lf %%\n", json_real_value(neutral) * 100);
	fprintf(stdout, "Sadness: %.3lf %%\n", json_real_value(sadness) * 100);
	fprintf(stdout, "Surprise: %.3lf %%\n", json_real_value(surprise)*100);
	fprintf(output, "%.15lf\n", json_real_value(anger));
	fprintf(output, "%.15lf\n", json_real_value(contempt));
	fprintf(output, "%.15lf\n", json_real_value(disgust));
	fprintf(output, "%.15lf\n", json_real_value(fear));
	fprintf(output, "%.15lf\n", json_real_value(happiness));
	fprintf(output, "%.15lf\n", (json_real_value(neutral)>0.93) ? json_real_value(neutral):0.0);
	fprintf(output, "%.15lf\n", json_real_value(sadness));
	fprintf(output, "%.15lf\n", json_real_value(surprise));
	puts("");

	fclose(fJSON);
	fclose(output);
}