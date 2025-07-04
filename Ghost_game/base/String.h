#pragma once
#include "mem.h"

#define MAX_STRING_SIZE 512

struct String_Buffer
{

	char* m_data = nullptr;
	Allocator* m_allocator = nullptr;
	uint32_t m_buffersize = 1024;
	uint32_t m_currentsize = 0;


	void init(Allocator* _allocator, size_t initial_capacity);
	void clear();

	void append(const char* string);
	void append(const String_Buffer& other_buffer);
	void append_f(const char* format, ...);
	void append_m(void* memory, size_t size);

	char* append_use(const char* string);
	char* append_use_f(const char* format, ...);
	char* append_use_substring(const char* string, uint32_t start_index, uint32_t end_index);

	void close_current_string();

	char* current() { return m_data + m_currentsize; }
	const char* get_full_string() { return m_data; }

	uint32_t get_index(const char* text) const;
	const char* get_text(uint32_t index) const;
	char* reserve(size_t size);
};