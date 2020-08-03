#include <spatial/ui/ImGuiHelpers.h>

namespace fs = std::filesystem;
namespace fl = filament;

namespace spatial
{

void imguiRefreshViewport(std::uint32_t width, std::uint32_t height, float scaleX, float scaleY)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
	io.DisplayFramebufferScale.x = scaleX;
	io.DisplayFramebufferScale.y = scaleY;
}

void imguiRefreshDeltaTime(float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = delta;
}

std::pair<int, int> imguiGetFrameSize()
{
	ImGuiIO& io = ImGui::GetIO();
	int fw = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fh = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);

	return {fw, fh};
}

bool imguiIsMinimized()
{
	auto [width, height] = imguiGetFrameSize();
	return width == 0 && height == 0;
}

VertexBuffer imguiCreateVertexBuffer(fl::Engine& engine, size_t capacity)
{
	return VertexBuffer{engine, fl::VertexBuffer::Builder()
									.vertexCount(capacity)
									.bufferCount(1)
									.attribute(fl::VertexAttribute::POSITION, 0,
											   fl::VertexBuffer::AttributeType::FLOAT2, 0, sizeof(ImDrawVert))
									.attribute(fl::VertexAttribute::UV0, 0, fl::VertexBuffer::AttributeType::FLOAT2,
											   sizeof(fl::math::float2), sizeof(ImDrawVert))
									.attribute(fl::VertexAttribute::COLOR, 0, fl::VertexBuffer::AttributeType::UBYTE4,
											   2 * sizeof(fl::math::float2), sizeof(ImDrawVert))
									.normalized(fl::VertexAttribute::COLOR)
									.build(engine)};
}

IndexBuffer imguiCreateIndexBuffer(fl::Engine& engine, size_t capacity)
{
	return IndexBuffer{
		engine,
		fl::IndexBuffer::Builder().indexCount(capacity).bufferType(fl::IndexBuffer::IndexType::USHORT).build(engine)};
}

uint64_t imguiMakeScissorKey(int frameBufferHeight, const ImVec4& clipRect)
{
	auto left = static_cast<uint16_t>(clipRect.x);
	auto bottom = frameBufferHeight - static_cast<uint16_t>(clipRect.w);
	auto width = static_cast<uint16_t>(clipRect.z - clipRect.x);
	auto height = static_cast<uint16_t>(clipRect.w - clipRect.y);
	return (static_cast<uint64_t>(left) << 0ull) | (static_cast<uint64_t>(bottom) << 16ull) |
		   (static_cast<uint64_t>(width) << 32ull) | (static_cast<uint64_t>(height) << 48ull);
}

Texture imguiCreateTextureAtlas(fl::Engine& engine, const std::vector<char>& resourceData)
{
	auto& io = ImGui::GetIO();

	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF(const_cast<char*>(resourceData.data()), resourceData.size(), 16.0f, &fontConfig);

	unsigned char* data;
	int width, height, bpp;

	io.Fonts->GetTexDataAsAlpha8(&data, &width, &height, &bpp);

	const auto size = size_t(width) * height * bpp;
	auto pb = fl::Texture::PixelBufferDescriptor{data, size, fl::Texture::Format::R, fl::Texture::Type::UBYTE};
	const auto texture = fl::Texture::Builder()
							 .width(static_cast<uint32_t>(width))
							 .height(static_cast<uint32_t>(height))
							 .levels(static_cast<uint8_t>(1))
							 .format(fl::Texture::InternalFormat::R8)
							 .sampler(fl::Texture::Sampler::SAMPLER_2D)
							 .build(engine);

	texture->setImage(engine, 0, std::move(pb));

	return createResource(engine, texture);
}

} // namespace spatial