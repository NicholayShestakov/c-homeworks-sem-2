cmake_minimum_required(VERSION 3.25)
project(C_homeworks C)
add_compile_options(-g -Wall -Wextra -pedantic)
enable_testing()

# Libraries
add_library(avl_tree src/avl_tree/avl_tree.c)

# Executables
add_executable(aerosoft src/avl_tree/main.c)

# Links
target_link_libraries(aerosoft PRIVATE avl_tree m)

# Tests
add_executable(avl_tree_test src/avl_tree/test/test_avl_tree.c)
add_test(NAME AVLTreeTest COMMAND avl_tree_test)
