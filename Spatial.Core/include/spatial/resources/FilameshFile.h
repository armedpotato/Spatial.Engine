#pragma once

#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/resources/BoundingBox.h>

#include <cstdint>
#include <istream>
#include <string>
#include <vector>
#include <future>

namespace spatial
{

struct FilameshFileHeader
{
	uint32_t version{};
	uint32_t partsCount{};
	math::AxisAlignedBoundingBox aabb{};
	uint32_t flags{};
	uint32_t offsetPosition{};
	uint32_t stridePosition{};
	uint32_t offsetTangents{};
	uint32_t strideTangents{};
	uint32_t offsetColor{};
	uint32_t strideColor{};
	uint32_t offsetUV0{};
	uint32_t strideUV0{};
	uint32_t offsetUV1{};
	uint32_t strideUV1{};
	uint32_t vertexCount{};
	uint32_t vertexSize{};
	uint32_t indexType{};
	uint32_t indexCount{};
	uint32_t indexSize{};
};

struct FilameshFilePart
{
	uint32_t offset{};
	uint32_t count{};
	uint32_t minIndex{};
	uint32_t maxIndex{};
	uint32_t materialID{};
	math::AxisAlignedBoundingBox boundingBox{};
	std::string materialName{};
};

struct FilameshFile
{
	FilameshFileHeader header{};
	std::vector<FilameshFilePart> parts{};
	std::vector<uint8_t> vertexData{};
	std::vector<uint8_t> indexData{};
};

std::istream& operator>>(std::istream& stream, spatial::FilameshFilePart& part);

std::istream& operator>>(std::istream& stream, spatial::FilameshFileHeader& header);

std::istream& operator>>(std::istream& stream, spatial::FilameshFile& filamesh);

FilameshFile loadFilameshFromMemory(const std::vector<uint8_t>& data);

std::future<FilameshFile> loadFilameshFromMemoryAsync(std::vector<uint8_t>&& data);

} // namespace spatial
