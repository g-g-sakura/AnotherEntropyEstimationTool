////////////////////////////////////////////////////////////////////////////////
// SpecialFunctions.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "SpecialFunctions.h"
#include <boost/math/special_functions/erf.hpp>

namespace entropy_estimator_lib
{
	double calc_Z_alpha(double alpha)
	{
		double x = 2.0 * (1.0 - alpha);
		double val = boost::math::erfc_inv<double>(x);
		return M_SQRT2 * val;
	}

}