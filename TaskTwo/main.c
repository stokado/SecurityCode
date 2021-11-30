/*
* Nazarov Gleb Maksimovich, 2021
* Parses EDID file and prints information in console.
*
* Programm takes path to an EDID binary as a command line argument.
*/

// For IO functions
#include "parseedid.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Error!\n");
		printf("Pass the path to an EDID file name as a command line argument\n");
		printf("Example\n");
		printf("./TaskTwo ../../Tests/TaskTwo/edid1.bin\n");
		return 4;
	}

	FILE* fp;
	// Read binary file
	fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		fputs("File error", stderr);
		return 1;
	}

	// Obtain file size
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	// Reset file pointer
	rewind(fp);

	// If size > BLOCKSIZE bytes then file contains extensions
	uint32_t extension = 0;
	if (size > BLOCK_SIZE) {
		// Number of extensions
		extension = size / BLOCK_SIZE - 1;
		// Set size of bytes to be read to BLOCKSIZE
		size /= (extension + 1);
	}

	// Allocate memory for a block of BLOCKSIZE
	unsigned char* edid = (unsigned char*)malloc(sizeof(unsigned char) * size);
	if (edid == NULL) {
		fputs("Memory error", stderr);
		return 2;
	}

	// Copy file into the buffer
	uint32_t result = fread(edid, 1, size, fp);
	if (result != size) {
		free(edid);
		fclose(fp);
		fputs("Reading error", stderr);
		return 3;
	}

	// Parse BASE body
	parseEdid(edid);

	// If there are any extenstions, parse them one block at the time
	if (extension > 0) {
		for (size_t i = 0; i < extension; i++) {
			result = fread(edid, 1, size, fp);
			if (result != size) {
				free(edid);
				fclose(fp);
				fputs("Reading error", stderr);
				return 3;
			}

			printf("***\tParsing extension block #%i\t***\n\n", i + 1);
			parseExt(edid);
			printf("***\tEnd of extension block #%i\t***\n", i + 1);
		}
	}

	free(edid);
	fclose(fp);
	return 0;
}