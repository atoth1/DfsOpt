add_executable(parser-tests ParserTest.cpp)
target_link_libraries(parser-tests GTest::GTest GTest::Main ${PROJECT_NAME})
gtest_discover_tests(parser-tests)

# Copy over inputs
configure_file("successful-parse.xml" "successful-parse.xml" COPYONLY)
configure_file("ill-formed.xml" "ill-formed.xml" COPYONLY)
configure_file("missing-reqs.xml" "missing-reqs.xml" COPYONLY)
configure_file("missing-player-attr.xml" "missing-player-attr.xml" COPYONLY)
configure_file("missing-dst-attr.xml" "missing-dst-attr.xml" COPYONLY)
configure_file("sanity-check-qbs.xml" "sanity-check-qbs.xml" COPYONLY)
configure_file("sanity-check-flex.xml" "sanity-check-flex.xml" COPYONLY)
