add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
