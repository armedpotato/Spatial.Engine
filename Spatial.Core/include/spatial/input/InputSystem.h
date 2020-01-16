#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/input/InputState.h>
#include <spatial/core/ApplicationConnector.h>

namespace spatial::input
{

class InputSystem
{
    using self_t = InputSystem;

private:
    core::AppEventConnector<desktop::MouseMovedEvent, self_t> m_mouseConnector;
    core::AppEventConnector<desktop::KeyEvent, self_t> m_keyConnector;
    core::AppEventConnector<desktop::TextEvent, self_t> m_textConnector;
    core::AppSignalsConnector<self_t> m_signalsConnector;

public:
    InputSystem(core::Application& app);

    void onStart();
    void onStartFrame(float delta);

    void onEvent(const desktop::MouseMovedEvent &event);
    void onEvent(const desktop::KeyEvent &event);
    void onEvent(const desktop::TextEvent &event);

};

} // namespace spatial
