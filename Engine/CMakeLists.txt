set(ENGINE_ROOT_DIR "${XHUANG_ROOT_DIR}/Engine")
set(ENGINE_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source")
set(ENGINE_BIN_DIR "${XHUANG_ROOT_DIR}/bin")
### Vulkan
find_package(Vulkan REQUIRED)

add_subdirectory(3rdparty)
add_subdirectory(Source/Runtime)
add_subdirectory(Source/Editor)
