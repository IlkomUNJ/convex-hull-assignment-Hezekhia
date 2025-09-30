# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TugasPert4_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TugasPert4_autogen.dir\\ParseCache.txt"
  "TugasPert4_autogen"
  )
endif()
