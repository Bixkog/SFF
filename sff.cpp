#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "aux.h"
#include "image.h"

std::vector<cv::Mat> get_input(std::string dir_name)
{
	std::vector<cv::Mat> inputs;
	std::string file_format = "/b_bigbug00%.2d_croppped.png";
	std::string format = dir_name + file_format;
	char file_name[100];
	for(int i = 0; i <= 12; ++i)
	{
		sprintf(file_name, format.c_str(), i);
		auto mat = cv::imread(file_name, cv::IMREAD_COLOR);
		if(mat.empty())
		{
			throw std::logic_error("Could not find image");
		}
		inputs.push_back(mat);
	}

	return inputs;
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << "Provide path to bug images" << std::endl;
		return 0;
	}
	auto inputs = get_input(argv[1]);

	auto imgs = map(inputs, toImage<uint8_t>);
	auto g_imgs = map(imgs, toGreyscale<uint8_t>);
	auto f_imgs = map(g_imgs, focusMeasure<uint8_t, int>);

	auto final_img = gather_focus(imgs, f_imgs);
	auto depth_img = get_depth(f_imgs);

	auto final_Mat = toMat(final_img);
	auto depth_Mat = toMat(depth_img);

    if(!depth_Mat.empty())
    	cv::imwrite("depth.png", depth_Mat);
   	else
   		throw std::logic_error("depth matrix empty");

   	if(!final_Mat.empty())
    	cv::imwrite("final.png", final_Mat); // Show our image inside it.
   	else
   		throw std::logic_error("final matrix empty");
    
    return 0;
}