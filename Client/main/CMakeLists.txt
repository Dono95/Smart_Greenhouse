message(STATUS "Building: main")

# Set source files to variable SOURCES
set(SOURCES 
./main.cpp
./GreenhouseManager.cpp
./ClientStatusIndicator)

set(DIRECTORIES
"../components/")

# Include for common code; need to be reconsidered
idf_component_register(
  SRCS ${SOURCES}
  INCLUDE_DIRS ${DIRECTORIES}
  REQUIRES Common_components Bluetooth)

