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

set(_DEBUG                  1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -D_DEBUG=${_DEBUG}")

set(NADOR_VIDEO_DEBUG       1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DNADOR_VIDEO_DEBUG=${NADOR_VIDEO_DEBUG}")

#set(USE_GLFW_WINDOW         1   CACHE STRING "C++ flag")
#set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DUSE_GLFW_WINDOW=${USE_GLFW_WINDOW}")

set(USE_OPENGL_3            1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DUSE_OPENGL_3=${USE_OPENGL_3}")

set(USE_IMGUI               1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DUSE_IMGUI=${USE_IMGUI}")

set(PLATFORM_WINDOWS        1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DPLATFORM_WINDOWS=${PLATFORM_WINDOWS}")

set(DR_MP3_IMPLEMENTATION   1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DDR_MP3_IMPLEMENTATION=${DR_MP3_IMPLEMENTATION}")

set(DR_WAV_IMPLEMENTATION   1   CACHE STRING "C++ flag")
set(NADOR_CXX_FLAGS             "${NADOR_CXX_FLAGS} -DDR_WAV_IMPLEMENTATION=${DR_WAV_IMPLEMENTATION}")

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

add_definitions(
    #-DUSE_GLFW_WINDOW
    -DUSE_SDL_WINDOW
)

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
        
        ${NADOR_EIMAGE_NAME_PATH}
        # ${CMAKE_CURRENT_SOURCE_DIR}/../res/atlas/__atlases
        

    #PUBLIC
    #    # where top-level project will look for the library's public headers
    #    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    #    # where external projects will look for the library's public headers
    #    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    #    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dep>
    #    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dep/json-develop/single_include>
    #    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/res/atlas/__atlases>
)

# note that it is not CMAKE_INSTALL_PREFIX we are checking here
if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()

# without it public headers won't get installed
set(public_headers
    #include/calculate.h
    #include/test/test.h
)

# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

# for CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

# the variant with PUBLIC_HEADER property unfortunately does not preserve the folder structure
# set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
# so instead we iterate through public headers and install them "manually"
#foreach(header ${public_headers})
#    file(RELATIVE_PATH header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" "${header}")   
#    get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
#    install(
#        FILES ${header}
#        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_directory_path}"
#    )
#endforeach()

# Or you can set it globally for the entire project on configuration with -DCMAKE_DEBUG_POSTFIX="d".
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

# install the target and create export-set
#install(TARGETS ${PROJECT_NAME}
#    EXPORT "${PROJECT_NAME}Targets"
#    # these get default values from GNUInstallDirs, no need to set them
#    #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
#    #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
#    #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
#    # except for public headers, as we want them to be inside a library folder
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} # include/SomeLibrary
#    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
#)

# generate and install export file
#install(EXPORT "${PROJECT_NAME}Targets"
#    FILE "${PROJECT_NAME}Targets.cmake"
#    NAMESPACE ${namespace}::
#    DESTINATION cmake
#)

#include(CMakePackageConfigHelpers)

# generate the version file for the config file
#write_basic_package_version_file(
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#    VERSION "${version}"
#    COMPATIBILITY AnyNewerVersion
#)
# create config file
#configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    INSTALL_DESTINATION cmake
#)
# install config files
#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#    DESTINATION cmake
#)
# generate the export targets for the build tree
#export(EXPORT "${PROJECT_NAME}Targets"
#    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
#    NAMESPACE ${namespace}::
#)