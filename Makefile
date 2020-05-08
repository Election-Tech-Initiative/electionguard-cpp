.PHONY: build clean demo-c demo-cpp test

.EXPORT_ALL_VARIABLES:
ELECTIONGUARD_DIR="$(realpath .)/build/ElectionGuard"

environment:
	wget -O cmake/CPM.cmake https://github.com/TheLartians/CPM.cmake/releases/latest/download/CPM.cmake

build: clean
	if [ ! -d "build" ]; then mkdir build; fi
	if [ ! -d "build/ElectionGuard" ]; then mkdir build/ElectionGuard; fi
	cmake -S . -B build/ElectionGuard
	cmake --build build/ElectionGuard

clean:
	if [ -d "build" ]; then rm -rf ./build/*; fi
	
demo-c: build
	if [ ! -d "build/apps" ]; then mkdir build/apps; fi
	if [ ! -d "build/apps/demo_in_c" ]; then mkdir build/apps/demo_in_c; fi
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -S apps/demo_in_c -B build/apps/demo_in_c
	cmake --build build/apps/demo_in_c --target DemoInC
	./build/apps/demo_in_c/DemoInC

demo-cpp: build
	if [ ! -d "build/apps" ]; then mkdir build/apps; fi
	if [ ! -d "build/apps/demo_in_cpp" ]; then mkdir build/apps/demo_in_cpp; fi
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -S apps/demo_in_cpp -B build/apps/demo_in_cpp
	cmake --build build/apps/demo_in_cpp --target DemoInCPP
	./build/apps/demo_in_cpp/DemoInCPP

test: clean
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -Htest -Bbuild/test
	cmake --build build/test
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target ElectionGuardTests
	./build/test/ElectionGuardTests
	