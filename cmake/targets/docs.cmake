## ======================================================================================
## Collect headers to document
## ======================================================================================
get_target_property(public_include_dir ml::base64 INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${public_include_dir}/*base64.hpp)
string(REPLACE ";" " " headers_space_separated "${headers}")

## ======================================================================================
## Configure doxygen
## ======================================================================================
set(DOXYFILE_PROJECT_NAME           "Base64")
set(DOXYFILE_PROJECT_VERSION        "${PROJECT_VERSION}")
set(DOXYFILE_PROJECT_DESCRIPTION    "${PROJECT_DESCRIPTION}")
set(DOXYFILE_PROJECT_LOGO           "")
set(DOXYFILE_OUTPUT_DIRECTORY       "${CMAKE_CURRENT_BINARY_DIR}/docs")
set(DOXYFILE_STRIP_FROM_PATH        "")
set(DOXYFILE_STRIP_FROM_INC_PATH    "")
set(DOXYFILE_INPUT                  "${headers_space_separated}")
set(DOXYFILE_HTML_HEADER            "${CMAKE_CURRENT_BINARY_DIR}/docs/header.html")
set(DOXYFILE_HTML_FOOTER            "")
set(DOXYFILE_HTML_STYLESHEET        "")
set(DOXYFILE_HTML_EXTRA_STYLESHEET  "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css")
set(DOXYFILE_HTML_EXTRA_FILES       "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js")
set(DOXYFILE_TREEVIEW_WIDTH         "335")
set(DOXYFILE_COLORSTYLE_HUE         "209")
set(DOXYFILE_COLORSTYLE_SAT         "255")
set(DOXYFILE_COLORSTYLE_GAMMA       "113")
file(MAKE_DIRECTORY ${DOXYFILE_OUTPUT_DIRECTORY})
configure_file(docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile @ONLY)
configure_file(docs/header.html ${CMAKE_CURRENT_BINARY_DIR}/docs/header.html @ONLY)

## ======================================================================================
## Doxygen target
## ======================================================================================
add_custom_target(docs
  COMMAND Doxygen::doxygen ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "[ml::base64] Generating documentation with Doxygen"
  VERBATIM)