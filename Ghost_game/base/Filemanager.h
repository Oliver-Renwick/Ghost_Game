#pragma once

#include "mem.h"
#include "Array.h"
#include <stdio.h>
#include "String.h"

struct FileResult
{
	char* data = nullptr;
	size_t size;
};

struct FileManager
{


	char* read_text_file(const char* filepath, Allocator* allocator, size_t* size);
	char* read_binary_file(const char* filepath, Allocator* allocator, size_t* size);

	FileResult read_text_file(const char* filepath, Allocator* allocator);
	FileResult read_binary_file(const char* filepath, Allocator* allocator);

	bool file_exist(const char* filename);
	bool delete_file(const char* filepath);

	//The Project path should be built with CMake
	static const char* get_base_path() { return PROJECT_PATH; };
	static const char* get_asset_path(Allocator* allocator);
};