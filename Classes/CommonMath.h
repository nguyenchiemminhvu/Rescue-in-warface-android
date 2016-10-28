#ifndef __COMMON_MATH_H__
#define __COMMON_MATH_H__

#pragma once

namespace CommonMath {

	template<typename T>
	inline T min(T a, T b) {
		return (a > b) ? b : a;
	}

	template<typename T>
	inline T max(T a, T b) {
		return (a > b) ? a : b;
	}

	template<typename T> 
	inline T clamp(T value, T low, T high) {
		return min(high, max(value, low));
	}

}

#endif // !__COMMON_MATH_H__

