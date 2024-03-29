#pragma once

#include "structs/window_data.hpp"

namespace raptor_engine {
namespace structs {

enum class low_level_render_api_type : u8
{
	// Realized
	OPENGL = 0,

	// Unrealized
	DIRECTX,
	METALL,
	VULKAN,

	LOW_LEVEL_RENDER_API_TYPE_COUNT
};

struct high_level_renderer_data
{
public:
	high_level_renderer_data();

	high_level_renderer_data(const sdl_window_sptr&			  sdl_window_ptr_,
							 const low_level_render_api_type& render_api_type_ = low_level_render_api_type::OPENGL);

	~high_level_renderer_data();

public:
	const sdl_window_sptr get_window() const;

	low_level_render_api_type get_render_api_type() const;

 private:
    sdl_window_sptr window_ptr;
    low_level_render_api_type render_api_type;
};

using high_level_renderer_data_uptr = std::unique_ptr<high_level_renderer_data>;
using high_level_renderer_data_sptr = std::shared_ptr<high_level_renderer_data>;
using high_level_renderer_data_wptr = std::weak_ptr<high_level_renderer_data>;

}  // namespace structs
}  // namespace raptor_engine