# https://decovar.dev/blog/2021/03/08/cmake-cpp-library/#the-library

cmake_minimum_required(VERSION 3.25)

project(nador VERSION 1.0)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

# CXX Flags
set(NADOR_CXX_FLAGS "")

set(NADOR_WINDOW_OPTIONS "SDL2, GLFW")
set(NADOR_VIDEO_OPTIONS "OPENGL_3")

option(NADOR_VIDEO_DEBUG_ENABLE     "Enable video debug"                        ON)
option(NADOR_IMGUI_ENABLE           "Enable imgui."                             ON)

set(NADOR_WINDOW_API                "SDL2" CACHE STRING "Enable window support. Options: ${NADOR_WINDOW_OPTIONS}")
set(NADOR_VIDEO_API                 "OPENGL_3" CACHE STRING "The nador video api. Options: ${NADOR_VIDEO_OPTIONS}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -D_DEBUG")
endif()

if(NADOR_VIDEO_DEBUG_ENABLE)
    set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DNADOR_VIDEO_DEBUG")
endif()

if(${NADOR_VIDEO_API} STREQUAL "OPENGL_3")
    set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DUSE_OPENGL_3")
endif()

if(NADOR_IMGUI_ENABLE)
    set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DUSE_IMGUI")
endif()

if(WIN32)
    set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DPLATFORM_WINDOWS")
endif()

set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DDR_MP3_IMPLEMENTATION")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DDR_WAV_IMPLEMENTATION")

if(${NADOR_WINDOW_API} STREQUAL "SDL2")
    set(NADOR_CXX_FLAGS "${NADOR_CXX_FLAGS} -DUSE_SDL_WINDOW")
elseif(${NADOR_WINDOW_API} STREQUAL "GLFW")
    set(NADOR_CXX_FLAGS "${NADOR_CXX_FLAGS} -USE_GLFW_WINDOW")
else()
    message(FATAL_ERROR "Base platform not set! You need to set NADOR_WINDOW_API! Options: ${NADOR_WINDOW_OPTIONS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NADOR_CXX_FLAGS}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LIBTYPE "STATIC")

set(NADOR_SRC_DIR nador)
set(TINYXML2_DIR dep/tinyxml2)
set(GLFW_DIR dep/glfw)
set(GLEW_DIR dep/glew)
set(OPENAL_SOFT_DIR dep/openal-soft)
set(FREETYPE_DIR dep/freetype)
set(IMGUI_DIR dep/imgui)
set(STB_IMAGE_DIR dep/stb_image)
set(SDL_2_DIR dep/sdl2)

option(NADOR_EIMAGE_NAME_PATH "Path to EImageName.h" "")

add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLEW_DIR}/build/cmake)
add_subdirectory(${OPENAL_SOFT_DIR})
add_subdirectory(${FREETYPE_DIR})
add_subdirectory(${SDL_2_DIR})

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} LINK_PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} LINK_PRIVATE glew_s)
target_link_libraries(${PROJECT_NAME} LINK_PRIVATE OpenAL)
target_link_libraries(${PROJECT_NAME} LINK_PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} LINK_PRIVATE SDL2-static)

target_sources(${PROJECT_NAME}
    PRIVATE
    # nador source
    ${NADOR_SRC_DIR}/App.cpp

    ${NADOR_SRC_DIR}/common/GlobalEvents.cpp
    ${NADOR_SRC_DIR}/common/GlobalListener.cpp

    ${NADOR_SRC_DIR}/sound/open_al/OpenAlSoundController.cpp

    ${NADOR_SRC_DIR}/system/Factory.cpp

    ${NADOR_SRC_DIR}/system/file/WindowsFileController.cpp

    ${NADOR_SRC_DIR}/system/input/GLFWInputController.cpp
    ${NADOR_SRC_DIR}/system/input/SDLInputController.cpp

    ${NADOR_SRC_DIR}/system/window/WindowGLFW.cpp
    ${NADOR_SRC_DIR}/system/window/WindowSDL.cpp

    ${NADOR_SRC_DIR}/test/DebugMenu.cpp
    ${NADOR_SRC_DIR}/test/TestCommon.cpp
    ${NADOR_SRC_DIR}/test/TestController.cpp

    ${NADOR_SRC_DIR}/test/test_views/AtlasTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/ClearColorTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/FontTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/InputTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/SoundTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/TextureTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/UiElementsTest.cpp
    ${NADOR_SRC_DIR}/test/test_views/UiSquareTest.cpp

    ${NADOR_SRC_DIR}/ui/IUiElement.cpp
    ${NADOR_SRC_DIR}/ui/UiAligner.cpp
    ${NADOR_SRC_DIR}/ui/UiApp.cpp
    ${NADOR_SRC_DIR}/ui/UiButton.cpp
    ${NADOR_SRC_DIR}/ui/UiCommon.cpp
    ${NADOR_SRC_DIR}/ui/UiEdgeDrawer.cpp
    ${NADOR_SRC_DIR}/ui/UiEdgeDrawer.cpp
    ${NADOR_SRC_DIR}/ui/UiEditText.cpp
    ${NADOR_SRC_DIR}/ui/UiImage.cpp
    ${NADOR_SRC_DIR}/ui/UiLogicState.cpp
    ${NADOR_SRC_DIR}/ui/UiOverlay.cpp
    ${NADOR_SRC_DIR}/ui/UiParser.cpp
    ${NADOR_SRC_DIR}/ui/UiScene.cpp
    ${NADOR_SRC_DIR}/ui/UiScrollList.cpp
    ${NADOR_SRC_DIR}/ui/UiTextLabel.cpp

    ${NADOR_SRC_DIR}/utils/ImGuiHelper.cpp
    ${NADOR_SRC_DIR}/utils/TickTracker.cpp
    ${NADOR_SRC_DIR}/utils/TinyXmlHelper.cpp
    ${NADOR_SRC_DIR}/utils/Utils.cpp

    ${NADOR_SRC_DIR}/video/TextureLoader.cpp

    ${NADOR_SRC_DIR}/video/atlas/Atlas.cpp
    ${NADOR_SRC_DIR}/video/atlas/AtlasController.cpp

    ${NADOR_SRC_DIR}/video/buffers/IndexBuffer.cpp
    ${NADOR_SRC_DIR}/video/buffers/Texture.cpp
    ${NADOR_SRC_DIR}/video/buffers/VertexBuffer.cpp

    ${NADOR_SRC_DIR}/video/font/Font.cpp
    ${NADOR_SRC_DIR}/video/font/FontController.cpp

    ${NADOR_SRC_DIR}/video/material/Material.cpp

    ${NADOR_SRC_DIR}/video/opengl3/APIOpenGL3.cpp
    ${NADOR_SRC_DIR}/video/opengl3/VideoOpenGL3.cpp

    ${NADOR_SRC_DIR}/video/opengl3/shaders/ShaderOpenGL3.cpp
    
    ${NADOR_SRC_DIR}/video/renderer/RenderData.cpp
    ${NADOR_SRC_DIR}/video/renderer/Renderer.cpp

    ${NADOR_SRC_DIR}/video/renderer/simple_renderer/SimpleRenderer.cpp

    ${NADOR_SRC_DIR}/video/renderer/batch_renderer/BatchRenderData.cpp
    ${NADOR_SRC_DIR}/video/renderer/batch_renderer/BatchRenderer.cpp

    ${NADOR_SRC_DIR}/video/shader/Shader.cpp
    ${NADOR_SRC_DIR}/video/shader/ShaderController.cpp

    # tinyxml2
    ${TINYXML2_DIR}/tinyxml2.cpp

    # imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp

    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp

    # stb_image
    ${STB_IMAGE_DIR}/stb_image.cpp
)

if(NOT NADOR_EIMAGE_NAME_PATH)
    message(FATAL_ERROR "NADOR_EIMAGE_NAME_PATH is not set")
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/

        ${CMAKE_CURRENT_SOURCE_DIR}/nador

        ${CMAKE_CURRENT_SOURCE_DIR}/${GLFW_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/${GLEW_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/${OPENAL_SOFT_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/${FREETYPE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/${SDL_2_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/${IMGUI_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/dep
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/json-develop/single_include
        
        # location of header EImageName
        ${NADOR_EIMAGE_NAME_PATH}
)

# Or you can set it globally for the entire project on configuration with -DCMAKE_DEBUG_POSTFIX="d".
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

#------------------ Unit tests -------------------------------
option(NADOR_BUILD_UNIT_TEST    "Building unit tests"   ON)

if(NADOR_BUILD_UNIT_TEST)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
