file(GLOB_RECURSE MIRA_GAME_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_executable(MiraGame
    ${MIRA_GAME_SOURCES}
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