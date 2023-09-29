#pragma once

#include <vector>
#include <memory>
#include <string>

namespace raptor_engine {
namespace structs {

struct render_object_data
{
public:
	render_object_data();

	render_object_data(const std::string& path_vs_, const std::string& path_fs_, const std::vector<float>& vertices_);
	
	~render_object_data();

public:

	std::string vs_path;
	std::string fs_path;
	std::vector<float> vertices;
};

using render_object_data_uptr = std::unique_ptr<render_object_data>;
using render_object_data_sptr = std::shared_ptr<render_object_data>;
using render_object_data_wptr = std::weak_ptr<render_object_data>;

using render_objects = std::vector<render_object_data>;

}  // namespace structs
}  // namespace raptor_engine