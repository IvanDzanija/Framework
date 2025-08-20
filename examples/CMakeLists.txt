add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
