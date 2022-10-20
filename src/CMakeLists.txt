add_library(${PROJECT_NAME} DfsOpt_Outputter.cpp DfsOpt_Solver.cpp DfsOpt_XmlParser.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE LIBXMLXX::LIBXMLXX)

set(EXE_NAME dfs-opt)
add_executable(${EXE_NAME} DfsOpt_Main.cpp)
target_link_libraries(${EXE_NAME} ${PROJECT_NAME})
