#include "focus.h"

#include "Image.h"

#include <vector>

static
size_t max_focus(const std::vector<Image<int>>& f_imgs, size_t pixel)
{
	auto comp = [pixel](const auto& f_img1, const auto& f_img2)
				{
					return f_img1[pixel] < f_img2[pixel];
				};
	auto max_e = std::max_element(f_imgs.begin(), f_imgs.end(), comp);

	return std::distance(f_imgs.begin(), max_e);
}

Image<uint8_t> gather_focus(const std::vector<Image<uint8_t>>& imgs, 
				  const std::vector<Image<int>>& f_imgs)
{
	if(imgs.empty())
		throw std::logic_error("No images to gather focus");
	if(f_imgs.empty())
		throw std::logic_error("No focus images to gather focus");

	size_t nr = imgs[0].rows();
	size_t nc = imgs[0].cols();
	size_t size = nr * nc * 3;
	Image<uint8_t> final_img(nr, nc, 3);
	for(size_t i = 0; i < size; i+= 3)
	{
		auto max_f = max_focus(f_imgs, i/3);
		for(size_t j = 0; j < 3; ++j)
			final_img[i + j] = imgs[max_f][i + j];
	}
	return final_img;
}