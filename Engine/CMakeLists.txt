find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb_image.h")
find_path(MINIAUDIO_INCLUDE_DIR miniaudio.h)

file(GLOB_RECURSE MIRA_ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_library(MiraEngine STATIC
    ${MIRA_ENGINE_SOURCES}
)

target_include_directories(MiraEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    PRIVATE
        ${STB_INCLUDE_DIRS}
        ${MINIAUDIO_INCLUDE_DIR}
)

target_link_libraries(MiraEngine
    PUBLIC
        OpenGL::GL
        glfw
        GLEW::GLEW
        glm::glm
        imgui::imgui
        assimp::assimp
)