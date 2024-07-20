# Tests
include("openroad")

set(TEST_NAMES
)

foreach(TEST_NAME IN LISTS TEST_NAMES)
  or_integration_test("prv" ${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/regression)
endforeach()

find_package(Boost)
