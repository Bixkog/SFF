#include "aux.h"
#include "depth.h"
#include "focus.h"
#include "Image.h"
#include "image_utils.h"

#include <algorithm>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

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
	if(argc < 2)
	{
		std::cout << "Provide path to bug images directory" << std::endl;
		return 0;
	}

	std::cout << "Reading files." << std::endl;
	auto inputs = get_input(argv[1]);
	auto imgs = map(inputs, [](const auto& m){return Image<uint8_t>(m);});
	
	std::cout << "Converting to greyscale." << std::endl;
	auto g_imgs = map(imgs, to_greyscale<uint8_t>);
	
	std::cout << "Calculating focus." << std::endl;
	auto f_imgs = map(g_imgs, focus_measure<uint8_t>);

	std::cout << "Creating combined focus image." << std::endl;
	auto final_img = gather_focus(imgs, f_imgs);

	std::cout << "Creating depth image." << std::endl;
	auto depth_img = get_depth(f_imgs);

	std::string depth_filename = "depth.png";
	std::string final_filename = "final.png";
	if(argc > 2)
		final_filename = argv[2];
	if(argc > 3)
		depth_filename = argv[3];

	std::cout << "Writing results to files: " 
			  << final_filename << " "
			  << depth_filename << std::endl;
	auto final_Mat = to_mat(final_img);
	auto depth_Mat = to_mat(depth_img);

    if(depth_Mat.empty())
   		throw std::logic_error("depth matrix empty");

   	if(final_Mat.empty())
   		throw std::logic_error("final matrix empty");
	
	cv::imwrite(depth_filename.c_str(), depth_Mat);
	cv::imwrite(final_filename.c_str(), final_Mat);

    return 0;
}