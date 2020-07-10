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

    set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/res) # st¥d bierzemy pliki do konwersji
    set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/data) # tu maj¥ l¥dowa†
    file(MAKE_DIRECTORY ${DATA_DIR}) #zaˆ¢¾my data
    
    set(PLT_PATH ${DATA_DIR}/falkon.plt) #tak si© b©dzie nazywa† docelowa paleta
    convertPalette(falkon.elf ${RES_DIR}/falkon.pal ${PLT_PATH}) # z res/falkon.pal do build/data/falkon.plt