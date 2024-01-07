set(TARGET_NAME XHuangEditor)
set(SRC_LIST main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_LIST})
add_executable(${TARGET_NAME} ${SRC_LIST})
target_link_libraries(${TARGET_NAME} PUBLIC XHuangRuntime)
set_target_properties(${TARGET_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${ENGINE_BIN_DIR}/Win"
    VS_DEBUGGER_WORKING_DIRECTORY "${XHUANG_ROOT_DIR}")