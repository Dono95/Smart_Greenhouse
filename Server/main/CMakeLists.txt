set(SOURCES 
./main.cpp
./GreenhouseManager.cpp)

set(DIRECTORIES
"."
"../components/")

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS ${DIRECTORIES})