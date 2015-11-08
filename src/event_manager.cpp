#include <string.h>
#include <assert.h>
#include "event_manager.hpp"
#include "precompiled.hpp"

using namespace ces;

//------------------------------------------------------------------------------
u32 EventManager::RegisterListener(event::EventType type, const fnEventListener& listener)
{
  _listeners[type].push_back(listener);
  return (u32)_listeners[type].size();
}

//------------------------------------------------------------------------------
void EventManager::AddEvent(event::EventType type, void* data, int len)
{
  assert(_eventOfs + len + sizeof(u32) < EVENT_BUF_SIZE);

  // hmm, is there a better way to do this?
  event::EventBase* base = (event::EventBase*)data;
  base->len = len;

  memcpy(&_eventBuf[_eventOfs], data, len);
  _eventOfs += len;
}

//------------------------------------------------------------------------------
void EventManager::Tick()
{
  for (char* cur = &_eventBuf[0]; cur < &_eventBuf[_eventOfs];)
  {
    event::EventBase* event = (event::EventBase*)cur;
    for (const fnEventListener& listener : _listeners[event->type])
    {
      listener(event);
    }
    cur += event->len;
  }

  // reset the event queue
  _eventOfs = 0;
}