#pragma once

#include <opencv2/highgui/highgui.hpp>

#include <vector>

#include "aux.h"

template <typename T>
struct Image
{
	std::vector<T> pixels;
	const size_t nr, nc;
	const size_t channels;

	Image(size_t nr = 0,
		  size_t nc = 0,
		  size_t channels = 1)
	: nr(nr),
	nc(nc),
	channels(channels)
	{}

	// for 2D abstraction over pixels
	T at(int row, int col, size_t channel = 0) const
	{
		if(row >= (int)nr || row < 0)
			return 0;
		if(col >= (int)nc || col < 0)
			return 0;
		assert(channel < channels);
		return pixels[(row * nc + col) * channels + channel];
	}
};

template <typename T>
Image<T> toImage(const cv::Mat& m)
{
	Image<T> img(m.rows, m.cols, m.channels());
	if(m.isContinuous())
		img.pixels.assign(m.datastart, m.dataend);
	else
	{
		for(int i = 0; i < m.rows; ++i)
			img.pixels.insert(img.pixels.end(), 
				m.ptr<T>(i), m.ptr<T>(i) + m.cols);
	}
	return img;
}

template <typename T>
Image<T> toGreyscale(const Image<T>& img)
{
	if(img.channels != 3)
		return img; // assume its already greyscale
	Image<T> greyImage(img.nr, img.nc, 1);
	greyImage.pixels.resize(img.nr * img.nc);
	for(size_t i = 0; i < greyImage.pixels.size(); ++i)
	{
		greyImage.pixels[i] = img.pixels[i*img.channels + 0] * 0.114 + // b
							  img.pixels[i*img.channels + 1] * 0.587 + // g
							  img.pixels[i*img.channels + 2] * 0.299;  // r
	}
	return greyImage;
}

template <typename R, typename T, typename M>
Image<R> apply_mask(const Image<T>& img, std::vector<M> mask, size_t m_size)
{
	Image<R> result(img.nr, img.nc, img.channels);
	result.pixels.reserve(img.nr * img.nc);

	int lb = -m_size/2;
	int ub = m_size/2;

	for(int row = 0; row < (int)img.nr; ++row)
	{
		for(int col = 0; col < (int)img.nc; ++col)
		{
			int val = 0;
			for(int i = lb; i <= ub; ++i)
				for(int j = lb; j <= ub; ++j)
					val += mask[(i+ub)*m_size + (j+ub)] *
							img.at(row + i, col + j);
			result.pixels.push_back(val);
		}
	}

	return result;
}

//LAP4
template <typename T, typename F>
Image<F> get_focus(const Image<T>& lap, size_t n_size)
{
	Image<F> result(lap.nr, lap.nc, lap.channels);
	result.pixels.reserve(lap.nr * lap.nc);

	int lb = -n_size/2;
	int ub = n_size/2;

	// optimize by division to small squares
	std::vector<T> lap_v;
	lap_v.reserve(n_size*n_size);
	for(int row = 0; row < (int)lap.nr; ++row)
	{
		for(int col = 0; col < (int)lap.nc; ++col)
		{
			lap_v.clear();
			for(int i = lb; i <= ub; ++i)
				for(int j = lb; j <= ub; ++j)
					lap_v.push_back(lap.at(row + i, col + j));

			double sum = 0;
			for(auto e : lap_v)
				sum += e;
			double mean = sum / lap_v.size();
			double variance = 0;
			for(auto e : lap_v)
				variance += (e - mean) * (e - mean);
			result.pixels.push_back(variance);
		}
	}

	return result;
}

template <typename T, typename F>
Image<F> focusMeasure(const Image<T>& img)
{
	std::vector<double> laplacian_mask = 
		{0,  0, -1,  0,  0,
		 0, -1, -2, -1,  0,
		-1, -2, 17, -2, -1,
		 0, -1, -2, -1,  0,
		 0,  0, -1,  0,  0};

	auto laplacian = apply_mask<int>(img, laplacian_mask, 5);
	auto focus = get_focus<int, F>(laplacian, 3);
	return focus;
}

template <typename T>
cv::Mat toMat(Image<T>& img)
{
	int type;
	if(img.channels == 1)
		type = CV_8UC1;
	else
		type = CV_8UC3;
	return cv::Mat(img.nr, img.nc, type, img.pixels.data());
}

Image<uint8_t> gather_focus(const std::vector<Image<uint8_t>>& imgs, 
				  const std::vector<Image<int>>& f_imgs);

Image<uint8_t> get_depth(const std::vector<Image<int>>& f_imgs);