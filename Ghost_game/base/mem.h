#pragma once


#include <iostream>
#include <assert.h>

struct Allocator {
	virtual ~Allocator() { }
	virtual void*				allocate(size_t size, size_t alignment) = 0;
	virtual void                deallocate(size_t marker) = 0;
};

struct StackAllocator : public Allocator
{
	uint8_t* memory = nullptr;
	size_t alloc_marker = 0;
	size_t total_size = 0;

	void init(size_t size);
	void shutdown();

	void* allocate(size_t size, size_t alignment);
	void deallocate(size_t marker);

	size_t get_marker();

	void clear();
};

struct DoubleStackAllocator : public Allocator
{
	uint8_t* memory = nullptr;
	size_t total_size = 0;
	size_t top = 0;
	size_t bottom = 0;

	void init(size_t size);
	void shutdown();

	void* allocate_top(size_t size, size_t alignment);
	void* allocate_bottom(size_t size, size_t alignment);

	void deallocate_top(size_t marker);
	void deallocate_bottom(size_t marker);

	size_t get_topmarker();
	size_t get_bottommarker();

	void clear_bottom();
	void clear_top();
};


struct LinearAllocator : public Allocator
{
	uint8_t* memory = nullptr;
	size_t total_size = 0;
	size_t alloc_marker = 0;

	void init(size_t size);
	void shutdown();

	void* allocate(size_t size, size_t alignment) override;
	void deallocate(size_t marker) override;

	void clear();
};


void memory_copy(void* destination, void* source, size_t size);
size_t memory_align(size_t size, size_t alignment);


#define Kilo(size) 1024 * size
#define Mega(size) 1024 * 1024 * size
#define Giga(size) 1024 * 1024 * 1024 * size