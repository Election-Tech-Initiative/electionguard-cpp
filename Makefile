.PHONY: build clean demo-c demo-cpp test

BUILD_DEBUG?=false

.EXPORT_ALL_VARIABLES:
ELECTIONGUARD_DIR="$(realpath .)/build/electionguard-core"

# Detect operating system
ifeq ($(OS),Windows_NT)
    OPERATING_SYSTEM := Windows
else
    OPERATING_SYSTEM := $(shell uname 2>/dev/null || echo Unknown)
endif

all: environment build

environment:
	wget -O cmake/CPM.cmake https://github.com/TheLartians/CPM.cmake/releases/latest/download/CPM.cmake
ifeq ($(OPERATING_SYSTEM),Darwin)
    
endif
ifeq ($(OPERATING_SYSTEM),Linux)
    
endif
ifeq ($(OPERATING_SYSTEM),Windows)
    
endif

ifeq ($(BUILD_DEBUG),true)
build: build-debug
else
build: build-release
endif

build-debug: clean
	if [ ! -d "build" ]; then mkdir build; fi
	if [ ! -d "build/electionguard-core" ]; then mkdir build/electionguard-core; fi
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B build/electionguard-core -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug
else
	cmake -S . -B build/electionguard-core -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON
endif
	cmake --build build/electionguard-core

build-release: clean
	if [ ! -d "build" ]; then mkdir build; fi
	if [ ! -d "build/electionguard-core" ]; then mkdir build/electionguard-core; fi
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B build/electionguard-core -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
else
	cmake -S . -B build/electionguard-core -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
endif
	cmake --build build/electionguard-core

clean:
	if [ -d "build" ]; then rm -rf ./build/*; fi
	
demo-c: build
	if [ ! -d "build/apps" ]; then mkdir build/apps; fi
	if [ ! -d "build/apps/demo_in_c" ]; then mkdir build/apps/demo_in_c; fi
ifeq ($(OPERATING_SYSTEM),Windows)
	CMAKE_PREFIX_PATH="./build/electionguard-core" cmake -S apps/demo_in_c -B build/apps/demo_in_c -G "MSYS Makefiles"
	cmake --build build/apps/demo_in_c --target DemoInC
	PATH=$(PWD)/build/electionguard-core/src:$$PATH; ./build/apps/demo_in_c/DemoInC
else
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -S apps/demo_in_c -B build/apps/demo_in_c
	cmake --build build/apps/demo_in_c --target DemoInC
	./build/apps/demo_in_c/DemoInC
endif

demo-cpp: build
	if [ ! -d "build/apps" ]; then mkdir build/apps; fi
	if [ ! -d "build/apps/demo_in_cpp" ]; then mkdir build/apps/demo_in_cpp; fi
ifeq ($(OPERATING_SYSTEM),Windows)
	CMAKE_PREFIX_PATH="./build/electionguard-core" cmake -S apps/demo_in_cpp -B build/apps/demo_in_cpp -G "MSYS Makefiles"
	cmake --build build/apps/demo_in_cpp --target DemoInCPP
	PATH=$(PWD)/build/electionguard-core/src:$$PATH; ./build/apps/demo_in_cpp/DemoInCPP
else
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -S apps/demo_in_cpp -B build/apps/demo_in_cpp
	cmake --build build/apps/demo_in_cpp --target DemoInCPP
	./build/apps/demo_in_cpp/DemoInCPP
endif

test: clean
ifeq ($(OPERATING_SYSTEM),Windows)
	CMAKE_PREFIX_PATH="./build/test" cmake -Htest -Bbuild/test -G "MSYS Makefiles"
	cmake --build build/test
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target ElectionGuardTests
	PATH=$(PWD)/build/test/_deps/electionguard-build/src:$(PWD)/src/electionguard:$$PATH; ./build/test/ElectionGuardTests
else
	ElectionGuard_DIR=$(ELECTIONGUARD_DIR) cmake -Htest -Bbuild/test
	cmake --build build/test
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target ElectionGuardTests
	./build/test/ElectionGuardTests
endif
	