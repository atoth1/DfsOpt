add_executable(outputter-tests OutputterTest.cpp)
target_link_libraries(outputter-tests GTest::GTest GTest::Main ${PROJECT_NAME})
gtest_discover_tests(outputter-tests)

# Copy over gold file
configure_file("num-requested.txt.gold" "num-requested.txt.gold" COPYONLY)
configure_file("fewer-num-requested.txt.gold" "fewer-num-requested.txt.gold" COPYONLY)
configure_file("captain-mode.txt.gold" "captain-mode.txt.gold" COPYONLY)
configure_file("captain-mode-dst.txt.gold" "captain-mode-dst.txt.gold" COPYONLY)
