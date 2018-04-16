cmake_minimum_required(VERSION 3.5.1)
project (dfb_draw)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(DIRECTFB REQUIRED directfb)

set(FONT_PATH "\"${CMAKE_INSTALL_PREFIX}/shared/decker.ttf\"")
configure_file(config.h.in config.h)

set(SOURCESLIB window.cpp squareDraw.cpp freeDraw.cpp event.cpp canvas.cpp)
add_library(modular SHARED ${SOURCESLIB})
target_include_directories(modular PUBLIC ${CMAKE_BINARY_DIR} ${DIRECTFB_INCLUDE_DIRS})
target_link_libraries(modular PUBLIC ${DIRECTFB_LIBRARIES})

add_executable(dfb_draw dfb_draw.cpp)
target_link_libraries(dfb_draw modular)

# set(CMAKE_SKIP_BUILD_RPATH  FALSE)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
# set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set_target_properties(dfb_draw
                      PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")       

install(TARGETS modular dfb_draw
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

install(FILES decker.ttf DESTINATION shared)
