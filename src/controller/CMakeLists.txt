add_library(Controller controller.hpp controller.cpp)

target_link_libraries(Controller Logic)

add_library(CreaturesAI creaturesAI.hpp creaturesAI.cpp)

target_link_libraries(CreaturesAI Controller)