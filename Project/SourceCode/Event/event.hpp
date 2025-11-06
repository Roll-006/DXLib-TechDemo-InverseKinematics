#pragma once
#include "../Interface/i_event.hpp"
#include "../Kind/event_kind.hpp"
#include "../IncludeList/event.hpp"

template <typename EventDataT>
struct Event : public IEvent
{
	EventKind	event_kind;
	EventDataT	data;

	Event(const EventKind event_kind, EventDataT data) : 
		event_kind(event_kind), data(data){ }

	[[nodiscard]] std::type_index GetType() const override { return std::type_index(typeid(EventDataT)); }
};
