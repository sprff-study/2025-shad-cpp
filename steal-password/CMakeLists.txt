set(CMAKE_CXX_COMPILER "g++-14")

string(RANDOM LENGTH 6 ALPHABET 123456789 RANDOM_PASSWORD)
string(RANDOM LENGTH 4 ALPHABET 123456789 RANDOM_CODE)

add_executable(steal_password main.cpp verify.cpp)
target_compile_options(steal_password PRIVATE -O0)

set_property(
    SOURCE verify.cpp
    PROPERTY
        COMPILE_DEFINITIONS
            PPASSWORD=${RANDOM_PASSWORD}
            PSECURECODE=${RANDOM_CODE}
)
