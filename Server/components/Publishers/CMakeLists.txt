message(STATUS "Building component: Server greenhouse publishers")

# Set source file to variable SOURCES
set(SOURCES
./BluetoothDataPublisher.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "."
                                PRIV_REQUIRES Publishers Observers Common_components)
