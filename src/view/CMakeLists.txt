find_package(Curses REQUIRED)

add_library(View view.hpp view.cpp)

target_link_libraries(View PRIVATE ${CURSES_LIBRARIES} Controller CreaturesAI)
target_include_directories(View PRIVATE ${CURSES_INCLUDE_DIRS})