.PHONY: build build-debug build-debug-all build-release build-android build-ios clean format memcheck sanitize sanitize-asan sanitize-tsan test

TARGET?=Release
NDK_PATH?=/Users/$$USER/Library/Android/sdk/ndk/21.3.6528147

.EXPORT_ALL_VARIABLES:
ELECTIONGUARD_BUILD_DIR=$(realpath .)/build
ELECTIONGUARD_BUILD_APPS_DIR=$(ELECTIONGUARD_BUILD_DIR)/apps
ELECTIONGUARD_BUILD_BINDING_DIR=$(ELECTIONGUARD_BUILD_DIR)/bindings
ELECTIONGUARD_BUILD_LIBS_DIR=$(ELECTIONGUARD_BUILD_DIR)/libs
CPM_SOURCE_CACHE=$(realpath .)/.cache/CPM

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
	brew install cmake
	brew install cppcheck
	brew install llvm
	ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
endif
ifeq ($(OPERATING_SYSTEM),Linux)
	sudo apt install build-essential
	sudo apt install llvm
	sudo apt install cmake
	sudo apt install lcov
	sudo apt install cppcheck
	sudo apt install clang-tidy
	sudo apt install valgrind
endif
ifeq ($(OPERATING_SYSTEM),Windows)
    
endif

ifeq ($(TARGET),Release)
build: build-release
else
build: build-debug
endif

build-debug:
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64

build-release: clean
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DUSE_STATIC_ANALYSIS=ON
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 \
		-DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DUSE_STATIC_ANALYSIS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64

build-android:
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/arm64-v8a \
		-DCMAKE_BUILD_TYPE=$(TARGET) -DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/arm64-v8a
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/armeabi-v7a \
		-DCMAKE_BUILD_TYPE=$(TARGET) -DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_SYSTEM_VERSION=26 \
		-DCMAKE_ANDROID_ARM_NEON=ON
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/armeabi-v7a
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86 \
		-DCMAKE_BUILD_TYPE=$(TARGET) -DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=x86 -DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86_64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) -DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=x86_64 -DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86_64

build-ios:
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios -GXcode \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=iOS \
		"-DCMAKE_OSX_ARCHITECTURES=arm64;arm64e;x86_64" \
		-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
		-DCMAKE_INSTALL_PREFIX=$(ELECTIONGUARD_BUILD_LIBS_DIR)/ios \
		-DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO \
		-DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios --config $(TARGET) --target install

build-netstandard: build-android build-ios
	msbuild ./bindings/netstandard/ElectionGuard/ElectionGuard.sln /t:Build /p:Configuration=$(TARGET)
	cp ./bindings/netstandard/ElectionGuard/ElectionGuard.NuGet/bin/$(TARGET)/* $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/

build-all: build build-android build-ios

clean:
	if [ -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then rm -rf $(ELECTIONGUARD_BUILD_DIR)/*; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_APPS_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_APPS_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android/arm64-v8a" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/arm64-v8a; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android/armeabi-v7a" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/armeabi-v7a; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86_64" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/x86_64; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/ios" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios; fi

format: build
	cd $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 && $(MAKE) format

memcheck:
ifeq ($(OPERATING_SYSTEM),Windows)
	echo "Static analysis is only supported on Linux"
else
	cd $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 && $(MAKE) memcheck-ElectionGuardTests
endif

rebuild: clean build

sanitize: sanitize-asan sanitize-tsan

sanitize-asan:
ifeq ($(OPERATING_SYSTEM),Windows)
	echo "Address sanitizer is only supported on Linux & Mac"
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON -DUSE_STATIC_ANALYSIS=ON -DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="address;undefined" \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardCTests
endif

sanitize-tsan:
ifeq ($(OPERATING_SYSTEM),Windows)
	echo "Thread sanitizer is only supported on Linux & Mac"
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON -DUSE_STATIC_ANALYSIS=ON -DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="thread" \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardCTests
endif

test:
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DOPTION_ENABLE_TESTS=ON
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64
	PATH=$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/src:$$PATH; $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardTests
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 \
		-DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON -DUSE_STATIC_ANALYSIS=ON -DUSE_DYNAMIC_ANALYSIS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/test/ElectionGuardCTests
endif
