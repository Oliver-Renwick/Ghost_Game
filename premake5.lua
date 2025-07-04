include "Dependencies.lua"

workspace "Ghost_game"
   configurations 
   { 
      "Debug", 
      "Release" 
   }

   platforms 
   { 
      "x64" 
   }     

project "Ghost_game"
   kind "WindowedApp"
   language "C++"
   cppdialect "C++20"

   targetdir "bin/%{cfg.buildcfg}"

   files 
   { 
       "Ghost_game/**.h",
       "Ghost_game/**.hpp",
       "Ghost_game/**.cpp",
       "Ghost_game/**.c" 
   }

   includedirs
   {
      "Ghost_game",
      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.KTX}",
      "%{IncludeDir.TINY_GLTF}",
      "%{IncludeDir.ImGUI}",
   } 

   

   links
   {
      "%{Library.Vulkan}",  
      "%{Library.ktx}"     
   }

   filter "system:windows"
      defines { 
         "VK_USE_PLATFORM_WIN32_KHR",
         "PROJECT_PATH=\"" .. os.getcwd() .. "\"" 
    } 

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter "platforms:x64"  
      architecture "x86_64"






