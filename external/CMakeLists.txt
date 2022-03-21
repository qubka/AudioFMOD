set(BUILD_SHARED_LIBS OFF)
if(APPLE)
    set(BUILD_FRAMEWORK ON)
endif()


# GLFW
set(GLFW_LIB_NAME "glfw")
set(GLFW_INC_PATH ${GLFW_LIB_NAME}/include)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_VULKAN_STATIC  OFF CACHE BOOL "" FORCE)    # OFF by default
#set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)    # OFF by default
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_LIB_NAME})

# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH ${GLM_LIB_NAME})

add_library(${GLM_LIB_NAME} INTERFACE)

target_include_directories(${GLM_LIB_NAME}
        INTERFACE
        ${GLM_INC_PATH}
        )

# glad
set(GLAD_LIB_NAME "glad")
set(GLAD_SRC_PATH "${GLAD_LIB_NAME}/src")
set(GLAD_INC_PATH "${GLAD_LIB_NAME}/include")

add_library( ${GLAD_LIB_NAME}
        STATIC
        "${GLAD_SRC_PATH}/glad.c"
        )

target_include_directories(${GLAD_LIB_NAME}
        PUBLIC
        "${GLAD_INC_PATH}"
        )

# stb
set(STB_LIB_NAME "stb")
set(STB_INC_PATH ${STB_LIB_NAME}/)

add_library(${STB_LIB_NAME} INTERFACE)

target_include_directories(${STB_LIB_NAME}
        INTERFACE
        ${STB_INC_PATH}
        )

# entt
set(ENTT_LIB_NAME "entt")
set(ENTT_INC_PATH ${ENTT_LIB_NAME}/src)

add_library(${ENTT_LIB_NAME} INTERFACE)

target_include_directories(${ENTT_LIB_NAME}
        INTERFACE
        ${ENTT_INC_PATH}
        )


# magic_enum
set(MAGIC_ENUM_LIB_NAME "magic_enum")
set(MAGIC_ENUM_INC_PATH ${MAGIC_ENUM_LIB_NAME}/include)

add_library(${MAGIC_ENUM_LIB_NAME} INTERFACE)

target_include_directories(${MAGIC_ENUM_LIB_NAME}
        INTERFACE
        ${MAGIC_ENUM_INC_PATH}
        )


# freetype
set(FREETYPE_LIB_NAME "freetype")
set(FREETYPE_INC_PATH ${FREETYPE_LIB_NAME}/include)

add_subdirectory(${FREETYPE_LIB_NAME})


# fmod
set(FMOD_LIB_NAME "fmodstudioapi")
set(FMOD_INC_PATH ${FMOD_LIB_NAME}/core/inc)
set(FMOD_STUDIO_INC_PATH ${FMOD_LIB_NAME}/studio/inc)

target_include_directories(${FMOD_LIB_NAME}
        PUBLIC
        ${FMOD_INC_PATH}
        ${FMOD_STUDIO_INC_PATH}
        )

set(FMOD_LIBRARY ${FMOD_LIB_NAME}/core/lib/x86_64)
set(FMOD_STUDIO_LIBRARY ${FMOD_LIB_NAME}/studio/lib/x86_64)