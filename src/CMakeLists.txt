add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(controller)

add_executable(main main.cpp)
target_link_libraries(main View)

add_executable(speedTest speedTest.cpp)
target_link_libraries(speedTest View)