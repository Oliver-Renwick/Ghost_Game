
#include "mem.h"

size_t memory_align(size_t size, size_t alignment)
{
	assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of 2");
	const size_t align_mask = alignment - 1;
	return (size + align_mask) & ~align_mask;
}

void memory_copy(void* destination, void* source, size_t size)
{
	memcpy(destination, source, size);
}

/*	Stack Allocator	*/

void StackAllocator::init(size_t size)
{
	memory = (uint8_t*)malloc(size);
	alloc_marker = 0;
	total_size = size;
}


void* StackAllocator::allocate(size_t size, size_t alignment)
{
	size_t new_start = memory_align(alloc_marker, alignment);
	if (new_start > total_size) { std::cout << "We got to make lof funcctions" << std::endl; }
	size_t new_size = new_start + size;
	if (new_size > total_size) { std::cout << "Guess what I'm going to say!!" << std::endl; }

	alloc_marker = new_size;
	return memory + new_start;
}

void StackAllocator::deallocate(size_t marker)
{
	if (marker > total_size) { std::cout << "This would be out of bound" << std::endl; }
	//std::cout << "Check all the log problem when we have log function" << std::endl;

	alloc_marker = marker;
}

size_t StackAllocator::get_marker()
{
	return alloc_marker;
}



void StackAllocator::clear()
{
	alloc_marker = 0;
}

void StackAllocator::shutdown()
{
	if (memory)
		free(memory);

	memory = nullptr;
	alloc_marker = 0;
	total_size = 0;
}

/*	Double Stack Allocator	*/

void DoubleStackAllocator::init(size_t size)
{
	memory = (uint8_t*)malloc(size);
	bottom = 0;
	top = size;
	total_size = size;
}

void* DoubleStackAllocator::allocate_bottom(size_t size, size_t alignment)
{
	size_t new_start = memory_align(bottom, alignment);
	size_t new_size = new_start + size;
	if (new_start > total_size || new_size > total_size)
	{
		std::cout << "You are asking out of bound memory" << std::endl;
		return nullptr;
	}

	bottom = new_size;
	return memory + new_start;
}

void* DoubleStackAllocator::allocate_top(size_t size, size_t alignment)
{
	size_t new_start =  memory_align(top - size, alignment);
	if (new_start <= bottom) { return nullptr; }

	top = new_start;
	return memory + new_start;
}

void DoubleStackAllocator::deallocate_bottom(size_t marker)
{
	if (marker > top) { std::cout << "This would be out of bound" << std::endl; return; }
	bottom = marker;
}

void DoubleStackAllocator::deallocate_top(size_t marker)
{
	if (marker < bottom) { std::cout << "This would be out of bound" << std::endl; return; }
	top = marker;
}

size_t DoubleStackAllocator::get_bottommarker()
{
	return bottom;
}

size_t DoubleStackAllocator::get_topmarker()
{
	return top;
}

void DoubleStackAllocator::shutdown() 
{
	if (memory)
		free(memory);

	memory = nullptr;
	top = 0;
	bottom = 0;
	total_size = 0;
}

void DoubleStackAllocator::clear_bottom()
{
	bottom = 0;
}

void DoubleStackAllocator::clear_top()
{
	top = total_size;
}

void LinearAllocator::init(size_t size)
{
	memory = (uint8_t*)malloc(size);
	alloc_marker = 0;
	total_size = size;
}

void* LinearAllocator::allocate(size_t size, size_t alignment)
{
	size_t new_start = memory_align(alloc_marker, alignment);
	size_t new_size = new_start + size;
	if (new_size > total_size) { std::cout << "Well it out of bounds" << std::endl; return nullptr; }

	alloc_marker = new_size;
	return memory + new_start;
}

void LinearAllocator::deallocate(size_t marker)
{
	//well linear allocator dosent de alloccate i guess
}

void LinearAllocator::shutdown()
{
	clear();
	if (memory)
		free(memory);
}

void LinearAllocator::clear()
{
	alloc_marker = 0;
}