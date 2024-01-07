#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

#include "root_directory.h" // This is a configuration file generated by CMake.

class file_system
{
private:
  typedef std::string (*Builder) (const std::string& path);

public:
  static std::string get_path(const std::string& path)
  {
    static std::string(*pathBuilder)(std::string const &) = get_path_builder();
    return (*pathBuilder)(path);
  }

private:
  static std::string const& get_root()
  {
    static char const * envRoot = getenv("LOGL_ROOT_PATH");
    static char const * givenRoot = (envRoot != nullptr ? envRoot : logl_root);
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
  }

  //static std::string(*foo (std::string const &)) getPathBuilder()
  static Builder get_path_builder()
  {
    if (get_root() != "")
		return &file_system::get_path_relative_root;
    else
		return &file_system::get_path_relative_binary;
  }

  static std::string get_path_relative_root(const std::string& path)
  {
    return get_root() + std::string("/") + path;
  }

  static std::string get_path_relative_binary(const std::string& path)
  {
    return "../../../" + path;
  }
};

// FILESYSTEM_H
#endif
