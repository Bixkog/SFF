#include "aux.h"

#include <cassert>
#include <cmath>
#include <vector>

/*
	F (focus) and D (depth) represent 3 layers,
	the most focused layer and its neighobur layers.
	Three points (D[0], F[0]), (D[1], F[1]), (D[2], F[2])
	are interpolated with gaussian curve, 
	and its extremum point is taken as depth of pixel.
*/

double gaussian_peak(const std::vector<double>& F, const std::vector<double>& D)
{
	assert(F.size() == 3);
	assert(D.size() == 3);
	return ((std::log(F[1] + 1) - std::log(F[2] + 1)) * (D[1] * D[1] - D[0] * D[0]) - 
		(std::log(F[1] + 1) - std::log(F[0] + 1)) * (D[1] * D[1] - D[2] * D[2])) /
		((2 * (D[1] - D[0])) * ((std::log(F[1] + 1) - std::log(F[0] + 1)) + 
			(std::log(F[1] + 1) - std::log(F[2] + 1))));
}

double gaussian_var(const std::vector<double>& F, const std::vector<double>& D)
{
	assert(F.size() == 3);
	assert(D.size() == 3);
	return -((D[1] * D[1] - D[0] * D[0]) + (D[1] * D[1] - D[2] * D[2])) / 
			(2 * (((std::log(F[1] + 1) - std::log(F[2] + 1)) + 
				(std::log(F[1] + 1) - std::log(F[0] + 1)))));
}

double gaussian_height(double peak, double var, double max_f, double max_d)
{
	return max_f / std::exp(-0.5 * std::pow(((max_d - peak)/var), 2));
}
