project(TestApp)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(TestApp_SRCS TestApp.cpp main.cpp)
set(TestApp_MOC_HEADERS TestApp.h)
set(TestApp_FORMS TestApp.ui)

QT4_WRAP_UI(TestApp_SRCS ${TestApp_FORMS})
QT4_WRAP_CPP(TestApp_MOC_OUT ${TestApp_MOC_HEADERS})
add_executable(TestApp ${TestApp_SRCS} ${TestApp_MOC_OUT})
target_link_libraries(TestApp ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} VM167 usb-1.0)
