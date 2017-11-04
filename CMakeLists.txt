CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
project(oathgen)

find_library(CryptoPP crypto++)

if (CryptoPP-NOTFOUND)
    message(FATAL-ERROR "Crypto++ not found!")
else ()
    message(STATUS "Crypto++ found! (${CryptoPP})")
    link_libraries(crypto++)
endif ()

# Test 32/64 bits
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    message(STATUS "Target is 64 bits")
else()
    message(WARNING "Target is 32 bits")
endif()

file(GLOB OATHGEN_SRC src/*.hpp src/*.cpp)
file(GLOB OATHGEN_B32_TEST base32test/*.hpp base32test/*.cpp)

add_executable(oathgen ${OATHGEN_SRC})
add_executable(base32test ${OATHGEN_B32_TEST})