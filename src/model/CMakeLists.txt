add_library(Matrix INTERFACE matrix.hpp)

file(GLOB HEADERS "./*.hpp")
list(FILTER HEADERS EXCLUDE REGEX "./*matrix.hpp")
add_library(Logic STATIC ${HEADERS} itemUseLogic.cpp)

target_link_libraries(Logic Matrix)