#pragma once
/*
Copyright 2017 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

namespace dbj {
	template<typename T>
	class holder {
		T default_value_{};
		holder();
		holder(const holder &);
		holder(const holder &&);
		holder &  operator = (const holder &);
		holder && operator = (const holder &&);
	public:
		holder(const T & defval_) : default_value_(defval_) {
			// default_value_ = defval_;
		};
		const T & operator () () const noexcept {
			return default_value_;
		}
		const T & operator () (const T & new_ = default_value_) noexcept {
			if (new_ != default_value_)
				default_value_ = new_;
			return default_value_;
		}
	};

	//
	template<typename T>
	inline auto holder_maker(T defval_) {
		return [&](const T * new_ = nullptr)
		{
			static T default_value = defval_;
			if (new_ && *new_ != default_value)
				default_value = *new_;
			return default_value;
		};
	};

	//
	template< typename T, T defval_ >
	inline T & static_default(const T * new_ = nullptr)
	{
		static T default_value = defval_;
		if (new_ != nullptr)
			default_value = static_cast<T>(*new_);
		return default_value;
	}

	namespace {
		using namespace Gdiplus;

		/*
		works but delivers just a default value for a repeated use.
		there is no function  to re-use
		*/
		const auto sm = static_default<SmoothingMode, SmoothingMode::SmoothingModeAntiAlias>();

		/*
		inline auto smoothingmode(	const Gdiplus::SmoothingMode * new_ = nullptr ) {
		return default_smoothing(new_);
		}

		inline auto linecap( const Gdiplus::LineCap * new_ = nullptr ) {
		return default_linecap(new_);
		}
		*/

		/* pointer to template function instances
		using auto gives much simpler and shorter code
		this works but we loose the default argument feature  on function pointers
		declared this way
		*/
		auto default_smoothing = static_default<SmoothingMode, SmoothingMode::SmoothingModeAntiAlias>;
		auto default_linecap = static_default<LineCap, LineCap::LineCapRound>;

		/*
		also there is aproblem here ...

		error C3535: cannot deduce type for 'auto' from 'overloaded-function'
		error C2440: 'initializing': cannot convert from 'T &(__cdecl *)(const T *)' to 'int'
		note: Context does not allow for disambiguation of overloaded function

		auto default_problem = static_default < REAL, REAL{10} > ;

		above template instance is not "different enough" from two previous ones
		*/

		auto default_width = holder_maker<Gdiplus::REAL>(10);
	}

}
