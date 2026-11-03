cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target coursework-format
./build/coursework