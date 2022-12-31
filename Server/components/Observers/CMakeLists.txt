message(STATUS "Building component: Server observers")

# Set source file to variable SOURCES
set(SOURCES
./BluetoothDataObserver.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "." "../"
                                PRIV_REQUIRES Common_components Managers)
