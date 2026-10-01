add_executable(MiraGame
    Source/main.cpp
    
    
    
)

target_include_directories(MiraGame
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(MiraGame
    PRIVATE
        MiraEngine
)