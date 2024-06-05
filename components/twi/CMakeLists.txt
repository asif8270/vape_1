get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_DIR_NAME}.c)
target_include_directories(app PRIVATE .)