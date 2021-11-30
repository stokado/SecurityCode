#include "parseedid.h"

const char established[16][20] = {
	"720x400 @ 70 Hz",
	"720x400 @ 88 Hz",
	"640x480 @ 60 Hz",
	"640x480 @ 67 Hz",
	"640x480 @ 72 Hz",
	"640x480 @ 75 Hz",
	"800x600 @ 56 Hz",
	"800x600 @ 60 Hz",
	"800x600 @ 72 Hz",
	"800x600 @ 75 Hz",
	"832x624 @ 75 Hz",
	"1024x768 @ 87 Hz",
	"1024x768 @ 60 Hz",
	"1024x768 @ 70 Hz",
	"1024x768 @ 75 Hz",
	"1280x1024 @ 75 Hz"
};

void parseEdid(const unsigned char* edid) {

	printf("***\tParsing Base body\t***\n\n");
	checkChecksum(edid);
	checkHeader(edid);
	parseModelName(edid);
	parseManufacturer(edid);
	parseManufactureDate(edid);
	parseEdidVersion(edid);
	parseBasicParameters(edid);
	parseTimings(edid);
	printf("***\tEnd of Base body\t***\n\n");
}

// Parse CEA EDID extension
void parseExt(const unsigned char* ext) {
	if (ext[0] == 0x02) {

		checkChecksum(ext);

		// Byte number withing this block where the DTD's begin
		uint32_t dtdStart = ext[2];
		// Total number of native formats in DTD's in current block
		uint32_t totalNative = ext[3] & 0x0F;
		uint32_t counter = 0;

		if (dtdStart != 0x00) {
			for (int i = dtdStart; i < DTD_END; i += DTD_SIZE) {
				if (ext[i] == 0x00) {
					break;
				}

				if (counter < totalNative) {
					parsePreferred(ext, i, 1);
					counter++;
				}
				else {
					parsePreferred(ext, i, 0);
				}
			}
		}
	}
	else {
		printf("Not a CEA extension block\n");
	}
	printf("\n");
}

// Check checksum
void checkChecksum(const unsigned char* block) {
	// Sum of all bytes should equal 0
	unsigned char sum = 0;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		sum += block[i];
	}
	if (sum) {
		printf("Checksum failed\n");
		return;
	}
	else {
		printf("Checksum OK\n");
	}
	printf("\n");
}

// Check header of EDID Base
void checkHeader(const unsigned char* base) {

	// Headers of EDID Base have fixed pattern
	// 00 FF FF FF FF FF FF 00
	// 0  1  2  3  4  5  6  7

	int i = 0;
	for (i = 0; i < HEADER_SIZE; i++) {
		if ((i == 0 || i == HEADER_SIZE - 1) && base[i] != headerEdge) {
			break;
		}
		else if ((i > 0 && i < HEADER_SIZE - 1) && base[i] != headerMiddle) {
			break;
		}
	}
	if (i < HEADER_SIZE) {
		printf("Header incorrect\n\n");
		return;
	}
	printf("Header OK\n\n");
}

// Model name may be stored in one of 4 DTDs
void parseModelName(const unsigned char* base) {

	printf("Display parameters\n\n");
	// Search for model name
	// Detailed Timing Descriptor -> Display Descriptor -> Display Name


	// Text must not exceed 13 bytes
	char monitorName[MAX_NAME] = { '\0' };

	// One descriptor size = 18 bytes (last element = byte 125)
	for (int i = DTD_START; i < DTD_END; i += DTD_SIZE) {

		// Specifically search for a Display descriptor
		// bytes 0 and 1 == 0
		if (base[i] == 0x00 && base[i + 1] == 0x00) {
			// 4th byte is a data type flag - if == FC - monitor name
			if (base[i + 3] == display) {
				// Parse monitor name from descriptor data (starts from 6th byte)
				for (int j = 0; j < 13; j++) {
					// If Monitor name is less than 13 symbols, terminated with LF symbol 
					if (base[DTD_DATA_START + i + j] == '\n') {
						monitorName[j] = 0x00;
						break;
					}
					else
						monitorName[j] = base[DTD_DATA_START + i + j];
				}
			}
		}
	}
	if (monitorName[0] != '\0') {
		printf("\tMonitor name: \"%s\"\n", monitorName);
	}
	else {
		printf("\tCouldn't find monitor name\n\n");
	}
}

// Parse Manufacturer ID
void parseManufacturer(const unsigned char* base) {
	// Manufacturer

	/* Bytes 8 and 9 (5 bits per letter and 1 bit is reserved)
	*  Letter 1: bits 14-10
	*  Letter 2: bits 9-5
	*  Letter 3: bits 4-0
	*/
	printf("\tManufacturer: \"%c%c%c\"\n",
		(base[8] >> 2 & 0x1F) + 'A' - 1, // We need bits 3-6 of byte 8 
		(((base[8] & 0x03) << 3) | ((base[9] & 0xE0) >> 5)) + 'A' - 1,	// We need 2 first bits of byte 8 and 3 last bits of byte 9
		(base[9] & 0x1F) + 'A' - 1	// First 5 bits of byte 9
	);
	printf("\n");
}

// Parse manufacture date
void parseManufactureDate(const unsigned char* base) {
	if (base[WEEK_BYTE] == modelYearFlag) {
		printf("\tYear of model: %i\n", (uint32_t)base[YEAR_BYTE] + 1990);
	}
	else {
		printf("\tMonitor manufactured: week %i of %i\n", (uint32_t)base[WEEK_BYTE], (uint32_t)base[YEAR_BYTE] + 1990);
	}
	printf("\n");
}

// EDID Version
void parseEdidVersion(const unsigned char* base) {
	printf("\tEDID version: %i.%i\n", (uint32_t)base[VERSION_BYTE], (uint32_t)base[REVISION_BYTE]);
	printf("\n");
}

//Basic Display Parameters
void parseBasicParameters(const unsigned char* base) {

	// Digital or analog
	if ((base[VIDEO_INPUT_BYTE] & 0x80) == isDigital)
		printf("\tDigital Display\n");
	else {
		printf("\tAnalog Display\n");
	}

	// Screen size 
	if (base[HSIZE_BYTE] && base[VSIZE_BYTE])
		printf("\tDisplay size: h:%i cm v:%i cm\n", (uint32_t)base[HSIZE_BYTE], (uint32_t)base[VSIZE_BYTE]);
	else
		printf("\tScreen size: not given. Probably a projector.\n");
	printf("\n");
}

// Parse preferred timing
void parsePreferred(const unsigned char* block, uint32_t start, uint16_t isNative) {
	// horisontal active pixels in byte 56 and 58 (bits 7-4)
	uint32_t hActive = block[start + 2] + ((block[start + 4] & 0xF0) << 4);
	// horisontal blank pixels in byte 57 and 58 (bits 3-0)
	uint32_t hBlank = block[start + 3] + ((block[start + 4] & 0x0F) << 8);
	// vertical active pixels in byte 59 and 61 (bits 7-4)
	uint32_t vActive = block[start + 5] + ((block[start + 7] & 0xF0) << 4);
	// horisontal blank pixels in byte 60 and 61 (bits 3-0)
	uint32_t vBlank = block[start + 6] + ((block[start + 7] & 0x0F) << 8);

	// Pixel clock bytes 54 and 55 little-endian
	uint32_t pixelClock = ((block[start + 1] << 8) | block[start]);

	uint64_t hTotal = (uint64_t)hActive + hBlank;
	uint64_t vTotal = (uint64_t)vActive + vBlank;

	// Refresh rate according to CVT v. 1.2
	double refreshRate = (pixelClock * 10000.0) / (hTotal * vTotal);
	if (isNative) {
		printf("\tNative: %ix%i @ %.2f Hz\n\n", hActive, vActive, refreshRate);
	}
	else {
		printf("\tPreferred mode: %ix%i @ %.2f Hz\n", hActive, vActive, refreshRate);
	}
}

// Parse standard timing information
void parseStandard(const unsigned char* base) {
	// Bytes 38-53 are up to 8 2-byte display modes
	for (uint32_t i = STANDARD_START_BYTE; i < STANDARD_END_BYTE; i += 0x02) {

		// Refresh rate
		uint32_t refresh = 0;
		// Horisontal resolution
		uint32_t hres = 0;
		// Vertical resolution
		uint32_t vres = 0;
		// Check if field is not empty
		if (!(base[i] == unused && base[i + 1] == unused)) {

			// Horisontal resolution is defined in first byte if it is not reserved
			if (!(base[i] == reserved)) {
				hres = ((int)base[i] + 31) * 8;
			}

			// Second byte stores inofmation about aspect ratio (bits 7, 6) 
			// and refresh rate (bits 5-0)

			// Vertical resolution is counted with Horisontal resolution and aspect ratio
			switch ((base[i + 1] & 0xC0) >> 6) {
			case sixteenByTen:
				vres = (hres * 10) / 16;	//	16:10	aspect ratio
				break;
			case fourByThree:
				vres = (hres * 3) / 4;	//	4:3		aspect ratio
				break;
			case fiveByFour:
				vres = (hres * 4) / 5;	//	5:4		aspect ratio
				break;
			case sexteenByNine:
				vres = (hres * 9) / 16;	//	16:9	aspect ratio
				break;
			}
			// Vertical frequency = data value (bits 5-0) + 60
			refresh = (base[i + 1] & 0x3F) + 60;
			printf("\tStandard mode: %ix%i @ %i Hz\n", hres, vres, refresh);
		}
	}
	printf("\n");
}

void parseEstablished(const unsigned char* base) {

	// Established 2
	for (int bit = 0; bit < 8; bit++) {
		if ((base[ESTABLISHED_II_BYTE] >> bit) & 0x01) {
			printf("\tEstablished II: %s\n", established[15 - bit]);
		}
	}
	printf("\n");
	// Established 1
	for (int bit = 0; bit < 8; bit++) {
		if ((base[ESTABLISHED_I_BYTE] >> bit) & 0x01) {
			printf("\tEstablished I: %s\n", established[7 - bit]);
		}
	}
	printf("\n");
}

// Timing information
void parseTimings(const unsigned char* base) {

	printf("Parsing timings\n\n");

	// If bit 1 of byte 24 is set then there is a preferred timing mode
	// In EDID verisons => 1.3, it is also a native mode and it must be in first DTD block
	if ((base[BITMAP_FEATURES_BYTE] & 0x02) == preferredSpecified) {
		parsePreferred(base, DTD_START, 1);
	}

	parseStandard(base);
	parseEstablished(base);
}
