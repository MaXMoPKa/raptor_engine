#pragma once

#include "ecs/platform.hpp"

namespace raptor_engine {
namespace ecs {

template <class T>
class FamilyTypeId
{
public:
	template <class U>
	static const TypeId get()
	{
		static const TypeId STATIC_TYPE_ID {count++};
		return STATIC_TYPE_ID;
	}

	static TypeId get()
	{
		return count;
	}

private:
	inline static TypeId count = 0;
};

} // namespace ecs
} // namespace raptor_engine