cmake_minimum_required(VERSION 3.0)
set(name cest_tests)
project(${name})

# Now test C++23's type_info::operator==
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_STANDARD 23)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_STANDARD 23)
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
endif()

add_compile_options(-Wall)

# Use lsb_release to display Ubuntu version (useful within Github Actions)
function(display_os_information)
  if (WIN32)
    # todo: name LSB_RELEASE_DESC more generically and set for Windows here
  else()
    find_program(LSB_RELEASE_EXEC lsb_release)
    if (NOT LSB_RELEASE_EXEC OR NOT CMAKE_SYSTEM_NAME MATCHES "Linux")
      message("Cannot determine Ubuntu version using lsb_release.")
      return()
    endif()

    execute_process(COMMAND "${LSB_RELEASE_EXEC}" --short --description
                    OUTPUT_VARIABLE LSB_RELEASE_DESC
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS "OS Version:         ${LSB_RELEASE_DESC}")
  endif()

endfunction()

display_os_information()

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_BUILD_TYPE:   ${CMAKE_BUILD_TYPE}")

add_executable(${name} ${name}.cpp)
add_test(all_${name} ${name})
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)

# Indicates to test cest's constexpr (and runtime) support
target_compile_definitions(${name} PRIVATE CONSTEXPR_CEST=1)

if (DEFINED USE_CEST_VECTOR_ETC)
  # Silence CMake https://stackoverflow.com/a/36452112/2023370 :
  set(ignoreMe "${USE_CEST_VECTOR_ETC}")
  message("USE_CEST_VECTOR_ETC is defined. Using cest::vector etc.")
  target_compile_definitions(${name} PRIVATE USE_CEST_VECTOR_ETC=1)
else()
  message("USE_CEST_VECTOR_ETC is NOT defined. Using std::vector etc.")
endif ()

enable_testing()
