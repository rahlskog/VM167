include_directories(/usr/include/libusb-1.0)
add_library(VM167 SHARED VM167.c)

install(TARGETS VM167 LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)