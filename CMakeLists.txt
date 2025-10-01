cmake_minimum_required(VERSION 3.10)

# Set the project name and specify it's a C project
project(ML-Library C)

# Set C standard (you can change this to C11 or C17 if needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include header files
include_directories(header tests)

# Add main source files as a library
add_library(math_funcs STATIC src/math_funcs.c src/matrix.c src/vector.c)
add_library(progress_bar STATIC src/progressbar.c)

# Add the executable using source files
add_executable(main src/main.c src/regression.c src/file_handling.c src/eval_metrics.c)
add_executable(default_lin_reg src/default_lin_reg.c src/regression.c src/file_handling.c src/eval_metrics.c)
add_executable(default_log_reg src/default_log_reg.c src/regression.c src/file_handling.c src/eval_metrics.c)
add_executable(default_soft_reg src/default_soft_reg.c src/regression.c src/file_handling.c src/eval_metrics.c)

# Add test executable
add_executable(testActivation tests/test_activations.c tests/unity.c)
add_executable(testDot tests/test_dot_product.c tests/unity.c)
add_executable(testIden tests/test_identity.c tests/unity.c)
add_executable(testMatVect tests/test_mat_vect_mult.c tests/unity.c)
add_executable(testTrans tests/test_transpose.c tests/unity.c)
add_executable(testMatOps tests/test_matrix_operations.c tests/unity.c)
add_executable(testDataManip tests/test_data_manipulation.c tests/unity.c src/file_handling.c)
add_executable(testVectOps tests/test_vector_operations.c tests/unity.c)
add_executable(testRandPerm tests/test_random_permutation.c tests/unity.c)

# Link test executable with the library under test
target_link_libraries(testActivation PRIVATE math_funcs m)
target_link_libraries(testDot PRIVATE math_funcs m)
target_link_libraries(testIden PRIVATE math_funcs m)
target_link_libraries(testMatVect PRIVATE math_funcs m)
target_link_libraries(testTrans PRIVATE math_funcs m)
target_link_libraries(testDataManip PRIVATE math_funcs m)
target_link_libraries(testMatOps PRIVATE math_funcs m)
target_link_libraries(testVectOps PRIVATE math_funcs m)
target_link_libraries(testRandPerm PRIVATE math_funcs m)
target_link_libraries(main PRIVATE math_funcs progress_bar m)
target_link_libraries(default_lin_reg PRIVATE math_funcs progress_bar m)
target_link_libraries(default_log_reg PRIVATE math_funcs progress_bar m)
target_link_libraries(default_soft_reg PRIVATE math_funcs progress_bar m)

add_compile_options(-Wall -Wextra -Wpedantic)

# Add a test target for CTest
# enable_testing()
# add_test(NAME math_test COMMAND testActivation)
