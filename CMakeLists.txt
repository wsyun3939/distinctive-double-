cmake_minimum_required(VERSION 3.0.0)
project(DISTINCTIVE-DOUBLE- VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(DISTINCTIVE-DOUBLE- main.c branch_and_bound.c EnumerateRelocation.c IntDequeue.c lower_bound.c support_function.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
