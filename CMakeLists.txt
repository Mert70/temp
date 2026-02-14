cmake_minimum_required(VERSION 3.16)

set(APP_NAME "IHA_deneme_1")
project(${APP_NAME} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SDL3
message(STATUS "Installing SDL3")
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

# ImGui
message(STATUS "Installing ImGui")
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# Libsodium
message(STATUS "Installing Libsodium")
FetchContent_Declare(
  Sodium
  GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
  GIT_TAG master
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

# glm
message(STATUS "Installing glm")
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Proje kaynakları
file(GLOB_RECURSE APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.cpp"
)

# ImGui kaynak dosyaları
set(IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

add_executable(${APP_NAME} ${APP_SOURCES} ${IMGUI_SOURCES})

# Include dizinleri
target_include_directories(${APP_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
    "${libsodium_SOURCE_DIR}/src/libsodium/include"
    "${glm_SOURCE_DIR}/"
)

# Link kütüphaneleri
target_link_libraries(${APP_NAME} PRIVATE SDL3::SDL3 sodium )

# Test kaynakları
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

foreach(TEST_FILE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/external"
        "${imgui_SOURCE_DIR}"
        "${imgui_SOURCE_DIR}/backends"
        "${libsodium_SOURCE_DIR}/src/libsodium/include"
        "${glm_SOURCE_DIR}/"
    )
    target_link_libraries(${TEST_NAME} PRIVATE SDL3::SDL3 sodium )
endforeach()

# SDL3 DLL kopyalama
function(copy_sdl_target TARGET_NAME)
    if(WIN32 OR APPLE OR UNIX)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        )
    endif()
endfunction()

copy_sdl_target(${APP_NAME})
foreach(TEST_FILE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    copy_sdl_target(${TEST_NAME})
endforeach()
