cmake_minimum_required(VERSION 2.8.5)
project(atarenium LANGUAGES C)

# ACE
add_subdirectory(ACE ace)

add_executable(falkon.elf main.c game.c menu.c gameover.c intro.c credits.c score.c robbo.c) # Compiler emits .elf file, we need hunk for Amiga

target_compile_options(falkon.elf PRIVATE -Wall -Wextra)

target_link_libraries(falkon.elf ace)

add_custom_command( # So call elf2hunk
	TARGET falkon.elf POST_BUILD
	COMMAND ${ELF2HUNK} falkon.elf falkon.exe
)

set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/res) # st�d bierzemy pliki do konwersji
set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/data) # tu maj� l�dowa�
file(MAKE_DIRECTORY ${DATA_DIR}) #za���my data

set(PLT_PATH ${DATA_DIR}/falkon.plt) #tak si� b�dzie nazywa� docelowa paleta
convertPalette(falkon.elf ${RES_DIR}/falkon.pal ${PLT_PATH}) # z res/falkon.pal do build/data/falkon.plt

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/title.png
	DESTINATIONS ${DATA_DIR}/title.bm
)

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/gej_ower.png
	DESTINATIONS ${DATA_DIR}/gej_ower.bm
)

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo1.png
	DESTINATIONS ${DATA_DIR}/tlo1.bm
)

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo2.png
	DESTINATIONS ${DATA_DIR}/tlo2.bm
)

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo3.png
	DESTINATIONS ${DATA_DIR}/tlo3.bm
)
convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo4.png
	DESTINATIONS ${DATA_DIR}/tlo4.bm
)
convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo5.png
	DESTINATIONS ${DATA_DIR}/tlo5.bm
)
convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/tlo6.png
	DESTINATIONS ${DATA_DIR}/tlo6.bm
)

convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES ${RES_DIR}/HUD.png
	DESTINATIONS ${DATA_DIR}/HUD.bm
)



convertBitmaps(
	TARGET falkon.elf PALETTE ${PLT_PATH}
	MASK_COLOR "#000000"
	INTERLEAVED SOURCES ${RES_DIR}/tileset.png
	DESTINATIONS ${DATA_DIR}/tileset.bm
	MASKS ${DATA_DIR}/tileset_mask.bm
)


file(
    COPY ${RES_DIR}/uni54.fnt
    DESTINATION ${DATA_DIR}
)
