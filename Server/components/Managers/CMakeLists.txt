message(STATUS "Building component: Server managers")

# Set source files to variable SOURCES
set(SOURCES
./EventManager.cpp
./NetworkManager.cpp
./ComponentController.cpp
./WifiConnectionHolder.cpp)

set(DIRECTORIES
"." 
"../"
"../../main")

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS ${DIRECTORIES}
                                REQUIRES Common_components mqtt json)
