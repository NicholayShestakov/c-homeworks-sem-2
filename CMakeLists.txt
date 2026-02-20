cmake_minimum_required(VERSION 3.25)
project(C_homeworks C)
add_compile_options(-Wall -Wextra -pedantic)

# Libraries
add_library(csv2txt src/csv/csv2txt.c)

# Executables
add_executable(csv src/csv/main.c)

# Links
target_link_libraries(csv PRIVATE csv2txt)
