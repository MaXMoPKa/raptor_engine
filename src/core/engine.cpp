#include "engine.hpp"

using namespace raptor_engine;

class engine::engine_pimpl
{
public:

	engine_pimpl() : hardware_sys(std::make_shared<hardware_system>()) { }

	engine_pimpl(const engine_data_sptr& engine_info)
		: engine_info {engine_info}, hardware_sys {std::make_shared<hardware_system>(engine_info->hardware_system_info)}, 
		render_engine {this->hardware_sys->get_window()}
	{
	}

public:

	void create(const engine_data_sptr& engine_info)
	{
		engine_pimpl tmp {engine_info};
		this->swap(tmp);
	}

	void swap(engine_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->hardware_sys, pimpl.hardware_sys);
		std::swap(this->render_engine, pimpl.render_engine);
		std::swap(this->engine_info, pimpl.engine_info);
	}

	void reset() noexcept
	{
		engine_pimpl tmp {};
		this->swap(tmp);
	}

 public:

    void run() noexcept
	{
		bool window_should_close = false;
		while (!window_should_close) 
		{ 
			this->hardware_sys->process_events(window_should_close);

			this->render_engine.pre_update();
			this->render_engine.update();
			this->render_engine.post_update();
		}
	}

 public:
  [[nodiscard]] engine_data_sptr get_engine_data() const {
    return this->engine_info;
  }

  [[nodiscard]] hardware_system_sptr get_hardware_system() const {
    return this->hardware_sys;
  }

  [[nodiscard]] render_engine* get_render_engine()
  {
	return &this->render_engine;
  }

 private:
  engine_data_sptr engine_info;

  hardware_system_sptr hardware_sys{};

  render::render_engine render_engine {};
};

engine::engine() : pimpl{std::make_unique<engine_pimpl>()} {}

engine::engine(const engine_data_sptr& engine_info)
    : pimpl{std::make_unique<engine_pimpl>(engine_info)} {}

engine::engine(engine&& engine) noexcept = default;
engine& engine::operator=(engine&& engine) noexcept = default;

engine::~engine() = default;

void engine::create(const engine_data_sptr& engine_info) {
  this->pimpl->create(engine_info);
}

void engine::swap(engine& engine) noexcept { this->pimpl.swap(engine.pimpl); }

void engine::reset() noexcept { this->pimpl->reset(); }

void engine::run() noexcept 
{
  this->pimpl->run();
}

engine_data_sptr engine::get_engine_data() const {
  return this->pimpl->get_engine_data();
}

hardware_system_sptr engine::get_hardware_system() const {
  return this->pimpl->get_hardware_system();
}


render_engine* engine::get_render_engine()
{
  return this->pimpl->get_render_engine();
}