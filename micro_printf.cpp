/*
Open source by dbj.org
printf inspired by 
https://stackoverflow.com/questions/16647278/minimal-implementation-of-sprintf-or-printf
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
*/
#include "stdafx.h"
#ifdef DBJ_MICRO_PRINTF
namespace dbj {

	namespace {

#ifdef BUFSIZ
	constexpr auto BUFFER_SIZE = BUFSIZ * 2;
#else
	constexpr auto BUFFER_SIZE = 512 * 2;
#endif

	template<typename CHAR_TYPE, size_t size = BUFSIZ>
	inline constexpr auto create_vector_buffer(CHAR_TYPE(&arr)[size]) {
		const auto first = arr[0];
		return std::vector<CHAR_TYPE>(size);
	}

	inline int normalize(double *val) {
		int exponent = 0;
		double value = *val;

		while (value >= 1.0) {
			value /= 10.0;
			++exponent;
		}

		while (value < 0.1) {
			value *= 10.0;
			--exponent;
		}
		*val = value;
		return exponent;
	}

	inline void ftoa_fixed(char(&the_buffer)[BUFFER_SIZE], double value) {
		/* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
		* Values with absolute values less than 0.000001 are rounded to 0.0
		* Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
		* The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
		* e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
		* characters, so that's what we really expect as the buffer size.
		*/
		char * buffer = the_buffer;
		int exponent = 0;
		int places = 0;
		static const int width = 4;

		if (value == 0.0) {
			buffer[0] = '0';
			buffer[1] = '\0';
			return;
		}

		if (value < 0.0) {
			*buffer++ = '-';
			value = -value;
		}

		exponent = normalize(&value);

		int digit = 0;
		while (exponent > 0) {
			digit = static_cast<int>(value * 10);
			*buffer++ = digit + '0';
			value = value * 10 - digit;
			++places;
			--exponent;
		}

		if (places == 0)
			*buffer++ = '0';

		*buffer++ = '.';

		while (exponent < 0 && places < width) {
			*buffer++ = '0';
			--exponent;
			++places;
		}
		// int digit = 0;
		while (places < width) {
			digit = static_cast<int>(value * 10.0);
			*buffer++ = digit + '0';
			value = value * 10.0 - digit;
			++places;
		}
		*buffer = '\0';
	}

	inline void ftoa_sci(char *buffer, double value) {
		int exponent = 0;
		int places = 0;
		static const int width = 4;

		if (value == 0.0) {
			buffer[0] = '0';
			buffer[1] = '\0';
			return;
		}

		if (value < 0.0) {
			*buffer++ = '-';
			value = -value;
		}

		int digit = 0;
		exponent = normalize(&value);

		digit = static_cast<int>(value * 10.0);
		*buffer++ = digit + '0';
		value = value * 10.0 - digit;
		--exponent;

		*buffer++ = '.';

		for (int i = 0; i < width; i++) {
			int digit = static_cast<int>(value * 10.0);
			*buffer++ = digit + '0';
			value = value * 10.0 - digit;
		}

		*buffer++ = 'e';
		_itoa_s(exponent, buffer, BUFFER_SIZE, 10);
	}
inline int fprintf(FILE *file, char const *fmt, va_list arg) {

		int int_temp;
		char char_temp;
		char *string_temp;
		double double_temp;

		char ch;
		int length = 0;

		char buffer[BUFFER_SIZE] = {} ;

		while (ch = *fmt++) {
			if ('%' == ch) {
				switch (ch = *fmt++) {
					/* %% - print out a single %    */
				case '%':
					fputc('%', file);
					length++;
					break;

					/* %c: print out a character    */
				case 'c':
					char_temp = va_arg(arg, int);
					fputc(char_temp, file);
					length++;
					break;

					/* %s: print out a string       */
				case 's':
					string_temp = va_arg(arg, char *);
					fputs(string_temp, file);
					length += strlen(string_temp);
					break;

					/* %d: print out an int         */
				case 'd':
					int_temp = va_arg(arg, int);
					_itoa_s(int_temp, buffer, BUFFER_SIZE, 10);
					fputs(buffer, file);
					length += strlen(buffer);
					break;

					/* %x: print out an int in hex  */
				case 'x':
					int_temp = va_arg(arg, int);
					_itoa_s(int_temp, buffer, BUFFER_SIZE, 16);
					fputs(buffer, file);
					length += strlen(buffer);
					break;

				case 'f':
					double_temp = va_arg(arg, double);
					ftoa_fixed(buffer, double_temp);
					fputs(buffer, file);
					length += strlen(buffer);
					break;

				case 'e':
					double_temp = va_arg(arg, double);
					ftoa_sci(buffer, double_temp);
					fputs(buffer, file);
					length += strlen(buffer);
					break;
				default:
					/* just ignore the unhandled format specifier
					   print '?' in its place
					*/
					fputc('?', file);
					length++;
					break;
				} // switch
			}
			else {
				putc(ch, file);
				length++;
			}
		}
		return length;
	}

} // namespace

inline int fprintf(FILE *file, char const *fmt, ...) {
		va_list arg;
		int length;

		va_start(arg, fmt);
		length = fprintf(file, fmt, arg);
		va_end(arg);
		return length;
	}

inline int printf(char const *fmt, ...) {
	va_list arg;
	int length;

	va_start(arg, fmt);
	length = dbj::fprintf(stdout, fmt, arg);
	va_end(arg);
	return length;
}


} // namespace dbj

#define TEST
#ifdef TEST 

extern int microprintftest() {

	double floats[] = { 0.0, 1.234e-10, 1.234e+10, -1.234e-10, -1.234e-10 };

	dbj::printf("%s, %d, %x \n", "Some string", 1, 0x1234);

	for (int i = 0; i < sizeof(floats) / sizeof(floats[0]); i++)
		dbj::printf("%f, %e\n", floats[i], floats[i]);

	return 0;
}

#endif

#endif // DBJ_MICRO_PRINTF