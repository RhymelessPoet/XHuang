set(TARGET_NAME XHuangRuntime)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

message("-- ${TARGET_NAME} Head Files : \n ${HEADER_FILES}")
message("-- ${TARGET_NAME} Source Files : \n ${SOURCE_FILES}")

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC $<BUILD_INTERFACE:${ENGINE_SOURCE_DIR}/Runtime>
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC $<BUILD_INTERFACE:${ENGINE_SOURCE_DIR}>
)

message("-- ${ENGINE_SOURCE_DIR}/Runtime")

### glfw
target_link_libraries(${TARGET_NAME} PUBLIC glfw)

### Vulkan
if (Vulkan_FOUND)
  target_include_directories(${TARGET_NAME} PUBLIC Vulkan_INCLUDE_DIRS)
  target_link_libraries(${TARGET_NAME} PUBLIC Vulkan::Vulkan)
endif()