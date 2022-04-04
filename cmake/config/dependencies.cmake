find_package(doctest 2.4.7 CONFIG QUIET)
if(NOT doctest_FOUND AND ML_BASE64_BUILD_TEST)
  CPMAddPackage(doctest VERSION 2.4.7 GITHUB_REPOSITORY doctest/doctest)
endif()

if (ML_BASE64_BUILD_DOCUMENTATION)
  find_package(Doxygen 1.9.3 REQUIRED)
  CPMAddPackage(NAME doxygen-awesome-css VERSION 2.0.3 GITHUB_REPOSITORY jothepro/doxygen-awesome-css)
endif()