#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/CameraControllers.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/ResourceManager.h>

namespace fl = filament;

namespace spatial
{

class Sandbox
{
	struct MaterialData
	{
		float metallic{.5f};
		float roughness{.4f};
	};

	struct CameraData
	{
		float sensitivity;
		float velocity;
	};

private:
	fl::Engine* m_engine;
	fl::Camera* m_camera;
	fl::View* m_view;

	SimpleCameraView m_cam;
	MaterialData m_materialData;
	CameraData m_cameraData;

	bool showEngineGui{true};
	bool enabledCameraController{true};

	TextureManager m_textureManager;
	MaterialManager m_materialManager;
	MaterialInstanceManager m_materialInstanceManager;
	MeshManager m_meshManager;

	Scene m_scene;
	Entity m_light;
	Skybox m_skybox;
	IndirectLight m_indirectLight;

public:
	explicit Sandbox(RenderingSystem& renderingSystem);

	void onEvent(const MouseMovedEvent& e);

	void onUpdateFrame(float delta);

	void onDrawGui();
};
} // namespace spatial
