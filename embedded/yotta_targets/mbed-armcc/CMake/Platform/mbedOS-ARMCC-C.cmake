# Copyright (C) 2014-2015 ARM Limited. All rights reserved.
#message("mbedOS-ARMCC-C.cmake included")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(ARMCC_ENV " ")
else()
    set(ARMCC_ENV "LC_ALL=en_US.utf8 LANG=en_US.utf8 ")
endif()

set(EXPLICIT_INCLUDES "")
if((CMAKE_VERSION VERSION_GREATER "3.4.0") OR (CMAKE_VERSION VERSION_EQUAL "3.4.0"))
    # from CMake 3.4 <INCLUDES> are separate to <FLAGS> in the
    # CMAKE_<LANG>_COMPILE_OBJECT, CMAKE_<LANG>_CREATE_ASSEMBLY_SOURCE, and
    # CMAKE_<LANG>_CREATE_PREPROCESSED_SOURCE commands
    set(EXPLICIT_INCLUDES "<INCLUDES> ")
endif()

# Override the link rules:
set(CMAKE_C_CREATE_SHARED_LIBRARY "echo 'shared libraries not supported' && 1")
set(CMAKE_C_CREATE_SHARED_MODULE  "echo 'shared modules not supported' && 1")
set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> -cr<LINK_FLAGS> <TARGET> <OBJECTS>")
set(CMAKE_C_COMPILE_OBJECT        "${ARMCC_ENV}<CMAKE_C_COMPILER> <DEFINES> --gnu -c ${EXPLICIT_INCLUDES}<FLAGS> -o <OBJECT> <SOURCE>")
set(CMAKE_C_LINK_EXECUTABLE       "<CMAKE_LINKER> -o <TARGET> <OBJECTS> <LINK_LIBRARIES> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS>")
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message("WARNING: preprocessing of .S files is not supported by ARMCC on windows.")
endif()
set(CMAKE_ASM_COMPILE_OBJECT      "${ARMCC_ENV} <CMAKE_C_COMPILER> <DEFINES> --gnu -c <FLAGS> -o <OBJECT> <SOURCE>")

set(CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Ospace -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT        "-Ospace -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-Ospace -g -DNDEBUG")
set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(CMAKE_ASM_FLAGS_DEBUG_INIT          "-O0 -g")
set(CMAKE_ASM_FLAGS_MINSIZEREL_INIT     "-O3 -Ospace -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELEASE_INIT        "-O3 -Ospace -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO_INIT "-O3 -Ospace -g -DNDEBUG")
set(CMAKE_INCLUDE_SYSTEM_FLAG_ASM  "-isystem ")

set(CMAKE_C_OUTPUT_EXTENSION ".o")
set(CMAKE_C_RESPONSE_FILE_LINK_FLAG "--via=")

