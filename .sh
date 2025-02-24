# мне было лень чето выдумывать, так что просто запускаем этот скрипт и по надобности (рас)комментируем тут нужные строки

clear
cmake . -B build
cd build
make -j

export TSAN_OPTIONS="log_path=tsan_report.log verbosity=1"

#valgrind --leak-check=full --show-leak-kinds=all ./src/main
#valgrind ./src/main
./src/main

#./src/speedTest

cd tests
#valgrind --leak-check=full ./ModelTest
#valgrind --leak-check=full ./ControllerTest
#cd ../..

#mkdir coverage
#gcovr -r . -e CMakeFiles --exclude-unreachable-branches --exclude-throw-branches --html-details -o ./coverage/coverage.html