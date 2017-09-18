#pragma once
/*
To (not)use dbj::dbg (un)define _DBJ_DBG_

(c) 2017 by dbj.org

inspirators are mentioned in the comments bellow
*/
#define _DBJ_DBG_
#ifdef _DBJ_DBG_

#include "stdafx.h"


namespace dbj {


	namespace dbg {

		template <typename T>
		DBJ_INLINE
		constexpr auto sizeof_array(const T& iarray) {
			return (sizeof(iarray) / sizeof(iarray[0]));
		}

#ifdef BUFSIZ
		constexpr static int TRACEMAXSTRING = BUFSIZ * 2;
#else
		constexpr static int TRACEMAXSTRING = 1024 * 2;
#endif

		// https://stackoverflow.com/questions/451749/is-there-a-trace-statement-for-basic-win32-c
		inline void trace( const char * filename, const unsigned int line, const char* const format, ...)
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
