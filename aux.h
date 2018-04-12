#pragma once

#include <vector>

template<typename T, typename F>
auto map(const std::vector<T>& c, F f)
{
	using T2 = decltype(f(std::declval<T>()));
	std::vector<T2> r; 
	r.reserve(c.size());
	for(const auto& e : c)
		r.push_back(f(e));
	return r;
}

double gaussian_peak(const std::vector<double>& F, const std::vector<double>& D);

double gaussian_var(const std::vector<double>& F, const std::vector<double>& D);

double gaussian_height(double peak, double var, double max_f, double max_d);
