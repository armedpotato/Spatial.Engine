#pragma once

#include <spatial/spatial.h>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

extern std::filesystem::path g_basePath;

spatial::core::Application& app();

std::vector<char> read(const std::filesystem::path& filePath);

inline spatial::render::RenderEngine& engine()
{
    return app().getRenderSys().getEngine();
}

inline const spatial::desktop::Window& window()
{
    return app().getRenderSys().getWindow();
}

inline spatial::render::View& view()
{
    return app().getRenderSys().getMainView();
}

inline spatial::render::Camera& camera()
{
    return app().getRenderSys().getMainCamera();
}