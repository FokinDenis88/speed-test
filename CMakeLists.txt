# CMakeList.txt : CMake project for speed-test, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Can be different with target
project("speed-test" VERSION 1.0)
set(TARGET_NAME speed-test)


#============================Google Test========================================
# http://google.github.io/googletest/quickstart-cmake.html
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#=================================================================================


set(SOURCES
    src/main.cpp
    )

set(INCLUDES
    include
    external/include
    my_tests/include
    external/FileStreamWrapper/include
    C:/Development/Programs/Libraries/Maths/eigen-3.4-rc1
    C:/Development/Projects/Templates/!MY/File Write, Read/FileStreamWrapper/include
    )

set(HEADER_FILES_FILTER
    include/tester-speed.hpp
    include/test-results-analitics.hpp
    include/test-speed.hpp
    include/write-line-console.hpp
    include/repeat-macros.hpp
    )
set(DOC_FILTER
    doc/general/to-do.txt
    doc/general/copy-past-helper.txt
    )
set(MY_TESTS_FILTER
    my_tests/AccumulateVsSum.h
    my_tests/MatrixMult.h
    my_tests/MatrixParallel.h
    my_tests/MeanSquare.h
    my_tests/MLP.h
    my_tests/Old.txt
    my_tests/OmpVsSTL.h
    my_tests/Sample.h
    my_tests/StructVsLocal.h
    my_tests/summation-for-neural-networks.hpp
    my_tests/switch.hpp
    my_tests/variables-outside-for-loop.hpp
    my_tests/vec-ops.hpp
    )

find_package(OpenMP REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add source to this project's executable.
add_executable(${TARGET_NAME} ${SOURCES} ${HEADER_FILES_FILTER} ${DOC_FILTER} ${MY_TESTS_FILTER})
# Include Directories
target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDES})
target_link_libraries(${TARGET_NAME} PRIVATE OpenMP::OpenMP_CXX)

# Filters for Grouping files in project
source_group("Header Files" FILES ${HEADER_FILES_FILTER})
source_group("Doc" FILES ${DOC_FILTER})
source_group("My_Tests" FILES ${MY_TESTS_FILTER})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
  set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 17)
endif()
set_property(TARGET ${TARGET_NAME} PROPERTY SUBSYSTEM CONSOLE)





# Help Information
# TODO: Add tests and install targets if needed.


# Добавляет директории для дальнейшей обработки. В них нужно добавлять еще одни CMakeLists.txt файлы
# add_subdirectory (Hello)

#add_library (Hello hello.cxx)
#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable (helloDemo demo.cxx demo_b.cxx)
#target_link_libraries (helloDemo LINK_PUBLIC Hello)

#set(SOURCE_EXE main.cpp)   set(SOURCE_LIB foo.cpp)
#add_library(foo STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo
#add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main
#target_link_libraries(main foo)
#include_directories(foo)			# Расположение заголовочных файлов
#file(GLOB <variable> [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS] [<globbing-expressions>...])
#target_compile_options(first-test PRIVATE -fexceptions)

#find_package(OpenMP)
#if (OPENMP_FOUND)
    #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

#set_property(TARGET speed-test PROPERTY SUBSYSTEM WINDOWS)
#CMAKE_CXX_FLAGS    /SUBSYSTEM:CONSOLE

#source_group("Header Files" FILES ${INCLUDES} REGULAR_EXPRESSION *)

#OpenMP
#find_package(OpenMP REQUIRED)
#target_link_libraries(artificial-neural-network PRIVATE OpenMP::OpenMP_CXX)