set(SOURCES 
./main.cpp
./GreenhouseManager.cpp)


idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        PRIV_REQUIRES Publishers Observers Common_components Bluetooth)