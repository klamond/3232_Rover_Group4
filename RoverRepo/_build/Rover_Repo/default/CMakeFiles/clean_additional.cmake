# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.cmf"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.hex"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.hxl"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.mum"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.o"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.sdb"
  "D:\\Documents\\UNB\\Winter26\\Embeded (ECE-3232)\\The Harvest\\RoverRepo\\out\\Rover_Repo\\default.sym"
  )
endif()
