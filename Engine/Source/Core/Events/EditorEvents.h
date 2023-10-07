#ifndef ENGINE_EDITOREVENTS_H
#define ENGINE_EDITOREVENTS_H

#include "Event.h"

#include <sstream>

namespace Engine
{
    class EditorExitPlayModeEvent : public Event
    {
    public:
        EditorExitPlayModeEvent() = default;

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "EditorExitPlayModeEvent";
            return ss.str();
        }

        EVENT_CLASS_TYPE(EditorExitPlayMode)
        EVENT_CLASS_CATEGORY(EventCategoryEditor)
    };
} // namespace Engine

#endif // ENGINE_EDITOREVENTS_H
