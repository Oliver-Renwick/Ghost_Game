#include "Resource.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

void parse_resource(Resource_Pool& resource_pool, json json_data ,resource_type::Enum _resource_type, String_Buffer& string_buffer)
{
	
	for (size_t i = 0; i < json_data.size(); i++)
	{
		uint32_t id = resource_pool.obtain_ID();
		std::string model_path = json_data[i];

		resource_pool.path[id] = string_buffer.append_use(model_path.c_str());
		resource_pool.reference[id] = 0;
		resource_pool.type[id] = _resource_type;
	}
}

void print_resource_path(Resource_Pool& _resource_pool)
{
	if (_resource_pool.current_index != 0)
	{
		for (int i = 0; i < _resource_pool.current_index; i++)
		{
			std::cout << _resource_pool.path[i] << std::endl;
		}
	}

	else
	{
		std::cout << "There is no assets in the resource pool please populate them" << std::endl;
	}
}

uint32_t Resource_Pool::obtain_ID()
{
	if (current_index < max_size)
	{
		int id = current_index;
		current_index++;
		return id;
	}

	return g_invalid_index;
}

void ResourceManager::Init(Allocator* allocator)
{
	temp_allocator = allocator;
	m_allocator.init(Mega(1));			// for now use dedicated allocator maybe then we can use from Arena
	m_asset_path  = FileManager::get_asset_path(temp_allocator);

	populate_asset();
}


void ResourceManager::populate_asset()
{

	String_Buffer string_buffer;
	string_buffer.init(&m_allocator, 1024);

	string_buffer.append(m_asset_path);
	string_buffer.append("\\Resource.json");

	const char* resource_path = string_buffer.get_full_string();

	FileResult file_request = m_file_sys.read_text_file(resource_path, temp_allocator);

	json resource_data = json::parse(file_request.data);

	//Json data
	json _models = resource_data["Model"];
	json _textures = resource_data["Texture"];
	json _sounds = resource_data["Sound"];
	json _animations = resource_data["Animation"];


	//Parsing

	parse_resource(m_resource_pool, _models, resource_type::Model, string_buffer);
	parse_resource(m_resource_pool, _textures, resource_type::Texture, string_buffer);
	parse_resource(m_resource_pool, _sounds, resource_type::Sound, string_buffer);
	parse_resource(m_resource_pool, _animations, resource_type::Animation, string_buffer);

	//Print Check
	print_resource_path(m_resource_pool);
}

void ResourceManager::shutdown()
{
	m_allocator.shutdown();
}




