# install C shared lib
# set(SlopeCraft_SCL_C_include_files
#
# ${CMAKE_SOURCE_DIR}/utilities/SC_GlobalEnums.h
# SlopeCraftL.h
# SCL_C/SlopeCraftL_global.h # a copy from ../SlopeCraft_global.h, an extra line `#define SCL_CAPI` is added in this file
# )

# install Cpp sharerd lib
set(SlopeCraft_SCL_Cpp_include_files

    ${CMAKE_SOURCE_DIR}/utilities/SC_GlobalEnums.h
    SlopeCraftL.h
    SlopeCraftL_global.h
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # install for applications
    install(TARGETS SlopeCraftL
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})

    # install to lib dirs
    install(TARGETS SlopeCraftL
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/lib

        # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/lib
    )

    # install(TARGETS SlopeCraftL_C
    # RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/bin
    # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/lib

    # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/lib
    # )
    install(FILES ${SlopeCraft_SCL_Cpp_include_files}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/include)

    # install(FILES ${SlopeCraft_SCL_C_include_files}
    # DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/include)
    return()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Install SlopeCraft for linux")

    # install for applications
    install(TARGETS SlopeCraftL
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

    install(FILES ${SlopeCraft_SCL_Cpp_include_files}
        DESTINATION include)
    return()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # install for applications
    install(TARGETS SlopeCraftL
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})

    # install to lib dirs
    install(TARGETS SlopeCraftL
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/lib

        # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/lib
    )

    # install(TARGETS SlopeCraftL_C
    # RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/bin
    # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/lib
    #
    # LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/lib
    # )
    install(FILES ${SlopeCraft_SCL_Cpp_include_files}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/Cpp/include)

    # install(FILES ${SlopeCraft_SCL_C_include_files}
    # DESTINATION ${CMAKE_INSTALL_PREFIX}/../install_SlopeCraftL/C/include)
    return()
endif()

# add_subdirectory(SCL_C)
