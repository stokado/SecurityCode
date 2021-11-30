/*
* Nazarov Gleb Maksimovich, 2021
* This programm finds integer numbers in JSON file (or from memory) and prints their values and nesting level in console.
* 
* Programm takes path to a JSON file as a command line argument.
*/

#include "parsejson.h"

const char* json = "{\n\"obj1\": {\n\"val0\": \"test\",\n\"val1\": 1337,\n\"val3\": true,\n\"obj2\": {\n\"val22\": 5,\n\"obj3\": {\n\"val31\": 20\n}\n},\n\"arr3\": [\n-1,\n-296,\n500,\n829,\n0\n]\n},\n\"last\": 777\n";


int main(int argc, char* argv[]) {
	
	// JSON will be parsed line by line, so prepare string for next line
	char singleLine[MAXLEN];
	// Prepare level variable to store current level
	int lvl = 0;

	if (argc < 2) {
		printf("Parsing JSON from memory\n");

		// Parse JSON line by line
		while (NULL != sgets(singleLine, MAXLEN, &json)) {
			int size = findSize(singleLine);
			parseLine(singleLine, size, &lvl);
		}
		printf("Next time try to parse JSON from a file\n");
		printf("Example: ./TaskOne ../../Test/TaskOne/test.json\n");
		return 0;
	}
	else {
		printf("Parsing JSON from file: %s\n\n", argv[1]);
		FILE* fp;
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			fputs("File open error", stderr);
			return 2;
		}

		// Read file line by line untill EOF symbol
		while (!feof(fp)) {
			fgets(singleLine, MAXLEN, fp);
			int size = findSize(singleLine);
			parseLine(singleLine, size, &lvl);
		}
		fclose(fp);
	}
	return 0;
}