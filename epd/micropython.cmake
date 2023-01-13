# Create an INTERFACE library for our C module.
add_library(usermod_epd INTERFACE)

include(${CMAKE_CURRENT_LIST_DIR}/depend.cmake)

file(GLOB MOD_SRC ${CMAKE_CURRENT_LIST_DIR}/*.c)

file(GLOB EPD47_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/LilyGo-EPD47)
set(LIBEPD47_SRC ${EPD47_DIR}/src/ed047tc1.c
                 ${EPD47_DIR}/src/epd_driver.c
                 ${EPD47_DIR}/src/i2s_data_bus.c
                 ${EPD47_DIR}/src/rmt_pulse.c
)
# file(GLOB ZLIB_SRC ${EPD47_DIR}/src/zlib/*.c)
# file(GLOB LIBJPEG_SRC ${EPD47_DIR}/src/libjpeg/*.c)

# Add our source files to the lib
target_sources(usermod_epd INTERFACE
    ${MOD_SRC}
    ${LIBEPD47_SRC}
    # ${ZLIB_SRC}
    # ${LIBJPEG_SRC}
)

# Add the current directory as an include directory.
target_include_directories(usermod_epd INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${EPD47_DIR}/src/
    # ${EPD47_DIR}/src/zlib/
    # ${EPD47_DIR}/src/libjpeg/
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_epd)
