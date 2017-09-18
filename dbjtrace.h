#pragma once
/*
To (not)use dbj::dbg (un)define _DBJ_DBG_

(c) 2017 by dbj.org

inspirators are mentioned in the comments bellow
*/
#define _DBJ_DBG_
#ifdef _DBJ_DBG_

#include "stdafx.h"

/*
Tame full path filenames  in __FILE__
https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/8488201#8488201
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
*/
__forceinline constexpr auto nicer_filename( const char * filename ) {
	return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
}

namespace dbj {

	// https://opensource.apple.com/source/bash/bash-80/bash/lib/sh/strnlen.c
	/* Find the length of S, but scan at most MAXLEN characters.  If no '\0'
	terminator is found within the first MAXLEN characters, return MAXLEN. */
	__forceinline size_t
		strnlen( register const char *s,	size_t maxlen
		)
	{
		register const char *e;
		size_t n;

		for (e = s, n = 0; *e && n < maxlen; e++, n++)
			;
		return n;
	}

	namespace dbg {

		template <typename T>
		__forceinline
		constexpr auto sizeof_array(const T& iarray) {
			return (sizeof(iarray) / sizeof(iarray[0]));
		}

#ifdef BUFSIZ
		constexpr static int TRACEMAXSTRING = BUFSIZ * 2;
#else
		constexpr static int TRACEMAXSTRING = 1024 * 2;
#endif

		// https://stackoverflow.com/questions/451749/is-there-a-trace-statement-for-basic-win32-c
		__forceinline void trace( const char * filename, const unsigned int line, const char* const format, ...)
		{
			std::vector<char> buffer(TRACEMAXSTRING, '\x0') ;
			auto nicer_name = nicer_filename(filename);
			DBJ_ASSERT(nicer_name);
			_snprintf_s(&buffer[0], buffer.size(), TRACEMAXSTRING-1, "%s(%d): ", nicer_name, line);
				
			_RPT0(_CRT_WARN, buffer.data());

			buffer.resize(TRACEMAXSTRING);
			va_list args;
			va_start(args, format);
			int nBuf;
			nBuf = _vsnprintf_s(
				&buffer[0],
				buffer.size(),
				TRACEMAXSTRING - 1,
				format,
				args);
			va_end(args);

			_RPT0(_CRT_WARN, buffer.data());
		}
	}
}
#define DBJ_TRACE( format, ...)  dbj::dbg::trace( __FILE__, __LINE__, format, __VA_ARGS__ )
#else
#define DBJ_TRACE((void)0)
#endif // _DBJ_DBG_
