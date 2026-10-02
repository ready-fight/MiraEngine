add_executable(MiraGame
    Source/main.cpp
    
    
    
)

add_custom_command(
    TARGET MiraGame
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:MiraGame>/Assets"
)

target_include_directories(MiraGame
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(MiraGame
    PRIVATE
        MiraEngine
)