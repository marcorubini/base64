# =================================================================================================
# Handle options
# =================================================================================================

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(PROJECT_IS_TOP_LEVEL TRUE)
endif()

option(ML_BASE64_BUILD_TEST          "Build unit test for ml::base64" ${PROJECT_IS_TOP_LEVEL})
option(ML_BASE64_BUILD_DOCUMENTATION "Compile doxygen documentation"  ${PROJECT_IS_TOP_LEVEL})

if (ML_BASE64_BUILD_TEST)
  set(test_status "ON")
else()
  set(test_status "OFF ")
endif()

if (ML_BASE64_BUILD_DOCUMENTATION)
  set(docs_status "ON")
else()
  set(docs_status "OFF ")
endif()

message(STATUS "[ml::base64] Building ${CMAKE_BUILD_TYPE} mode with: ${CMAKE_CXX_FLAGS}")
message(STATUS "[ml::base64] Unit tests         : ${test_status} (via ML_BASE64_BUILD_TEST)")
message(STATUS "[ml::base64] Docs               : ${docs_status} (via ML_BASE64_BUILD_DOCUMENTATION)")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)