#include "depth.h"

#include "aux.h"
#include "Image.h"

#include <vector>
#include <iostream>

double min_height = 350000;
double max_var = 280;

static
uint8_t pixel_depth(const std::vector<double>& pixel_f)
{
	auto max_f = std::max_element(pixel_f.begin(), pixel_f.end());
	if(max_f == pixel_f.begin())
		max_f++;
	else if(max_f == pixel_f.end())
		max_f--;
	auto left_f = max_f - 1;
	auto right_f = max_f + 1;

	// linear depth distribution
	double max_d = std::distance(pixel_f.begin(), max_f) 
					/ (double) pixel_f.size() * 255;
	double left_d = std::distance(pixel_f.begin(), left_f) 
					/ (double) pixel_f.size() * 255;
	double right_d = std::distance(pixel_f.begin(), right_f) 
					/ (double) pixel_f.size() * 255;

	std::vector<double> F = {*left_f, *max_f, *right_f};
	std::vector<double> D = {left_d, max_d, right_d};

	auto p = gaussian_peak(F, D);
	auto var = gaussian_var(F, D);
	auto h = gaussian_height(p, var, *max_f, max_d);
	if(var > max_var)
		return 0;
	if(h < min_height)
		return 0;
	if(p < 0)
		return 255;
	if(p > 255)
		return 0;
	return 255 - p;
}

Image<uint8_t> get_depth(const std::vector<Image<int>>& f_imgs)
{
	if(f_imgs.empty())
		throw std::logic_error("No focus images for depth calculation");
	size_t nr = f_imgs[0].rows();
	size_t nc = f_imgs[0].cols();
	size_t size = nr * nc;
	Image<uint8_t> depth_img(nr, nc, 1);

	for(size_t i = 0; i < size; ++i)
	{
		std::vector<double> pixel_f; pixel_f.reserve(f_imgs.size());
		for(const auto& f_img : f_imgs)
			pixel_f.push_back(f_img[i]);
		depth_img[i] = pixel_depth(pixel_f);
	}
	return depth_img;
}