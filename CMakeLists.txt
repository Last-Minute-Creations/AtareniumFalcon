cmake_minimum_required(VERSION 2.8.5)
project(atarenium LANGUAGES C)

# ACE
add_subdirectory(ACE ace)

add_executable(falkon.elf main.c) # Compiler emits .elf file, we need hunk for Amiga

target_compile_options(falkon.elf PRIVATE -Wall -Wextra)

target_link_libraries(falkon.elf ace)

add_custom_command( # So call elf2hunk
    TARGET falkon.elf POST_BUILD
    COMMAND ${ELF2HUNK} falkon.elf falkon.exe)

