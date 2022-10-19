set(SOURCES 
./main.cpp
./GreenhouseManager.cpp
./Drivers/Com_Interfaces/I2C_I.cpp
./Drivers/Sensors/SHT4x.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS ".")

