#include "spatial/resources/MemoryFileSystem.h"
#include <gtest/gtest.h>
#include <spatial/resources/PhysicalFileSystem.h>

using namespace spatial;

TEST(FileSystem, Resolve)
{
	auto root = PhysicalFileSystem{std::filesystem::current_path()};
	auto mem = root.mount<MemoryFileSystem>("mem");

	{
		auto result = root.resolve("");
		EXPECT_EQ(result.path, "");
		EXPECT_EQ(&result.fileSystem, &root);
	}

	{
		auto result = root.resolve("mem");
		EXPECT_EQ(result.path, "");
		EXPECT_EQ(&result.fileSystem, &*mem);
	}
}

TEST(FileSystem, OpenFileForRead)
{
	auto root = PhysicalFileSystem{std::filesystem::current_path() / "fs" / "a1"};
	root.mount<PhysicalFileSystem>("v1", std::filesystem::current_path() / "fs" / "a2");
	root.mount<PhysicalFileSystem>("v2", std::filesystem::current_path() / "fs");
	auto mem = root.mount<MemoryFileSystem>("m1");
	mem->define("memory_test.txt", "Hello, World! From Memory");

	EXPECT_EQ(root.readString("v1/unknown.txt"), "");

	EXPECT_EQ(root.readString("v1/test.txt"), "Hello, World! Inside A2");

	EXPECT_EQ(root.readString("v2/a1/b1/test.txt"), "Hello, World! Inside A1/B1");

	EXPECT_EQ(root.readString("b1/test.txt"), "Hello, World! Inside A1/B1");

	EXPECT_EQ(root.readString("m1/memory_test.txt"), "Hello, World! From Memory");
}

TEST(FileSystem, OpenFileForWrite)
{
	auto root = PhysicalFileSystem{std::filesystem::current_path() / "fs" / "a1"};
	root.mount<PhysicalFileSystem>("v1", std::filesystem::current_path() / "fs" / "a2");

	root.write("v1/test_write.txt", "Test Write");

	EXPECT_EQ(root.readString("v1/test_write.txt"), "Test Write");
}

TEST(FileSystem, ListPaths)
{
	auto root = PhysicalFileSystem{std::filesystem::current_path() / "fs"};
	root.mount<PhysicalFileSystem>("v1", std::filesystem::current_path() / "fs" / "a1" / "b1");
	auto mem = root.mount<MemoryFileSystem>("m1");
	mem->define("memory.txt", "123");
	mem->define("folder/memory2.txt", "1245");

	{
		auto result = root.list("v1/");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"test.txt", FileSystem::FileType::File}) != result.end());
	}

	{
		auto result = root.list("v1");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"test.txt", FileSystem::FileType::File}) != result.end());
	}

	{
		auto result = root.list("m1");
		EXPECT_EQ(result.size(), 2);
		EXPECT_TRUE(result.find({"memory.txt", FileSystem::FileType::File}) != result.end());
		EXPECT_TRUE(result.find({"folder", FileSystem::FileType::Directory}) != result.end());
	}

	{
		auto result = root.list("m1/folder");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"memory2.txt", FileSystem::FileType::File}) != result.end());
	}

	{
		auto result = root.list("");
		EXPECT_EQ(result.size(), 4);
		EXPECT_TRUE(result.find({"a1", FileSystem::FileType::Directory}) != result.end());
		EXPECT_TRUE(result.find({"a2", FileSystem::FileType::Directory}) != result.end());
		EXPECT_TRUE(result.find({"v1", FileSystem::FileType::Directory}) != result.end());
		EXPECT_TRUE(result.find({"m1", FileSystem::FileType::Directory}) != result.end());
	}
}