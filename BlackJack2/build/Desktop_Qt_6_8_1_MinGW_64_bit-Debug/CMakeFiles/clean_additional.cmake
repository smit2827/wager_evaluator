# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BlackJack2_autogen"
  "CMakeFiles\\BlackJack2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BlackJack2_autogen.dir\\ParseCache.txt"
  )
endif()
