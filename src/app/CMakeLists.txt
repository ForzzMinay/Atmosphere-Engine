find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(atmosphere
    main.cpp
)

target_link_libraries(atmosphere PRIVATE
    Qt6::Core
    atmosphere-core
)