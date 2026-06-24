# CMake allows faster rebuilds and generates compile_commands.json
# which is required by clangd for code navigation and refactoring.
cmake_minimum_required(VERSION 3.15)
project(WatariGoEngine LANGUAGES CXX)

# Fix the FetchContent timestamp warning (CMP0135)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

# ==========================================
# 1. FETCH GOOGLE TEST
# ==========================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ==========================================
# 2. AUTOMATED SOURCE DISCOVERY
# ==========================================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_library(WatariBoard ${SRC_FILES})
add_executable(watari src/main.cpp)
target_link_libraries(watari PRIVATE WatariBoard)

# ==========================================
# 3. AUTOMATED TEST DISCOVERY
# ==========================================
enable_testing()
find_package(Threads REQUIRED)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "test/*.cpp")
add_executable(WatariTest ${TEST_SOURCES})

# Link everything together safely
target_link_libraries(WatariTest PRIVATE WatariBoard GTest::gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(WatariTest)
