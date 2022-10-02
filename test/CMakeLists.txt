find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (
        ../include
        ${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable(test_ test.cpp)
target_link_libraries(test_ PUBLIC ${Boost_LIBRARIES})
add_test (NAME test COMMAND test_)