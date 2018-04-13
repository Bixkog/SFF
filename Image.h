#pragma once

#include <cassert>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

template <typename T, 
		  typename = std::enable_if_t<std::is_integral_v<T>>
		 >
class Image
{
public:
	Image(size_t n_rows,
		  size_t n_cols,
		  size_t n_channels)
	: n_rows(n_rows),
	n_cols(n_cols),
	n_channels(n_channels),
	pixels(n_rows * n_cols * n_channels)
	{
	}

	Image(const cv::Mat& m)
	: n_rows(m.rows),
	n_cols(m.cols),
	n_channels(m.channels()),
	pixels(n_rows * n_cols * n_channels)
	{
		if(m.isContinuous())
			pixels.assign(m.datastart, m.dataend);
		else
		{
			pixels.clear();
			for(int i = 0; i < m.rows; ++i)
				pixels.insert(pixels.end(), 
					m.ptr<T>(i), m.ptr<T>(i) + m.cols);
		}
	}

	// for 2D abstraction over pixels for mask calculation convinience
	T val(int row, int col, size_t channel = 0) const
	{
		assert(channel < n_channels);
		if(row >= (int)n_rows || row < 0)
			return 0;
		if(col >= (int)n_cols || col < 0)
			return 0;
		return pixels[(row * n_cols + col) * n_channels + channel];
	}

	T& at(size_t row, size_t col, size_t channel = 0)
	{
		assert(row < n_rows);
		assert(col < n_cols);
		assert(channel < n_channels);
		return pixels[(row * n_cols + col) * n_channels + channel];
	}

	T operator [](size_t i) const
	{
		return pixels[i];
	}

	T& operator [](size_t i)
	{
		return pixels[i];
	}

	size_t size() const
	{
		return n_rows * n_cols * n_channels;
	}

	size_t rows() const
	{
		return n_rows;
	}

	size_t cols() const
	{
		return n_cols;
	}

	size_t channels() const
	{
		return n_channels;
	}

	T* data()
	{
		return pixels.data();
	}

private:
	const size_t n_rows;
	const size_t n_cols;
	const size_t n_channels;
	std::vector<T> pixels;
};
