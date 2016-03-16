#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson\jansson.h>

//# - no face detected
//@ - failed to upload image (wrong link)

int main(int argc, char *argv[]){
	char* link;
	//getting photo url as a command line argument
	if (argc > 1){
		link = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
		strcpy(link, argv[1]);
	}
	else exit(EXIT_FAILURE);
	//getting full path to .exe directory
	char* path = (char*)malloc(sizeof(char)*(strlen(argv[0])+1));
	strcpy(path, argv[0]);
	for (int i = strlen(path) - 1 ; i >= 0; i--){
		if (path[i] == '\\'){
			path[i + 1] = '\0';
			break;
		}
	}
	//requesting data from Emotion API using curl, saving JSON response to html
	char cmd[400];
	char exportFileName[] = "emotions.json";
	sprintf(cmd, "%s\\curl.exe -o %s -XPOST https://api.projectoxford.ai/emotion/v1.0/recognize?subscription-key=349f1c7ff1e642498d333cb2fe5fab25 -k -d \"{'url':'%s'}\" -H \"Content-Type:application/json\"", path, exportFileName, link);
	//system(cmd);// WARNING! NO UPDATE IF COMMENTED
	//parsing and structurizing JSON response, writing to stream
	FILE* fJSON = fopen("emotions.json", "r");
	FILE* output = fopen("emotions.txt", "w");
	json_error_t error;
	json_t* root = json_loadf(fJSON, 0, &error);
	json_t* wrongURL = json_object_get(root, "error");
	if (json_is_object(wrongURL)){
		fprintf(output, "%s", "@"); //failed to download photo by API
	}
	else if (json_array_size(root) == 0) fprintf(output, "%s", "#"); //no face detected
	for (int i = 0; i < json_array_size(root); i++){
		json_t* data = json_array_get(root, i);
		json_t* error = json_object_get(data, "error");
		json_t* scores = json_object_get(data, "scores");
		json_t* anger = json_object_get(scores, "anger");
		json_t* contempt = json_object_get(scores, "contempt");
		json_t* disgust = json_object_get(scores, "disgust");
		json_t* fear = json_object_get(scores, "fear");
		json_t* happiness = json_object_get(scores, "happiness");
		json_t* neutral = json_object_get(scores, "neutral");
		json_t* sadness = json_object_get(scores, "sadness");
		json_t* surprise = json_object_get(scores, "surprise");
		fprintf(output, "%.15lf\n", json_real_value(anger));
		fprintf(output, "%.15lf\n", json_real_value(contempt));
		fprintf(output, "%.15lf\n", json_real_value(disgust));
		fprintf(output, "%.15lf\n", json_real_value(fear));
		fprintf(output, "%.15lf\n", json_real_value(happiness));
		fprintf(output, "%.15lf\n", json_real_value(neutral));
		fprintf(output, "%.15lf\n", json_real_value(sadness));
		fprintf(output, "%.15lf\n", json_real_value(surprise));
		puts("");
	}
	
	
	fclose(fJSON);
	fclose(output);
	free(link);
	free(path);

	return EXIT_SUCCESS;
}