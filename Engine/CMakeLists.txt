set(ENGINE_ROOT_DIR "${XHUANG_ROOT_DIR}/Engine")
set(ENGINE_SOURCE_DIR "${ENGINE_ROOT_DIR}/Source")

### Vulkan
find_package(Vulkan REQUIRED)

add_subdirectory(Source/Runtime)
add_subdirectory(Source/Editor)
