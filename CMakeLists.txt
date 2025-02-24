cmake_minimum_required(VERSION 3.16)

#add_compile_options(--coverage)
#add_link_options(--coverage)

add_compile_options(-fsanitize=thread)
add_link_options(-fsanitize=thread)

project(oop_lab_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

add_subdirectory(tests)
add_subdirectory(src)
