#include "image.h"

static
size_t max_focus(const std::vector<Image<int>>& f_imgs, size_t pixel)
{
	auto comp = [pixel](const auto& f_img1, const auto& f_img2)
				{
					return f_img1.pixels[pixel] < f_img2.pixels[pixel];
				};
	auto max_e = std::max_element(f_imgs.begin(), f_imgs.end(), comp);

	return std::distance(f_imgs.begin(), max_e); // argmax
}

Image<uint8_t> gather_focus(const std::vector<Image<uint8_t>>& imgs, 
				  const std::vector<Image<int>>& f_imgs)
{
	size_t nr = imgs[0].nr;
	size_t nc = imgs[0].nc;
	size_t size = nr * nc * 3;
	Image<uint8_t> final_img(nr, nc, 3);
	final_img.pixels.reserve(size);
	for(size_t i = 0; i < size; i+= 3)
	{
		auto max_f = max_focus(f_imgs, i/3);
		final_img.pixels.insert(final_img.pixels.end(), 
						 imgs[max_f].pixels.begin() + i,
						 imgs[max_f].pixels.begin() + i + 3);
	}
	return final_img;
}

double T3 = 350000;
double T4 = 280;

static
uint8_t pixel_depth(std::vector<double> pixel_f)
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
	if(var > T4)
		return 0;
	if(h < T3)
		return 0;
	return 255 - p;
}

Image<uint8_t> get_depth(const std::vector<Image<int>>& f_imgs)
{
	size_t nr = f_imgs[0].nr;
	size_t nc = f_imgs[0].nc;
	size_t size = nr * nc;
	Image<uint8_t> depth_img(nr, nc, 1);
	depth_img.pixels.reserve(size);

	for(size_t i = 0; i < size; ++i)
	{
		std::vector<double> pixel_f; pixel_f.reserve(f_imgs.size());
		for(const auto& f_img : f_imgs)
			pixel_f.push_back(f_img.pixels[i]);
		depth_img.pixels.push_back(pixel_depth(pixel_f));
	}
	return depth_img;
}