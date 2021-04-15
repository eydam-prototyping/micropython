add_library(ep_modules_logging INTERFACE)

target_sources(ep_modules_logging INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mp_module_logger.c
    ${CMAKE_CURRENT_LIST_DIR}/mp_class_logger.c
    ${CMAKE_CURRENT_LIST_DIR}/mp_class_formatter.c
    ${CMAKE_CURRENT_LIST_DIR}/mp_class_handler.c
    ${CMAKE_CURRENT_LIST_DIR}/c_funcs_logger.cpp
)

target_include_directories(ep_modules_logging INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usermod INTERFACE ep_modules_logging)