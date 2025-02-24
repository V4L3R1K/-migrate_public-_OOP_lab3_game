find_package(Catch2 3 REQUIRED)

add_executable(ModelTest modelTest.cpp)
target_link_libraries(ModelTest Logic Catch2::Catch2WithMain)

add_executable(ControllerTest controllerTest.cpp)
target_link_libraries(ControllerTest Controller Catch2::Catch2WithMain)