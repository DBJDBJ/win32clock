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

#include <assert.h>

#include <functional>
#include <utility>

namespace dbj {

	/*
	Task: implement this for any type
	*/
	inline int default_int_width(int new_ = 1) {
		static int dflt_ = new_;
		if (new_ != dflt_) dflt_ = new_;
		return new_;
	}

#pragma region "Version THREE"
	/*
	No stunts. A plain old function object.
	Simple usable and working
	*/
	template<typename T>
	class holder {
		// mutable members of const class instances are modifiable
		mutable T default_value_{};
		// in version 0.1 hide ctors, movers and copiers
		holder();
		holder(const holder &);
		holder(const holder &&);
		holder &  operator = (const holder &);
		holder && operator = (const holder &&);
	public:
		holder(const T & defval_) : default_value_(defval_) {
		};

		const T & operator () () const noexcept {
			return default_value_;
		}
		/*
		please read
		https://msdn.microsoft.com/en-us/library/031k84se.aspx
		to understand the operator declaration bellow
		it allows for const isntances to be used
		through the operator bellow
			const holder<REAL> width{ 10 };
			width(1024) ; // OK
		*/
		const volatile T & operator ()
			(const T & new_) const volatile noexcept
		{
			if (new_ != default_value_)
				default_value_ = new_;
			return default_value_;
		}
	};
	namespace {
		using namespace Gdiplus;

		// test the const onstance behaviour
		struct S {
			holder<REAL> width{ 10 };
		};

		const S konst_;

		holder<SmoothingMode> smoothnes{ SmoothingMode::SmoothingModeAntiAlias };
		holder<LineCap> linecap{ LineCap::LineCapRound };

		inline auto usage() {
			auto width_ = konst_.width(1024);
			const auto w1_ = konst_.width();
			auto w2_ = konst_.width();

			assert(width_ == 1024);
			assert(w2_ == w1_ == width_);

			auto lc_ = linecap();
			return smoothnes();
		}

	}
#pragma endregion "Version THREE"

#pragma region "Version TWO"
	/*
	Function template returning a lambda
	Closer to real stunt, but works
	And compiler has less problems disambiguating the instances
	*/
	template<typename T>
	inline auto holder_maker(T defval_) {
		// no T new_ = defval_ will not compile, try...
		return [&](const std::optional<T> & new_ = std::optional<T>())
		{
			static T default_value = defval_;
			if (new_ && *new_ != default_value)
				default_value = *new_;
			return default_value;
		};
	};

	namespace {
		using namespace Gdiplus;

		auto default_smoot = holder_maker<SmoothingMode>(SmoothingMode::SmoothingModeAntiAlias);
		auto default_lncap = holder_maker<LineCap>(LineCap::LineCapRound);
		auto default_width = holder_maker<Gdiplus::REAL>(10);

	}
#pragma endregion "Version TWO" 
#if 0
#pragma region "Version ONE"

	template< typename T, T defval_ = T() >
	inline T & static_default(const T & new_ = defval_)
	{
		static T default_value = defval_;
		if (new_ != default_value)
			default_value = new_;
		return default_value;
	}

	namespace {
		using namespace Gdiplus;

		const auto sm0 = static_default<SmoothingMode, SmoothingMode::SmoothingModeAntiAlias>();
		const auto sm1 = static_default<SmoothingMode>();

		auto sm2 = static_default<SmoothingMode, SmoothingMode::SmoothingModeAntiAlias>;
		auto lc0 = static_default<LineCap, LineCap::LineCapRound>;
		auto sm3 = static_default<SmoothingMode>;
		auto dlcF = static_default<LineCap>;

		auto dlc = dlcF(LineCap::LineCapRound);

		/*
		but there is a problem here ...
		auto default_problem = static_default < REAL, REAL{10} > ;

		error C3535: cannot deduce type for 'auto' from 'overloaded-function'
		error C2440: 'initializing': cannot convert from 'T &(__cdecl *)(const T *)' to 'int'
		note: Context does not allow for disambiguation of overloaded function

		above template instance is not "different enough" from two previous ones
		*/
	}

#pragma endregion "Version ONE"
#endif
} // namespace dbj