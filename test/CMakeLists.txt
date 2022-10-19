find_package (Boost COMPONENTS unit_test_framework REQUIRED)
find_package(OpenMP REQUIRED)
include_directories (
        ../include
        ${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable(test_ test.cpp)
target_link_libraries(test_ PUBLIC ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)
add_test (NAME test COMMAND test_)