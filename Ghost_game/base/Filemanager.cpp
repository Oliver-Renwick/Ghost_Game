#include "Filemanager.h"
#include <Windows.h>

size_t file_size(FILE* file)
{
	size_t file_length;
	fseek(file, 0, SEEK_END);

	file_length = ftell(file);

	fseek(file, 0, SEEK_SET);

	return file_length;
}



char* FileManager::read_text_file(const char* filepath, Allocator* allocator, size_t* size)
{
	char* text = nullptr;

	FILE* file;
	fopen_s(&file,filepath, "r");
	if (file)
	{
		size_t _size = file_size(file);
		text = (char*)allocator->allocate(_size, alignof(char));
		size_t read_text = fread(text, 1, _size, file);

		if (read_text != _size) { assert(false); }
		text[_size] = 0;
		if (size) { *size = _size; }
		fclose(file);
	}
	return text;
}

char* FileManager::read_binary_file(const char* filepath, Allocator* allocator, size_t* size)
{
	char* text = nullptr;

	FILE* file;
	fopen_s(&file, filepath, "rb");
	if (file)
	{
		size_t _size = file_size(file);
		text = (char*)allocator->allocate(_size, alignof(char));
		size_t read_text = fread(text, 1, _size, file);

		if (read_text != _size) { assert(false); }
		if (size) { *size = _size; }
		fclose(file);

	}
	return text;
}


FileResult FileManager::read_text_file(const char* filepath, Allocator* allocator)
{
	FileResult file_data{};

	FILE* file;
	fopen_s(&file, filepath, "r");
	if (file)
	{
		size_t size = file_size(file);
		file_data.data = (char*)allocator->allocate(size + 1, alignof(char));
		size_t read_text = fread(file_data.data, 1, size, file);
		
		if (read_text < size) { std::cout << "the read text is lower that allocated memory" << std::endl; }
		file_data.data[read_text] = 0;
		file_data.size = read_text;
		fclose(file);
	}

	return file_data;
}

FileResult FileManager::read_binary_file(const char* filepath, Allocator* allocator)
{
	FileResult file_data{};

	FILE* file;
	fopen_s(&file, filepath, "rb");
	if (file)
	{
		size_t size = file_size(file);
		file_data.data = (char*)allocator->allocate(size, alignof(char));
		size_t read_text = fread(file_data.data, 1, size, file);

		if (read_text != size) { assert(false); }
		file_data.size = size;
		fclose(file);
	}

	return file_data;
}

bool FileManager::file_exist(const char* filename)
{
	FILE* file;
	fopen_s(&file, filename, "r");
	if (!file)
	{
		return false;
	}

	fclose(file);
	return true;
}

bool FileManager::delete_file(const char* filepath)
{
	int result = remove(filepath);
	return result == 0;
}


const char* FileManager::get_asset_path(Allocator* m_allocator)
{
	const char* base_path = get_base_path();
	String_Buffer path;
	path.init(m_allocator, MAX_STRING_SIZE);
	path.append(base_path);
	path.append("\\Assets");

	const char* asset = path.get_full_string();

	return asset;

}