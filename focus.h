#pragma once

#include "Image.h"
#include "image_utils.h"

#include <vector>

template <typename T>
Image<int> get_focus(const Image<T>& lap, size_t n_size)
{
	Image<int> result(lap.rows(), lap.cols(), lap.channels());

	int lb = -n_size/2;
	int ub = n_size/2;

	// optimize by division to small squares
	std::vector<T> lap_v;
	lap_v.reserve(n_size*n_size);
	for(int row = 0; row < (int)lap.rows(); ++row)
	{
		for(int col = 0; col < (int)lap.cols(); ++col)
		{
			lap_v.clear();
			for(int i = lb; i <= ub; ++i)
				for(int j = lb; j <= ub; ++j)
					lap_v.push_back(lap.val(row + i, col + j));

			double sum = 0;
			for(auto e : lap_v)
				sum += e;
			double mean = sum / lap_v.size();
			double variance = 0;
			for(auto e : lap_v)
				variance += (e - mean) * (e - mean);
			result.at(row, col) = variance;
		}
	}

	return result;
}

template <typename T>
Image<int> focus_measure(const Image<T>& img)
{
	std::vector<double> laplacian_mask = 
		{0,  0, -1,  0,  0,
		 0, -1, -2, -1,  0,
		-1, -2, 17, -2, -1,
		 0, -1, -2, -1,  0,
		 0,  0, -1,  0,  0};

	auto laplacian = apply_mask<int>(img, laplacian_mask, 5);
	auto focus = get_focus<int>(laplacian, 3);
	return focus;
}

Image<uint8_t> gather_focus(const std::vector<Image<uint8_t>>& imgs, 
				  const std::vector<Image<int>>& f_imgs);
