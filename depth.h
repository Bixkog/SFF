#pragma once

#include "Image.h"

#include <vector>

Image<uint8_t> get_depth(const std::vector<Image<int>>& f_imgs);