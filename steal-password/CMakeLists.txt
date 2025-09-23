set(CMAKE_CXX_COMPILER "g++-14")

add_executable(steal_password main.cpp verify.cpp rate_limit.cpp steal.cpp)
target_compile_options(steal_password PRIVATE -O0)
