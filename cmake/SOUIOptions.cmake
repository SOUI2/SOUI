if(SOUI_OUTPUT_DIR)
  if(NOT IS_ABSOLUTE ${SOUI_OUTPUT_DIR})
    get_filename_component(SOUI_OUTPUT_DIR ${SOUI_OUTPUT_DIR} ABSOLUTE BASE_DIR ${CMAKE_BINARY_DIR})
  endif(NOT IS_ABSOLUTE ${SOUI_OUTPUT_DIR})

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOUI_OUTPUT_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(SOUI_OUTPUT_DIR)

option(SOUI_BUILD_CORE_STATIC "Build SOUI core as static library" OFF)
option(SOUI_BUILD_COMPONENTS_STATIC "Build SOUI components as static library" OFF)
option(SOUI_BUILD_DEMOS "Build SOUI demos" ON)
option(SOUI_WITH_UNICODE "Use Unicode" ON)
option(SOUI_WITH_STATIC_RUNTIME "Build SOUI with static runtime" OFF)
option(SOUI_WITH_XP_TOOLSET "Build for Windows XP" ON)
option(SOUI_WITH_BUILTIN_WCHAR "Treat wchar_t as built-in integral type" ON)

dump_variables(
  SOUI_OUTPUT_DIR
  SOUI_BUILD_CORE_STATIC
  SOUI_BUILD_COMPONENTS_STATIC
  SOUI_BUILD_DEMOS
  SOUI_WITH_UNICODE
  SOUI_WITH_STATIC_RUNTIME
  SOUI_WITH_XP_TOOLSET
  SOUI_WITH_BUILTIN_WCHAR
)

if(SOUI_BUILD_CORE_STATIC)
  set(SOUI_CORE_BUILD_TYPE STATIC)
else(SOUI_BUILD_CORE_STATIC)
  set(SOUI_CORE_BUILD_TYPE SHARED)
endif(SOUI_BUILD_CORE_STATIC)
set(ENABLE_SOUI_CORE_LIB ${SOUI_BUILD_CORE_STATIC})

if(SOUI_BUILD_COMPONENTS_STATIC)
  set(SOUI_COMPONENTS_BUILD_TYPE STATIC)
else(SOUI_BUILD_COMPONENTS_STATIC)
  set(SOUI_COMPONENTS_BUILD_TYPE SHARED)
endif(SOUI_BUILD_COMPONENTS_STATIC)
set(ENABLE_SOUI_COM_LIB ${SOUI_BUILD_COMPONENTS_STATIC})

if(SOUI_WITH_UNICODE)
  add_definitions(-DUNICODE -D_UNICODE)
  set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -DUNICODE -D_UNICODE")
  set(BUILD_CONF_UNICODE 1)
else(SOUI_WITH_UNICODE)
  add_definitions(-D_MBCS)
  set(BUILD_CONF_UNICODE 0)
endif(SOUI_WITH_UNICODE)

if(SOUI_WITH_STATIC_RUNTIME)
  include(MSVCStaticRuntime)
  set(BUILD_CONF_MT 1)
else(SOUI_WITH_STATIC_RUNTIME)
  set(BUILD_CONF_MT 1)
endif(SOUI_WITH_STATIC_RUNTIME)

if(SOUI_WITH_XP_TOOLSET)
  if (MSVC_VERSION GREATER 1700) # 1700 is Visual Studio 2012.
    add_definitions(-D_USING_V110_SDK71_)
  endif()
  set(BUILD_CONF_XP_TOOLSET 1)
else(SOUI_WITH_XP_TOOLSET)
  set(BUILD_CONF_XP_TOOLSET 0)
endif(SOUI_WITH_XP_TOOLSET)

if (NOT SOUI_WITH_BUILTIN_WCHAR)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zc:wchar_t-")
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zc:wchar_t-")
  set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /Zc:wchar_t-")
  set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zc:wchar_t-")
  set(BUILD_CONF_WCHAR 0)
else(NOT SOUI_WITH_BUILTIN_WCHAR)
  set(BUILD_CONF_WCHAR 1)
endif(NOT SOUI_WITH_BUILTIN_WCHAR)

set(CMAKE_DEBUG_POSTFIX "d")

if(SOUI_BUILD_DEMOS)
  add_subdirectory(demo)
endif(SOUI_BUILD_DEMOS)
