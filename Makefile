.PHONY: all build build-msvc build-android build-ios clean clean-netstandard environment format memcheck sanitize sanitize-asan sanitize-tsan test test-msvc test-netstandard

.EXPORT_ALL_VARIABLES:
ELECTIONGUARD_BINDING_DIR=$(realpath .)/bindings
ELECTIONGUARD_BINDING_LIB_DIR=$(ELECTIONGUARD_BINDING_DIR)/netstandard/ElectionGuard/ElectionGuard.Encryption
ELECTIONGUARD_BINDING_BENCH_DIR=$(ELECTIONGUARD_BINDING_DIR)/netstandard/ElectionGuard/ElectionGuard.Encryption.Bench
ELECTIONGUARD_BINDING_TEST_DIR=$(ELECTIONGUARD_BINDING_DIR)/netstandard/ElectionGuard/ElectionGuard.Encryption.Tests
ELECTIONGUARD_BUILD_DIR=$(subst \,/,$(realpath .))/build
ELECTIONGUARD_BUILD_DIR_WIN=$(subst \c\,C:\,$(subst /,\,$(ELECTIONGUARD_BUILD_DIR)))
ELECTIONGUARD_BUILD_APPS_DIR=$(ELECTIONGUARD_BUILD_DIR)/apps
ELECTIONGUARD_BUILD_BINDING_DIR=$(ELECTIONGUARD_BUILD_DIR)/bindings
ELECTIONGUARD_BUILD_LIBS_DIR=$(ELECTIONGUARD_BUILD_DIR)/libs
CPM_SOURCE_CACHE=$(subst \,/,$(realpath .))/.cache/CPM

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
	choco install unzip
endif
	wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.35.0/CPM.cmake
	wget -O sample-data-container.zip https://github.com/microsoft/electionguard/releases/download/v0.95.0/sample-data.zip
	unzip -o sample-data-container.zip


build:
	@echo 🧱 BUILD $(TARGET)
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)

build-msvc:
	@echo 🖥️ BUILD MSVC
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/Win32 -G "Visual Studio 17 2022" -A Win32 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 17 2022" -A x64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/Win32 --config $(TARGET)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 --config $(TARGET)
else
	echo "MSVC builds are only supported on Windows"
endif

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

build-netstandard:
ifeq ($(OPERATING_SYSTEM),Windows)
	make build-msvc
else
	make build
endif

	@echo 🖥️ BUILD NETSTANDARD
	cd ./bindings/netstandard/ElectionGuard && dotnet restore
	dotnet build --configuration $(TARGET) ./bindings/netstandard/ElectionGuard/ElectionGuard.sln /property:Platform=x86
	dotnet build --configuration $(TARGET) ./bindings/netstandard/ElectionGuard/ElectionGuard.sln /property:Platform=x64

clean:
	@echo 🗑️ Cleaning Output Directory
ifeq ($(OPERATING_SYSTEM),Windows)
	pwsh -Command "rm -R -Fo $(ELECTIONGUARD_BUILD_DIR_WIN); $$null"
else
	if [ -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then rm -rf $(ELECTIONGUARD_BUILD_DIR)/*; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_DIR); fi

	if [ -d "$(ELECTIONGUARD_BINDING_LIB_DIR)/bin" ]; then rm -rf $(ELECTIONGUARD_BINDING_LIB_DIR)/bin/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_LIB_DIR)/bin" ]; then mkdir $(ELECTIONGUARD_BINDING_LIB_DIR)/bin; fi
	if [ -d "$(ELECTIONGUARD_BINDING_LIB_DIR)/obj" ]; then rm -rf $(ELECTIONGUARD_BINDING_LIB_DIR)/obj/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_LIB_DIR)/obj" ]; then mkdir $(ELECTIONGUARD_BINDING_LIB_DIR)/obj; fi

	if [ -d "$(ELECTIONGUARD_BINDING_BENCH_DIR)/bin" ]; then rm -rf $(ELECTIONGUARD_BINDING_BENCH_DIR)/bin/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_BENCH_DIR)/bin" ]; then mkdir $(ELECTIONGUARD_BINDING_BENCH_DIR)/bin; fi
	if [ -d "$(ELECTIONGUARD_BINDING_BENCH_DIR)/obj" ]; then rm -rf $(ELECTIONGUARD_BINDING_BENCH_DIR)/obj/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_BENCH_DIR)/obj" ]; then mkdir $(ELECTIONGUARD_BINDING_BENCH_DIR)/obj; fi

	if [ -d "$(ELECTIONGUARD_BINDING_TEST_DIR)/bin" ]; then rm -rf $(ELECTIONGUARD_BINDING_TEST_DIR)/bin/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_TEST_DIR)/bin" ]; then mkdir $(ELECTIONGUARD_BINDING_TEST_DIR)/bin; fi
	if [ -d "$(ELECTIONGUARD_BINDING_TEST_DIR)/obj" ]; then rm -rf $(ELECTIONGUARD_BINDING_TEST_DIR)/obj/*; fi
	if [ ! -d "$(ELECTIONGUARD_BINDING_TEST_DIR)/obj" ]; then mkdir $(ELECTIONGUARD_BINDING_TEST_DIR)/obj; fi

	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR); fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/android" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/android; fi
	if [ ! -d "$(ELECTIONGUARD_BUILD_LIBS_DIR)/ios" ]; then mkdir $(ELECTIONGUARD_BUILD_LIBS_DIR)/ios; fi
endif

clean-netstandard: 
	@echo 🗑️ CLEAN NETSTANDARD
	dotnet clean ./bindings/netstandard/ElectionGuard/ElectionGuard.sln

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
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 17 2022" -A x64 \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DUSE_SANITIZER="address" \
		-DLOG_LEVEL=debug \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 --config Debug
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
		-DUSE_TEST_PRIMES=OFF \
		-DUSE_STATIC_ANALYSIS=ON \
		-DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="address" \
		-DCLANG_TIDY=ON \
		-DCPPCHECK=ON \
		-DIWYU=ON \
		-DLOG_LEVEL=debug \
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
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DUSE_STATIC_ANALYSIS=ON \
		-DUSE_DYNAMIC_ANALYSIS=ON \
		-DUSE_SANITIZER="thread" \
		-DLOG_LEVEL=debug \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/Debug/test/ElectionGuardCTests
endif

bench:
	@echo 🧪 BENCHMARK
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DLOG_LEVEL=info \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)
ifeq ($(OPERATING_SYSTEM),Windows)
	pwsh -Command "xcopy 'build\libs\x86_64\$(TARGET)\_deps\benchmark-build\src\libbenchmark.dll' 'build\libs\x86_64\$(TARGET)\test' /Q /Y;  $$null"
endif
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardBenchmark

bench-netstandard: build-netstandard
	@echo 🧪 BENCHMARK
	@echo net 6.0 x86
	./bindings/netstandard/ElectionGuard/ElectionGuard.Encryption.Bench/bin/x86/$(TARGET)/net6.0/ElectionGuard.Encryption.Bench
	@echo net 4.8 x86
	./bindings/netstandard/ElectionGuard/ElectionGuard.Encryption.Bench/bin/x86/$(TARGET)/net48/ElectionGuard.Encryption.Bench
	@echo net 6.0 x64
	./bindings/netstandard/ElectionGuard/ElectionGuard.Encryption.Bench/bin/x64/$(TARGET)/net6.0/ElectionGuard.Encryption.Bench
	@echo net 4.8 x64
	./bindings/netstandard/ElectionGuard/ElectionGuard.Encryption.Bench/bin/x64/$(TARGET)/net48/ElectionGuard.Encryption.Bench

# Test
test:
	@echo 🧪 TEST
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DLOG_LEVEL=debug \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DLOG_LEVEL=debug \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardCTests

test-msvc:
	@echo 🧪 TEST MSVC
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 -G "Visual Studio 17 2022" -A x64 \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DLOG_LEVEL=debug \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64 --config $(TARGET)
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/$(TARGET)/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/msvc/x64/test/$(TARGET)/ElectionGuardCTests
endif

test-netstandard: build-netstandard
	@echo 🧪 TEST NETSTANDARD
	dotnet test --configuration $(TARGET) ./bindings/netstandard/ElectionGuard/ElectionGuard.sln
ifeq ($(OPERATING_SYSTEM),Windows)
	dotnet test --configuration $(TARGET) ./bindings/netstandard/ElectionGuard/ElectionGuard.sln /property:Platform=x86
endif

coverage:
	@echo ✅ CHECK COVERAGE
ifeq ($(OPERATING_SYSTEM),Windows)
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) -G "MSYS Makefiles" \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
else
	cmake -S . -B $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET) \
		-DCMAKE_BUILD_TYPE=$(TARGET) \
		-DBUILD_SHARED_LIBS=ON \
		-DEXPORT_INTERNALS=ON \
		-DCAN_USE_VECTOR_INTRINSICS=ON \
		-DUSE_TEST_PRIMES=OFF \
		-DOPTION_ENABLE_TESTS=ON \
		-DCODE_COVERAGE=ON \
		-DUSE_STATIC_ANALYSIS=ON \
		-DCPM_SOURCE_CACHE=$(CPM_SOURCE_CACHE)
endif
	cmake --build $(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardTests
	$(ELECTIONGUARD_BUILD_LIBS_DIR)/x86_64/$(TARGET)/test/ElectionGuardCTests

# Sample Data

generate-sample-data:
	@echo Generate Sample Data
