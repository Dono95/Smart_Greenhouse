message(STATUS "Building component: Server sensors data")

# Set source file to variable SOURCES
set(SOURCES
./SensorsData.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "." "../"
                                PRIV_REQUIRES Common_components)
