add_executable(solver-tests SolverTest.cpp)
target_link_libraries(solver-tests GTest::GTest GTest::Main ${PROJECT_NAME})
gtest_discover_tests(solver-tests)
