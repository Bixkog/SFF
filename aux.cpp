#include "aux.h"
#include <cmath>

double gaussian_peak(const std::vector<double>& F, const std::vector<double>& D)
{
	return ((std::log(F[1] + 1) - std::log(F[2] + 1)) * (D[1] * D[1] - D[0] * D[0]) - 
		(std::log(F[1] + 1) - std::log(F[0] + 1)) * (D[1] * D[1] - D[2] * D[2])) /
		((2 * (D[1] - D[0])) * ((std::log(F[1] + 1) - std::log(F[0] + 1)) + 
			(std::log(F[1] + 1) - std::log(F[2] + 1))));
}

double gaussian_var(const std::vector<double>& F, const std::vector<double>& D)
{
	return -((D[1] * D[1] - D[0] * D[0]) + (D[1] * D[1] - D[2] * D[2])) / 
			(2 * (((std::log(F[1] + 1) - std::log(F[2] + 1)) + 
				(std::log(F[1] + 1) - std::log(F[0] + 1)))));
}

double gaussian_height(double peak, double var, double max_f, double max_d)
{
	return max_f / std::exp(-0.5 * std::pow(((max_d - peak)/var), 2));
}
