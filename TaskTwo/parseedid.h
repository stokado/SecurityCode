#ifndef PARSE_EDID_H
#define PARSE_EDID_H

// For types of fixed size
#include <stdint.h>
#include <stdio.h>

// EDID maximum block size
#define BLOCK_SIZE 128

// Size of DTD block
#define DTD_SIZE 18
// Header size of EDIT
#define HEADER_SIZE 8
// First byte of DTD in EDID base
#define DTD_START 54
// Display descriptor DTD data ofsset
#define DTD_DATA_START 5
// Last byte of DTD
#define DTD_END 125

// Maximum length of monitor name
#define MAX_NAME 14

// Model year flag or week when manufactured
#define WEEK_BYTE 16
// Year of model or year when manufactured
#define YEAR_BYTE 17

// EDID version byte
#define VERSION_BYTE 18
// EDID revision byte
#define REVISION_BYTE 19

// Video input paramaters
#define VIDEO_INPUT_BYTE 20
// Horisontal size byte
#define HSIZE_BYTE 21
// Vertical size byte
#define VSIZE_BYTE 22

// Bitmap Features byte
#define BITMAP_FEATURES_BYTE 24
// Established timings I byte
#define ESTABLISHED_I_BYTE 35
// Established timings II byte 
#define ESTABLISHED_II_BYTE 36
// Standard timing first byte
#define STANDARD_START_BYTE 38
// Standard timings last byte
#define STANDARD_END_BYTE 53

enum headerFlags {
	headerEdge = 0x00,
	headerMiddle = 0xFF
};

enum dateFlags {
	modelYearFlag = 0xFF
};

enum basicFlags {
	isDigital = 0x80
};

enum bitmapFeaturesFlags {
	preferredSpecified = 0x02
};

enum standardFlags {
	reserved = 0x00,
	unused = 0x01
};

enum aspectRatio {
	sixteenByTen = 0x00,
	fourByThree = 0x01,
	fiveByFour = 0x02,
	sexteenByNine = 0x03
};

enum dtdType {
	display = 0xFC
};

void parseEdid(const unsigned char* edid);
void checkHeader(const unsigned char* base);
void parseModelName(const unsigned char* base);
void parseManufacturer(const unsigned char* base);
void parseManufactureDate(const unsigned char* base);
void parseEdidVersion(const unsigned char* base);
void parseBasicParameters(const unsigned char* base);
void parseTimings(const unsigned char* base);
void parseStandard(const unsigned char* base);
void parseEstablished(const unsigned char* base);

void parseExt(const unsigned char* ext);

void checkChecksum(const unsigned char* block);
void parsePreferred(const unsigned char* block, uint32_t start, uint16_t isNative);
#endif