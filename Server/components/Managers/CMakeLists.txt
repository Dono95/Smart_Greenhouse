message(STATUS "Building component: Server bluetooth")

# Set source file to variable SOURCES
set(SOURCES
./EventManager.cpp
./NetworkManager.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "."
                                REQUIRES Common_components)
