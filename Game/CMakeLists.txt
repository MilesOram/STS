cmake_minimum_required (VERSION 3.8)

# set name for the target that will be specified inside this file
set (TARGET_NAME Main)

set(PYBIND11_PYTHON_VERSION 3.7.3 CACHE STRING "")
find_package(pybind11 CONFIG REQUIRED)  

add_executable (${TARGET_NAME} "Main.cpp" "enemies.cpp" "enemies.h" "events.cpp" "events.h"
"gamestate.h" "gamestate1.cpp" "gamestate2.cpp" "gamestate3.cpp" "ic_cards.cpp" "ic_cards.h"
"pc_info.cpp" "pc_info.h" "top.cpp" "top.h" "card.cpp" "card.h" "character.cpp" 
"character.h" "effects_triggers.cpp" "effects_triggers.h" "potions.h" "visuals.h"
"savestate.h" "potions.cpp" "relic.cpp"  "visuals.cpp" "savestate.cpp")
target_link_libraries(${TARGET_NAME} PRIVATE pybind11::embed pybind11::module pybind11::pybind11)

# set binary filenames
set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${TARGET_NAME}-debug )
set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_RELWITHDEBINFO ${TARGET_NAME}-relwidthdebinfo )
set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${TARGET_NAME}-release )
set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_MINSIZEREL ${TARGET_NAME}-minsizerel )

# set warning levels
if (MSVC)
	target_compile_options(${TARGET_NAME} PUBLIC /W4 /permissive)
else()
	target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic)
endif()