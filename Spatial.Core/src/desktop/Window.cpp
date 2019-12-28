#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <utility>
#include <cassert>

#if !defined(SPATIAL_PLATFORM_WINDOWS)
#include <unistd.h>
#else
#include <SDL_syswm.h>
#include <utils/unwindows.h>
#endif

using namespace spatial::common;

namespace spatial::desktop
{

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle{
		  SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						   width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE)}
{
	assert(m_windowHandle != nullptr);
}

Window::Window(Window &&other) noexcept
	: m_windowHandle(std::exchange(other.m_windowHandle, nullptr))
{
}

Window &Window::operator=(Window &&other) noexcept
{
	m_windowHandle = std::exchange(other.m_windowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (m_windowHandle)
		SDL_DestroyWindow(m_windowHandle);
}

void Window::onStartRender()
{
	//
}

void Window::onEndRender()
{
	SDL_Delay(16);
}

std::pair<uint32_t, uint32_t> Window::getFrameBufferSize() const
{
	int dsw, dsh;
	SDL_GL_GetDrawableSize(m_windowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

std::pair<int, int> Window::getWindowSize() const
{
	int w, h;
	SDL_GetWindowSize(m_windowHandle, &w, &h);

	return {w, h};
}

void *Window::getNativeHandle()
{
	SDL_SysWMinfo wmInfo;
	SDL_GetWindowWMInfo(m_windowHandle, &wmInfo);
	return wmInfo.info.win.window;
}

} // namespace spatial::desktop