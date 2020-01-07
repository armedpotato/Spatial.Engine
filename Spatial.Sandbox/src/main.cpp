#include "utils.h"

namespace fl = filament;
using namespace spatial;
using namespace spatial::math;
using namespace spatial::desktop;
using namespace spatial::core;
using namespace spatial::common;
using namespace spatial::render;
using namespace spatial::input;
using namespace std::filesystem;

class SandboxLayer
{
private:
    fl::Engine* m_engine;
    fl::Camera* m_camera;
    fl::View* m_view;

    Scene m_scene;
    Material m_material;
    EntityResource m_light;

    int c;

public:
    SandboxLayer(Application& app)
        : m_engine{app.getRenderSys().getEngine()},
          m_camera{app.getRenderSys().getMainCamera()},
          m_view{app.getRenderSys().getMainView()},

          m_scene{createScene(m_engine)},
          m_material{createMaterial(m_engine, read(path{"materials"} / "plastic.filamat"))},
          m_light{createEntity(m_engine)},
          c{0}
    {
    }

    void onStart()
    {
        float3 eye{0, 0, 4}, center{0, 0, 0}, up{0, 1, 0};
        m_camera->lookAt(eye, center, up);

        m_view->setScene(m_scene.get());
        
        //instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});
        //instance->setParameter("roughness", 0.5f);
        //instance->setParameter("clearCoat", 1.0f);
        //instance->setParameter("clearCoatRoughness", 0.3f);

        fl::LightManager::Builder(fl::LightManager::Type::SUN)
            .color(fl::Color::toLinear<fl::ACCURATE>({0.98f, 0.92f, 0.89f}))
            .intensity(110000.0f)
            .direction({0.6f, -1.0f, -0.8f})
            .castShadows(true)
            .sunAngularRadius(1.9f)
            .sunHaloSize(10.0f)
            .sunHaloFalloff(80.0f)
            .build(*m_engine, m_light);

        m_scene->addEntity(m_light);

        Logger::info("Hello from start!");
    }

    void onUpdate(float delta)
    {
        if (Input::combined(Key::LShift, Key::D))
            Logger::info("Hello, world! {}", ++c);

        if (Input::combined(Key::LShift, Key::W))
            Logger::warn("Hello, world! {}", ++c);

        if (Input::combined(Key::LShift, Key::C))
            Logger::critical("Hello, world! {}", ++c);

        //Logger::info("FPS: {}", int(1/delta));
    }

    void onFinish()
    {
        m_scene->remove(m_light);
    }
};

int main(int arc, char *argv[])
{
    g_basePath      = path{argv[0]}.parent_path() / "assets" / "sandbox";
    auto app        = Application{};
    auto layer      = SandboxLayer{app};
    auto connector  = ApplicationConnector{app, &layer};

    return app.run();
}