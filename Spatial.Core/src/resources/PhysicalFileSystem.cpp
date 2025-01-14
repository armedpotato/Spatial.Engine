#include <fstream>
#include <spatial/resources/PhysicalFileSystem.h>

namespace spatial
{

PhysicalFileSystem::PhysicalFileSystem(std::filesystem::path rootPath) : mRootPath{std::move(rootPath)}
{
}

std::unique_ptr<std::istream> PhysicalFileSystem::openReadStreamImpl(std::string_view path) noexcept
{
	auto absolutePath = makeFullPath(path);
	return std::make_unique<std::ifstream>(absolutePath, std::ios_base::binary);
}

std::unique_ptr<std::ostream> PhysicalFileSystem::openWriteStreamImpl(std::string_view path) noexcept
{
	auto absolutePath = makeFullPath(path);
	return std::make_unique<std::ofstream>(absolutePath, std::ios_base::binary);
}

std::filesystem::path PhysicalFileSystem::makeFullPath(std::string_view path) const noexcept
{
	return mRootPath / path;
}

std::set<FileSystem::Entry> PhysicalFileSystem::listImpl(std::string_view path) const
{
	auto result = std::set<Entry>{};

	auto absolutePath = makeFullPath(path);
	if (!std::filesystem::exists(absolutePath))
		return result;

	auto directoryView = std::filesystem::directory_iterator{absolutePath};

	for (auto& entry : directoryView)
		result.emplace(std::filesystem::relative(entry.path(), absolutePath).string(),
					   entry.is_directory() ? FileSystem::FileType::Directory : FileSystem::FileType::File);

	return result;
}

void PhysicalFileSystem::setRootPath(const std::filesystem::path& rootPath)
{
	mRootPath = rootPath;
}

} // namespace spatial