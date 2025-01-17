# https://github.com/Neargye/magic_enum/releases/download/v0.8.2/magic_enum.hpp

cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(magic_enum

    # URL https://github.com/Neargye/magic_enum/releases/download/v0.8.2/magic_enum.hpp
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG "v0.8.2"
    OVERRIDE_FIND_PACKAGE
)

message(STATUS "Downaloding magic_enum......")

FetchContent_MakeAvailable(magic_enum)