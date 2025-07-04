#pragma once
#include <assert.h>
#include <stdio.h>
#include <iostream>


#define G_ASSERT(condition, message, ...)                            \
    do {                                                             \
        if (!(condition)) {                                          \
            std::cout << "\033[91m"; \
            std::cout << "[ASSERTION FAILED] " << #condition << "\n"; \
            std::cout << "Message: " << message << std::endl; \
            std::cout << "File: " << __FILE__ << " | Line: " << __LINE__ << "\n"; \
            std::cout << "\033[0m"; \
            assert(false); \
        }\
    } while (0)	


#define G_WARN(condition, message)                                                \
    do {                                                                          \
        if (!(condition)) {                                                       \
            std::cout << "\033[93m"; \
            std::cout << "[WARNING] Condition failed: " << #condition << "\n"; \
            std::cout << "Message: " << message << std::endl; \
            std::cout << "File: " << __FILE__ << " | Line: " << __LINE__ << "\n"; \
            std::cout << "\033[0m"; \
        }                                                                         \
    } while (0)



#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << " Failed " << "\" in " << __FILE__ << " at line " << __LINE__ << "\n"; \
		assert(res == VK_SUCCESS);																		\
	}																									\
}