.PHONY: all build build-debug build-msvc build-debug-all build-release build-android build-ios clean environment format memcheck sanitize sanitize-asan sanitize-tsan test

.EXPORT_ALL_VARIABLES:
ELECTIONGUARD_BUILD_DIR=$(realpath .)/build
ELECTIONGUARD_BUILD_DIR_WIN=$(subst \c\,C:\,$(subst /,\,$(ELECTIONGUARD_BUILD_DIR)))
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

# Debug or Release (capitalized)
TARGET?=Release

ifeq ($(OPERATING_SYSTEM),Darwin)
NDK_PATH?=/Users/$$USER/Library/Android/sdk/ndk/21.3.6528147
endif
ifeq ($(OPERATING_SYSTEM),Linux)
NDK_PATH?=/usr/local/lib/android/sdk/ndk/21.3.6528147
endif
ifeq ($(OPERATING_SYSTEM),Windows)
NDK_PATH?=C:\Android\android-sdk\ndk-bundle
endif

all: environment build

environment:
ifeq ($(OPERATING_SYSTEM),Darwin)
	@echo 🍎 MACOS INSTALL
	brew install wget
	brew install cmake
	brew install cppcheck
	brew install include-what-you-use
	brew install llvm
	ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
endif
ifeq ($(OPERATING_SYSTEM),Linux)
	@echo 🐧 LINUX INSTALL
	sudo apt install -y build-essential
	sudo apt install -y iwyu
	sudo apt install -y llvm
	sudo apt install -y clang-9
	sudo apt install -y cmake
	sudo apt install -y lcov
	sudo apt install -y cppcheck
	sudo apt install -y clang-format
	sudo apt install -y clang-tidy
	sudo apt install -y valgrind
endif
ifeq ($(OPERATING_SYSTEM),Windows)
	@echo 🏁 WINDOWS INSTALL
	choco install wget
endif
	wget -O cmake/CPM.cmake https://github.com/TheLartians/CPM.cmake/releases/latest/download/CPM.cmake

ifeq ($(TARGET),Release)
build: build-release
else
build: build-debug
endif

build-debug:
	@echo 🐛 BUILD DEBUG
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug

build-msvc:
	@echo 🖥️ BUILD MSVC
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/Win32 -G "Visual Studio 16 2019" -A Win32 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 16 2019" -A x64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/Win32 --config $(TARGET)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 --config $(TARGET)
else
	echo "MSVC builds are only supported on Windows"
endif

build-release: clean
	@echo 🧱 BUILD RELEASE
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Release -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Release \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Release

build-android:
	@echo 🤖 BUILD ANDROID
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/arm64-v8a \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
		-DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/arm64-v8a
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/armeabi-v7a \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a \
		-DCMAKE_SYSTEM_VERSION=26 \
		-DCMAKE_ANDROID_ARM_NEON=ON
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/armeabi-v7a
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/x86 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=x86 \
		-DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/x86
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/x86_64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_ANDROID_NDK=$(NDK_PATH) \
		-DCMAKE_ANDROID_ARCH_ABI=x86_64 \
		-DCMAKE_SYSTEM_VERSION=26
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/android/$(TARGET)/x86_64

build-ios:
	@echo 📱 BUILD IOS
ifeq ($(OPERATING_SYSTEM),Darwin)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios/$(TARGET) \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE) \
		-DCMAKE_SYSTEM_NAME=iOS \
		"-DCMAKE_OSX_ARCHITECTURES=arm64;arm64e;x86_64" \
		-DCMAKE_OSX_DEPLOYMENT_TARGET=12.4 \
		-DCMAKE_INSTALL_PREFIX=$(ELECTIONGUARD_BUILD_LIBS_DIR)/ios/$(TARGET) \
		-DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO \
		-DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios/$(TARGET) --config $(TARGET) --target install
else
	echo "iOS builds are only supported on MacOS"
endif

build-netframework: build-msvc
	@echo 🖥️ BUILD NET FRAMEWORK
ifeq ($(OPERATING_SYSTEM),Windows)
	pwsh -Command "msbuild ./bindings/netframework/ElectionGuard.NetFramework/ElectionGuard.NetFramework.x64.sln /t:Build /p:Configuration=$(TARGET); $$null"
else
	echo "Net Framework builds are only supported on Windows"
endif

build-netstandard: build-android build-ios
	@echo 🖥️ BUILD NETSTANDARD
ifeq ($(OPERATING_SYSTEM),Darwin)
	msbuild ./bindings/netstandard/ElectionGuard/ElectionGuard.sln /t:Build /p:Configuration=$(TARGET)
	cp ./bindings/netstandard/ElectionGuard/ElectionGuard.NuGet/bin/$(TARGET)/* $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/$(TARGET)/
else
	echo "NetStandard builds are only supported on MacOS"
endif

build-all: build build-netstandard

clean:
	@echo 🗑️ Cleaning Output Directory
ifeq ($(OPERATING_SYSTEM),Windows)
	pwsh -Command "rm -R -Fo $(ELECTIONGUARD_BUILD_DIR_WIN); $$null"
else
	if [ -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then rm -rf $(ELECTIONGUARD_BUILD_DIR)/*; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_APPS_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_APPS_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/Debug" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/Debug; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/Release" ]; then mkdir $(ELECTIONGUARD_BUILD_BINDING_DIR)/netstandard/Release; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/ios" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios; fi
endif

format: build
	cd $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 && $(MAKE) format

memcheck:
	@echo 🧼 RUN STATIC ANALYSIS
ifeq ($(OPERATING_SYSTEM),Windows)
	@echo "Static analysis is only supported on Linux"
else
	cd $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64 && $(MAKE) memcheck-ElectionGuardTests
endif

rebuild: clean build

sanitize: sanitize-asan sanitize-tsan

sanitize-asan:
	@echo 🧼 SANITIZE ADDRESS AND UNDEFINED
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 16 2019" -A x64 \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DUSE_SANITIZER="address" \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/Debug/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/Debug/ElectionGuardCTests
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DEXPORT_INTERNALS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DUSE_STATIC_ANALYSIS=ON \
		-DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="address" \
		-DCLANG_TIDY=ON \
		-DCPPCHECK=ON \
		-DIWYU=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardCTests
endif

sanitize-tsan:
	@echo 🧼 SANITIZE THREADS
ifeq ($(OPERATING_SYSTEM),Windows)
	echo "Thread sanitizer is only supported on Linux & Mac"
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DUSE_STATIC_ANALYSIS=ON \
		-DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="thread" \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardCTests
endif

bench:
	@echo 🧪 BENCHMARK
ifeq ($(OPERATING_SYSTEM),Windows)
	echo "Benchmark is only supported on Linux & Mac"
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Release \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardBenchmark

test:
	@echo 🧪 TEST
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardCTests

test-msvc:
	@echo 🧪 TEST
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 16 2019" -A x64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 --config $(TARGET)
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/$(TARGET)/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/$(TARGET)/ElectionGuardCTests
endif

coverage:
	@echo ✅ CHECK COVERAGE
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DUSE_STATIC_ANALYSIS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardCTests
