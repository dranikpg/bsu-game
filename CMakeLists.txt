cmake_minimum_required(VERSION 3.17)
project(bsu_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

set(SOURCES
        src/main.cpp
        src/ecs/world.cpp
        src/components/position_component.cpp
        src/components/sprite_component.cpp
        src/prototype/prototype_widget.cpp
        src/systems/rendering_system.cpp
        src/context/painter_context.cpp
        src/resources/animation.cpp
        src/components/impulse_component.cpp
        src/systems/movement_system.cpp
        src/components/bounds_component.cpp
        src/context/input_context.cpp
        src/systems/input_movement_system.cpp
        src/resources/animation.cpp
        src/systems/animation_system.cpp
        src/components/animation_component.cpp
        src/context/window_context.cpp
        src/utils/parser/ase_animation_parser.cpp
        src/map/map_loader.cpp
        src/resources/path.cpp
        src/resources/level.cpp
        src/systems/level_system.cpp
        src/context/level_context.cpp
        src/levels/example/example_level.cpp
        src/utils/parser/ase_animation_parser.cpp
        src/systems/movement_animation_sync_system.cpp
        src/components/movement_animation_sync_component.cpp
        src/systems/behaviour_system.cpp
        src/components/behaviour_component.cpp
        src/levels/bsu_entrance/bsu_entrance_level.cpp
        src/levels/bsu_entrance/guard_behaviour.cpp,
        src/components/path_follow_component.cpp
        src/systems/path_follow_system.cpp))

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

qt5_add_resources(SOURCES assets/resources.qrc)
add_executable(${PROJECT_NAME} ${SOURCES})

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
