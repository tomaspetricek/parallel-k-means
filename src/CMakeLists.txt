include_directories(../include)
add_executable(mcc main.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(mcc PUBLIC OpenMP::OpenMP_CXX)