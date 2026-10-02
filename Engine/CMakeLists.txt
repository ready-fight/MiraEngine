find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb_image.h")

add_library(MiraEngine STATIC
    Source/Core/Application.cpp
    Source/Core/Window.cpp

    Source/Graphics/Renderer.cpp
    Source/Graphics/Shader.cpp
    Source/Graphics/Camera.cpp
    Source/Graphics/Mesh.cpp
    Source/Graphics/Model.cpp

    Source/Scene/Scene.cpp
    Source/Scene/GameObject.cpp
    Source/Scene/Transform.cpp
    Source/Core/Time.cpp
    Source/Collision/SphereCollider.cpp
    Source/Animation/Animation.cpp
    Source/Animation/Animator.cpp
)

target_include_directories(MiraEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    PRIVATE
        ${STB_INCLUDE_DIRS}
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