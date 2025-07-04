#pragma once

#include "mem.h"

#define ArraySize(array)  (sizeof(array) / sizeof((array)[0]))

template<typename T>
struct Array
{
	T* m_data;
	uint32_t m_size = 0;
	uint32_t m_capacity = 0;
	Allocator* m_allocator = nullptr;

	void Init(Allocator* allocator, uint32_t new_capacity, uint32_t new_size = 0);

	void grow(uint32_t new_capacity);
	void setsize(size_t new_size);
	void setcapacity(size_t new_capacity);
	void clear();

	T& back();
	T& front();

	const uint32_t size_in_bytes();
	const uint32_t capacity_in_bytes();

	void push(const T& element);
	T& push_use();

	void pop();

	T& operator[] ( uint32_t index);
	const T& operator[] (uint32_t index) const;
};

template <typename T>
inline T& Array<T>::operator[] (uint32_t index)
{
	assert(index < m_size);
	return m_data[index];
}

template<typename T>
inline const T& Array<T>::operator[] (uint32_t index) const
{
	assert(index < m_size);
	return m_data[index];
}

template<typename T>
inline void Array<T>::pop()
{
	assert(m_size > 0);
	m_size--;
}

template<typename T>
inline T& Array<T>::push_use()
{
	if (m_size >= m_capacity)
	{
		grow(m_capacity + 1);
	}

	++m_size;
	return back();
}

template<typename T>
inline T& Array<T>::back()
{
	assert(m_size);
	return m_data[0];
}

template<typename T>
inline T& Array<T>::front()
{
	assert(m_size);
	return m_data[m_size - 1];
}

template<typename T>
inline void Array<T>::setcapacity(size_t new_capacity)
{
	if (new_capacity > m_capacity)
	{
		grow(new_capacity);
	}
}

template<typename T>
inline void Array<T>::setsize(size_t new_size)
{
	if (new_size > m_capacity)
	{
		grow(new_size);
	}

	m_size = new_size;
}

template<typename T>
inline void Array<T>::clear()
{
	m_size = 0;
}

template<typename T>
inline void Array<T>::push(const T& element)
{
	if (m_size >= m_capacity)
	{
		grow(m_capacity + 1);
	}

	m_data[m_size++] = element;
}

template<typename T>
inline const uint32_t Array<T>::size_in_bytes()
{
	return m_size * sizeof(T);
}

template<typename T>
inline const uint32_t Array<T>::capacity_in_bytes()
{
	return m_capacity;
}



template<typename T>
inline void Array<T>::Init(Allocator* allocator, uint32_t initial_capacity, uint32_t initial_size)
{
	m_data = nullptr;
	m_size = initial_size;
	m_capacity = 0;
	m_allocator = allocator;

	if (initial_capacity > 0)
	{
		grow(initial_capacity);
	}
}

template<typename T>
inline void Array<T>::grow(uint32_t new_capacity)
{
	if (new_capacity < m_capacity * 2)
	{
		new_capacity = m_capacity * 2;
	}
	else if (new_capacity < 4)
	{
		new_capacity = 4;
	}

	T* new_data = (T*)m_allocator->allocate(new_capacity * sizeof(T), alignof(T));

	if (m_capacity)
	{
		memory_copy(new_data, m_data, m_size);

	}

	m_data = new_data;
	m_capacity = new_capacity;
}