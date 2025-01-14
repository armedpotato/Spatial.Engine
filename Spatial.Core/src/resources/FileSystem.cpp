#include <cassert>
#include <fmt/format.h>
#include <spatial/resources/FileSystem.h>
#include <sstream>

namespace spatial
{

void FileSystem::mount(std::string_view key, const SharedFileSystem& child)
{
	auto result = resolve(key);

	assert(result.path.find(SEPARATOR) == std::string_view::npos && !result.path.empty());
	result.fileSystem.mNodes.emplace(result.path, child);
}

FileSystem::ResolveResult FileSystem::resolve(std::string_view path) noexcept
{
	auto result = FileSystem::ResolveResult{std::string{path}, *this};
	if (path.empty())
		return result;

	auto separatorPos = path.find(SEPARATOR);
	if (separatorPos == std::string_view::npos)
	{
		auto it = mNodes.find(result.path);
		if (it != mNodes.end())
			return ResolveResult{"", *it->second};

		return result;
	}

	auto directoryName = std::string{path.substr(0, separatorPos)};

	auto it = mNodes.find(directoryName);
	if (it == mNodes.end())
		return result;

	auto restOfPath = path.substr(separatorPos + 1);
	return it->second->resolve(restOfPath);
}

std::unique_ptr<std::istream> FileSystem::openReadStream(std::string_view path) noexcept
{
	auto result = resolve(path);
	return result.fileSystem.openReadStreamImpl(result.path);
}

std::unique_ptr<std::ostream> FileSystem::openWriteStream(std::string_view path) noexcept
{
	auto result = resolve(path);
	return result.fileSystem.openWriteStreamImpl(result.path);
}

std::string FileSystem::readString(std::string_view path) noexcept
{
	auto stream = openReadStream(path);
	if (stream->fail())
		return "";

	return std::string{std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>()};
}

std::vector<uint8_t> FileSystem::readBinary(std::string_view path) noexcept
{
	auto stream = openReadStream(path);
	if (stream->fail())
		return std::vector<uint8_t>{};

	return std::vector<uint8_t>{std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>()};
}

bool FileSystem::write(std::string_view path, std::string&& data) noexcept
{
	auto stream = openWriteStream(path);
	if (stream->fail())
		return false;

	std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(*stream));
	return true;
}

std::set<FileSystem::Entry> FileSystem::list(std::string_view path) noexcept
{
	auto result = resolve(path);
	auto pathList = result.fileSystem.listImpl(result.path);

	for (auto& [k, _] : result.fileSystem.mNodes)
		pathList.emplace(k, FileType::Directory);

	return pathList;
}

std::future<std::vector<uint8_t>> FileSystem::readBinaryAsync(std::string_view path) noexcept
{
	return std::async(std::launch::async,
					  [this, p = std::string{path}]() -> std::vector<uint8_t> { return this->readBinary(p); });
}

std::unique_ptr<std::istream> AggregateFileSystem::openReadStreamImpl(std::string_view path) noexcept
{
	return makeFailedStream();
}

std::unique_ptr<std::ostream> AggregateFileSystem::openWriteStreamImpl(std::string_view path) noexcept
{
	return makeFailedStream();
}

std::set<FileSystem::Entry> AggregateFileSystem::listImpl(std::string_view path) const
{
	return std::set<Entry>{};
}

std::unique_ptr<std::iostream> AggregateFileSystem::makeFailedStream() noexcept
{
	auto stream = std::make_unique<std::stringstream>("");
	stream->setstate(std::ios::failbit);
	return stream;
}

} // namespace spatial