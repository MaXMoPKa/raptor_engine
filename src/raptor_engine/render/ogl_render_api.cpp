#include "render/ogl_render_api.hpp"

#include <string>
#include <array>

#include "render/shader.hpp"
#include "render/vertex_array_object.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/element_buffer_object.hpp"

#include "SDL2/SDL_timer.h"

using namespace raptor_engine::render;

static enum class SHADER_UNIFORMS : u32
{
	TIME,

	COUNT
};

static constexpr std::array<std::string_view, static_cast<u32>(SHADER_UNIFORMS::COUNT)> SHADER_UNIFORMS_NAME = { "g_time" };

class ogl_render_api::ogl_render_api_pimpl
{

public:

	ogl_render_api_pimpl() : gl_context {nullptr}, shader_program {}, vao {}, vbo {}, ebo {}, draw_config {} { }

	ogl_render_api_pimpl(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
		: vao {}, vbo {}, ebo {}, draw_config {scene_info->get_draw_config()}
	{
		gl_context = SDL_GL_CreateContext(window_ptr.get());
		if (this->gl_context == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create GL Context");
		}

		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", &result);
		}
	}

	~ogl_render_api_pimpl() = default;

public:

	void create(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
	{
		ogl_render_api_pimpl tmp(window_ptr, scene_info);
		this->swap(tmp);
	}

	void swap(ogl_render_api_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->gl_context, pimpl.gl_context);
	}

	void reset() noexcept
	{
		ogl_render_api_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void clear_color()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void use_shader_program(u32 shader_id)
	{
		if (draw_config->get_wireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} 
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glUseProgram(shader_id);
	}

	void bind_g_uniforms(const std::vector<u32>& g_uniforms_, u64 time_)
	{
		for (const auto& g_uniform_location : g_uniforms_)
		{
			glUniform1f(g_uniform_location, time_);
		}
	}

	void update_uniform()
	{
		u64 time				  = SDL_GetTicks64();
		float greenValue		  = static_cast<float>(sin(time) / 2.0 + 0.5);
		int	  vertexColorLocation = glGetUniformLocation(shader_program->get_id(), "our_color");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	}

	void bind_vao(u32 vao_id)
	{
		glBindVertexArray(vao_id);
	}

	void bind_texture(u32 texture_id_, u32 active_texture_id_)
	{
		glActiveTexture(GL_TEXTURE0 + active_texture_id_);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
	}

	void draw_arrays()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void draw_elements(draw_config_sptr draw_config_)
	{
		glDrawElements(draw_config_->get_mode(),
					   draw_config_->get_count(),
					   draw_config_->get_type(),
					   draw_config_->get_indices());
	}

	void swap_window(const sdl_window_sptr& window_ptr)
	{
		SDL_GL_SwapWindow(window_ptr.get());
	}

private:
	SDL_GLContext gl_context;

	shader_sptr	shader_program;

	vertex_array_object_sptr   vao;
	vertex_buffer_object_sptr  vbo;
	element_buffer_object_sptr ebo;

	draw_config_sptr draw_config;
};

ogl_render_api::ogl_render_api() : base_render_api(), pimpl{std::make_unique<ogl_render_api_pimpl>()} { }

ogl_render_api::ogl_render_api(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
	: base_render_api(window_ptr), pimpl {std::make_unique<ogl_render_api_pimpl>(window_ptr, scene_info)} { }

ogl_render_api::ogl_render_api(ogl_render_api&& api) noexcept = default;

ogl_render_api& ogl_render_api::operator=(ogl_render_api&& api) noexcept = default;

ogl_render_api::~ogl_render_api() = default;

void ogl_render_api::create(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info)
{
	this->pimpl->create(window_ptr, scene_info);
}

void ogl_render_api::swap(ogl_render_api& api) noexcept 
{
	base_render_api::swap(api);
	this->pimpl.swap(api.pimpl);
}

void ogl_render_api::reset() noexcept 
{
	this->pimpl->reset();
}

void ogl_render_api::clear_color()
{
	this->pimpl->clear_color();
}

void ogl_render_api::use_shader_program(u32 shader_id)
{
	this->pimpl->use_shader_program(shader_id);
}

void ogl_render_api::bind_g_uniforms(const std::vector<u32>& g_uniforms_, u64 time_) 
{
	this->pimpl->bind_g_uniforms(g_uniforms_, time_);
}

void ogl_render_api::bind_vao(u32 vao_id)
{
	this->pimpl->bind_vao(vao_id);
}

void ogl_render_api::bind_texture(u32 texture_id_, u32 active_texture_id_)
{
	this->pimpl->bind_texture(texture_id_, active_texture_id_);
}

void ogl_render_api::draw_arrays()
{
	this->pimpl->draw_arrays();
}

void ogl_render_api::draw_elements(draw_config_sptr draw_config)
{
	this->pimpl->draw_elements(draw_config);
}

void ogl_render_api::swap_window()
{
	this->pimpl->swap_window(this->get_window().lock());
}