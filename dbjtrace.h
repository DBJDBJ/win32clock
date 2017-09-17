#define _DBJ_DBG_
#ifdef _DBJ_DBG_
#pragma once

#include <crtdbg.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vector>


// https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/8488201#8488201
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// https://stackoverflow.com/questions/451749/is-there-a-trace-statement-for-basic-win32-c

namespace dbj {

	namespace dbg {

		template <typename T>
		constexpr auto sizeof_array(const T& iarray) {
			return (sizeof(iarray) / sizeof(iarray[0]));
		}

		constexpr static int TRACEMAXSTRING = 1024 * 2;

		__forceinline void trace( const char * filename, int line, const char* format, ...)
		{
			std::vector<char> buffer(TRACEMAXSTRING) ;

			buffer.resize(TRACEMAXSTRING);
//			size_t for_debug = buffer.size();
//			auto for_debug_modern = sizeof_array(&buffer[0]);

			_snprintf_s(&buffer[0], buffer.size(), buffer.size(), "%s(%d): ",
				&strrchr(filename, '\\')[1], line); 
				
			_RPT0(_CRT_WARN, buffer.data());

			buffer.resize(0); buffer.resize(TRACEMAXSTRING);
			va_list args;
			va_start(args, format);
			int nBuf;
			nBuf = _vsnprintf_s(
				&buffer[0],
				buffer.size() ,
				buffer.size(),
				format,
				args);
			va_end(args);

			_RPT0(_CRT_WARN, buffer.data());
		}
	}
}
#define TRACEF( format, ...)  dbj::dbg::trace( __FILENAME__, __LINE__, format, __VA_ARGS__ )

#define __FILENAME__ __FILE__

#endif // _DBJ_DBG_
