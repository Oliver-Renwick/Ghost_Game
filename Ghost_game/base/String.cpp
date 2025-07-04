#include "String.h"

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

/*TODo - Logging Function*/


void String_Buffer::init(Allocator* allocator, size_t initial_capacity)
{
	m_allocator = allocator;
	m_data = (char*)m_allocator->allocate(initial_capacity, alignof(char));
	assert(m_data);
	m_buffersize = (uint32_t)initial_capacity; 
	m_currentsize = 0;
}

void String_Buffer::clear()
{
	m_currentsize = 0;
	m_data[0] = 0;
}

void String_Buffer::append_f(const char* format, ...)
{
	if (m_currentsize >= m_buffersize)
	{
		printf("%s", "Buffer full please allocate more size");
		return;
	}

	va_list args;
	va_start(args, format);

#if defined(_MSC_VER)
	int written_chars = vsnprintf_s(&m_data[m_currentsize], m_buffersize - m_currentsize, _TRUNCATE, format, args);
#else
	int written_chars = vsnprintf(&m_data[m_currentsize], m_buffersize - m_currentsize, format, args);
#endif
	m_currentsize += (written_chars > 0) ? written_chars : 0;
	va_end(args);


	if (m_currentsize < m_buffersize)
	{
		m_data[m_currentsize] = 0;
	}
	else
	{
		m_data[m_buffersize - 1] = 0;
	}


	if (written_chars < 0)
	{
		assert(false);
		printf("%s", "New String is too big for current buffer, please allocate appropriate size");
	}
}

void String_Buffer::append(const char* string)
{
	append_f("%s", string);
}


void String_Buffer::append(const String_Buffer& other_buffer)
{
	if (other_buffer.m_currentsize == 0)
	{
		return;
	}

	if (m_currentsize + other_buffer.m_currentsize >= m_buffersize)
	{
		assert(false);
		printf("%s", "Buffer Overflow!!");
		return;
	}

	memcpy(&m_data[m_currentsize], other_buffer.m_data, other_buffer.m_currentsize);
	m_currentsize += other_buffer.m_currentsize;
}

void String_Buffer::append_m(void* memory, size_t size)
{
	if (m_currentsize + size >= m_buffersize)
	{
		assert(false);
		printf("%s", "Buffer Overflow");
		return;
	}

	memcpy(&m_data[m_currentsize], memory, size);
	m_currentsize += (uint32_t)size;
}

char* String_Buffer::append_use_f(const char* format, ...)
{
	uint32_t cached_offset = m_currentsize;

	if (m_currentsize >= m_buffersize)
	{
		assert(false);
		printf("%s", "Buffer Overflow, change buffer capacity");
		return nullptr;
	}

	va_list args;
	va_start(args, format);

#if defined(_MSC_VER)
	int written_chars = vsnprintf_s(&m_data[m_currentsize], m_buffersize - m_currentsize, _TRUNCATE ,format, args);
#else
	int written_chars = vsnprintf(&m_data[m_currentsize], m_buffersize - m_currentsize, format, args);
#endif
	m_currentsize += (written_chars > 0) ? written_chars : 0;
	va_end(args);

	if (written_chars < 0)
	{
		assert(false);
		printf("%s", "New String is too big for current buffer, please allocate appropriate size");
		return nullptr;
	}


	if (m_currentsize < m_buffersize)
	{
		m_data[m_currentsize] = 0;
		m_currentsize++;
	}
	else
	{
		m_data[m_buffersize - 1] = 0;
	}

	return this->m_data + cached_offset;
}

char* String_Buffer::append_use(const char* string)
{
	return append_use_f("%s", string);
}


char* String_Buffer::append_use_substring(const char* string, uint32_t start_index, uint32_t end_index)
{
	uint32_t size = end_index - start_index;
	if (m_currentsize + size >= m_buffersize)
	{
		assert(false);
		printf("%s", "Buffer Overflow");
		return nullptr;
	}

	uint32_t cached_offset = m_currentsize;

	memcpy(&m_data[m_currentsize], string, size);
	m_currentsize += size;

	if (m_currentsize < m_buffersize)
	{
		m_data[m_currentsize] = 0;
	}
	else
	{
		m_data[m_buffersize - 1] = 0;
	}

	return m_data + cached_offset;
}

void String_Buffer::close_current_string()
{
	m_data[m_currentsize] = 0;
	++m_currentsize;
}

uint32_t String_Buffer::get_index(const char* text) const
{
	uint64_t differnce = text - m_data;
	return (differnce < m_buffersize) ? (uint32_t)differnce : UINT32_MAX;
}

const char* String_Buffer::get_text(uint32_t index) const
{
	return (index < m_buffersize) ? m_data + index : nullptr;
}

char* String_Buffer::reserve(size_t size)
{
	if (m_currentsize + size >= m_buffersize)
		return nullptr;

	uint32_t cached_offset = m_currentsize;
	m_currentsize += (uint32_t)size;

	return m_data + cached_offset;
}