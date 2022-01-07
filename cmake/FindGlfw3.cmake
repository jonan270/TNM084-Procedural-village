# Source for how to set up CMAKE with static libraries and clion :
# https://www.youtube.com/watch?v=CqjokCekMM8

set(FIND_GLFW3_PATHS
        C:/glfw3)

find_path(GLFW3_INCLUDE_DIR glfw3.h
        PATH_SUFFIXES include
        PATHS ${FIND_GLFW3_PATHS})

find_library(GLFW3_LIBRARY
        NAMES glfw3 glfw3dll
        PATH_SUFFIXES lib
        PATHS ${FIND_GLFW3_PATHS})