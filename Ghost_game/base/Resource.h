#pragma once
#include "Array.h"
#include "Filemanager.h"
#include "enum.h"

#define MAX_RESOURCE 100


struct Resource_Pool
{
	uint32_t				current_index = 0;
	uint32_t				max_size = MAX_RESOURCE;

	const char*				path[MAX_RESOURCE];
	resource_type::Enum		type[MAX_RESOURCE];
	uint32_t				reference[MAX_RESOURCE];

	uint32_t obtain_ID();
};


struct ResourceManager
{
	//Use linear allocator instead of stack allocator for this subsystem.
	LinearAllocator         m_allocator;
	Allocator*              temp_allocator;
	const char*				m_asset_path = nullptr;
	FileManager				m_file_sys;
	Resource_Pool			m_resource_pool;
	//HashMap<string, int>  resource_map;			//This is where the resource pool index will be accessed through resource name

	void Init(Allocator* allocator);
	void populate_asset();
	void shutdown();
};

void print_resource_path(Resource_Pool& _resource_pool);

