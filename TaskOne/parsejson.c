#include "parsejson.h"


// Function to count nesting level (str - string to be parsed, size - size of str, *lvl - current level)
void countLvl(const char* str, int size, int* lvl) {
	// Iterate through whol string
	for (int i = 0; i < size; i++) {

		// If open bracket is found, then increment current level value
		if (str[i] == '{' || str[i] == '[') {
			(*lvl)++;
			return;
		}
		// If close bracket is found, then decrement current level value
		else if (str[i] == '}' || str[i] == ']') {
			(*lvl)--;
			return;
		}
	}
}

// Function to find integer object in str with size of size
int findInt(const char* str, int size) {

	// This variable stores quotes number
	int quotesNumber = 0;

	// Integer numbers may be negative
	int negative = 0;

	// First index of integer number
	int firstIndex = 0;
	// Last index of integer number
	int lastIndex = 0;

	// Iterate through while string
	for (int i = 0; i < size; i++) {

		// Find open and close quotes for a name or a string
		if (str[i] == '\"') {
			quotesNumber++;
			continue;
		}

		// If quotesNumber is odd then it is a name of a value or a string, which we avoid
		if (quotesNumber % 2 == 0) {
			// Parse integer

			// If first index is not set and we found number, then i is a first index of integer
			if (!firstIndex && (str[i] >= '0' && str[i] <= '9')) {
				firstIndex = i;
			}
			else if (str[i] == '-') {
				negative++;
			}
			// Last char in str is always a new line
			else if (str[i] == '\n') {
				// But last object or last element of an array doesn't have a comma so we have to keep it in mind
				if (str[i - 1] == ',') {
					lastIndex = i - 2;
				}
				else {
					lastIndex = i - 1;
				}
			}
		}

	}

	// Prepare answer
	int answer = INT_MAX;
	if (firstIndex) {

		answer = 0;
		// Create an integer out of string chars
		for (int i = lastIndex; i >= firstIndex; i--) {

			// Value of current number
			int num = str[i] - '0';

			// Take care of order
			answer += num * myPow(10, lastIndex - i);
		}

		if (negative) {
			answer = -answer;
		}
	}
	// If there is no number answer == INT_MAX, else answer stores value
	return answer;
}

// Raises the base to the power of exp (answer = base^exp)
int myPow(int base, int exp) {
	int answer = 1;
	for (int i = 0; i < exp; ++i) {
		answer *= base;
	}
	return answer;
}

// Parse line
void parseLine(const char* str, int size, int* lvl) {
	// First we try to find next integer
	int nextInt = findInt(str, size);
	// Then find out current level
	countLvl(str, size, lvl);

	// if there is no number, nextInt is set to INT_MAX
	if (nextInt != INT_MAX) {
		printf("%d:\t%d\n", nextInt, *lvl);
	}
}

// Find the actual size of a string
int findSize(const char* str) {
	int size = 0;
	while (str[size] != '\0') {
		size++;
	}
	return size;
}


// Function similar to fgets, build a substring s, maxsize is n, **str - current char in string to be read
char* sgets(char* s, int n, const char** str) {
	// Check if we already read while string
	if (**str == '\0') return NULL;

	int i;
	// Iterate through whole string (*str) this way we move pointer to currently read char
	for (i = 0; i < n - 1; ++i, ++(*str)) {
		s[i] = **str;
		if (**str == '\0')
			break;
		if (**str == '\n') {
			s[i + 1] = '\0';
			++(*str);
			break;
		}
	}

	// Just in case if line is too long
	if (i == n - 1)
		s[i] = '\0';

	// Return adress of first symbol in substring
	return s;
}