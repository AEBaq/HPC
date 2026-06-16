cmake_minimum_required(VERSION 3.10)
project(FindKmers)

set(CMAKE_C_STANDARD 99)

add_executable(k-mer src/main.c)

add_executable(k-mer-mono src/main_mono.c)
target_compile_options(k-mer-mono PRIVATE -O3 -Wall -Wextra)

add_executable(k-mer-multi src/main_multi.c)
target_link_libraries(k-mer-omp PRIVATE OpenMP::OpenMP_C)
target_compile_options(k-mer-omp PRIVATE -O3 -Wall -Wextra -fopenmp)

