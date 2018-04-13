#pragma once

#include "Image.h"

#include <vector>

template <typename T>
Image<T> to_greyscale(const Image<T>& img)
{
	if(img.channels() != 3)
		throw std::logic_error("Imvalid type of image for greyscale");
	Image<T> greyImage(img.rows(), img.cols(), 1);
	for(size_t i = 0; i < greyImage.size(); ++i)
	{
		greyImage[i] = img[i*img.channels() + 0] * 0.114 + // b
					   img[i*img.channels() + 1] * 0.587 + // g
					   img[i*img.channels() + 2] * 0.299;  // r
	}
	return greyImage;
}

template <typename R, typename T, typename M>
Image<R> apply_mask(const Image<T>& img, const std::vector<M>& mask, size_t m_size)
{
	Image<R> result(img.rows(), img.cols(), img.channels());

	int lb = -m_size/2;
	int ub = m_size/2;

	for(int row = 0; row < (int)img.rows(); ++row)
	{
		for(int col = 0; col < (int)img.cols(); ++col)
		{
			int val = 0;
			for(int i = lb; i <= ub; ++i)
				for(int j = lb; j <= ub; ++j)
					val += mask[(i+ub)*m_size + (j+ub)] *
							img.val(row + i, col + j);
			result.at(row, col) = val;
		}
	}

	return result;
}

template <typename T>
cv::Mat to_mat(Image<T>& img)
{
	int type;
	if(img.channels() == 1)
		type = CV_8UC1;
	else
		type = CV_8UC3;
	return cv::Mat(img.rows(), img.cols(), type, img.data());
}