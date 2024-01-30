#pragma once

#include <cstddef>

namespace raptor_engine {
namespace ecs {
namespace event {

class IEvent;
class IEventDelegate;

class IEventDispatcher
{
public:
	IEventDispatcher()			= default;
	virtual ~IEventDispatcher() = default;

public:
	virtual void Dispatch(IEvent* event) = 0;

	virtual void AddEventCallback(IEventDelegate* eventDelegate_) = 0;

	virtual void RemoveEventCallback(IEventDelegate* eventDelegate_) = 0;

	virtual std::size_t GetEventCallbackCount() const = 0;

};

using IEventDispatcherUptr = std::unique_ptr<IEventDispatcher>;
using IEventDispatcherSptr = std::shared_ptr<IEventDispatcher>;
using IEventDispatcherWptr = std::weak_ptr<IEventDispatcher>;

} // namespace event
} // namespace ecs
} // namespace raptor_engine