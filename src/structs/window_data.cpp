#include "window_data.hpp"

#include <utility>  // for swap

using namespace raptor_engine::structs;

window_data::window_data(const std::string& title, int x_pos, int y_pos,
                         int width, int height, u32 flags)
    : title{title},
      x_pos{x_pos},
      y_pos{y_pos},
      width{width},
      height{height},
      flags{flags} {}

void swap(window_data& first_data, window_data& second_data) {
  if (&first_data == &second_data) {
    return;
  }
  std::swap(first_data.title, second_data.title);
  std::swap(first_data.x_pos, second_data.x_pos);
  std::swap(first_data.y_pos, second_data.y_pos);
  std::swap(first_data.width, second_data.width);
  std::swap(first_data.height, second_data.height);
  std::swap(first_data.flags, second_data.flags);
}