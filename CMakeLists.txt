cmake_minimum_required(VERSION 3.6)
project(Universe2)

SET(CMAKE_CXX_STANDARD 14)


# Windows 10
SET(_HEXO_PLATFORM_OS_WINDOWS TRUE)
add_compile_definitions( HEXO_PLATFORM_OS_WINDOWS )
add_compile_definitions( HEXO_PLATFORM_OS_WINDOWS_10 )
add_compile_definitions( HEXO_GRAPHICS_RHI_WIN32_D3D12_GLFW )
add_compile_definitions( HEXO_GRAPHICS_WINDOW_GLFW )
add_compile_definitions( HEXO_GRAPHICS_RHI_D3D12 )
add_compile_definitions( HEXO_INPUT_IHI_WIN32_RAWINPUT )
# add_compile_definitions( HEXO_THREADING_THI_WIN32 )
add_compile_definitions( HEXO_FS_PLATFORM_WIN32 )
add_compile_definitions( WIN32_LEAN_AND_MEAN )

if (MSVC)
    add_compile_options(/permissive)
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD" )
else()
    add_compile_options(-Wall)
endif()



SET(CMAKE_BUILD_TYPE RelWithDebInfo)
add_compile_definitions(DEBUG)


include_directories(
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/inc
	${CMAKE_CURRENT_LIST_DIR}/src
	${CMAKE_CURRENT_LIST_DIR}/shd
	${CMAKE_CURRENT_LIST_DIR}/ext
)


add_executable(app
	src/main.cpp
    src/Application.cpp
    src/RenderPipeline.cpp
    src/FontRenderer.cpp
    src/Window.cpp
	src/Camera.cpp
    src/SpaceEngine.cpp
    src/ParticleOctree.cpp
    src/StaticParticlesBlock.cpp
    src/Multiverse.cpp
)

target_link_libraries(app
	${CMAKE_CURRENT_LIST_DIR}/lib/HexoInput.lib
    ${CMAKE_CURRENT_LIST_DIR}/lib/HexoGraphics.lib
	${CMAKE_CURRENT_LIST_DIR}/lib/HexoFileSystem.lib
)

add_custom_target(Shaders ALL
	COMMAND hxshc -i shd_compile.args -o ../build -p win -v -c ./ ./common --intermediate
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/shd
	COMMENT "Building shaders..."
)
