add_library(ml-base64 INTERFACE)
add_library(ml::base64 ALIAS ml-base64)
target_include_directories(ml-base64 INTERFACE include)
target_compile_features(ml-base64 INTERFACE cxx_std_20)