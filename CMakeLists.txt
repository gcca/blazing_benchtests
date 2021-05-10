cmake_minimum_required (VERSION 3.17)

project (btests)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package (Threads)
include (ExternalProject)
ExternalProject_Add (benchmark
  CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=no
    -DCMAKE_INSTALL_PREFIX=benchmarkprefix -DCMAKE_BUILD_TYPE=RELEASE
  EXCLUDE_FROM_ALL TRUE
  GIT_REPOSITORY git@github.com:google/benchmark.git GIT_TAG v1.5.3
  UPDATE_COMMAND "")
ExternalProject_Get_property (benchmark BINARY_DIR)
set (BM_ROOT ${BINARY_DIR}/benchmarkprefix)
set (BM_INCLUDE ${BM_ROOT}/include)
set (BM_LIB ${BM_ROOT}/lib/libbenchmark.a)
set (BM_MAIN ${BM_ROOT}/lib/libbenchmark_main.a)
file (MAKE_DIRECTORY ${BM_ROOT}/include)
file (MAKE_DIRECTORY ${BM_ROOT}/lib)

macro (add_namedlib name include lib dep)
  add_library (${name} INTERFACE IMPORTED)
  target_include_directories (${name} INTERFACE ${include})
  target_link_libraries (${name} INTERFACE ${lib} ${dep})
endmacro ()

macro (add_namedlib_with name target include lib dep)
  add_namedlib (${name} ${include} ${lib} ${dep})
  add_dependencies (${name} ${target})
endmacro ()

add_namedlib_with (Benchmark::Benchmark benchmark ${BM_INCLUDE} ${BM_LIB} Threads::Threads)
add_namedlib_with (Benchmark::Main Benchmark::Benchmark ${BM_INCLUDE} ${BM_MAIN} Benchmark::Benchmark)

add_library (Conda::Env INTERFACE IMPORTED)
target_include_directories (Conda::Env INTERFACE ${CONDA_PREFIX}/include)
target_link_directories (Conda::Env INTERFACE ${CONDA_PREFIX}/lib)

add_library (Blazing::Io INTERFACE IMPORTED)
target_include_directories (Blazing::Io INTERFACE ${BLAZING_IO_INCLUDE})
target_link_libraries (Blazing::Io INTERFACE ${BLAZING_IO_LIB})

add_library (blazrow OBJECT blazingsql arrow)
target_link_directories (blazrow PUBLIC $ENV{HOME}/.miniconda3/envs/snow/lib)
target_link_libraries (blazrow Benchmark::Main arrow Conda::Env Blazing::Io stdc++fs pfm)

macro (benchmark_executables)
  foreach (arg ${ARGN})
    add_executable (${arg} ${arg})
    target_link_libraries (${arg} blazrow)
  endforeach ()
endmacro ()

benchmark_executables (
  readfile_test
  createdirectory_test
  deletedirectory_test
  movedirectory_test)
