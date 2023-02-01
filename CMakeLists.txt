cmake_minimum_required(VERSION "3.17.0")

project("Advent_of_code")

# Set language version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${PROJECT_SOURCE_DIR}/cmake/standard_warnings_and_optimizations.cmake")

set(INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")

if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE STREQUAL Debug))
	add_definitions("-D_GLIBCXX_DEBUG")
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")

set(
	FILES
	"Day01_SonarSweep"
	"Day02_Dive"
	"Day03_BinaryDiagnostic"
	"Day04_GiantSquid"
	"Day05_HydrothermalVenture"
	"Day06_Lanternfish"
	"Day07_TheTreacheryOfWhales"
	"Day08_SevenSegmentSearch"
	"Day09_SmokeBasin"
	"Day10_SyntaxScoring"
	"Day11_DumboOctopus"
	"Day12_PassagePathing"
	"Day13_TransparentOrigami"
	"Day14_ExtendedPolymerization"
	"Day15_Chiton"
	"Day16_PacketDecoder"
	"Day17_TrickShot"
	"Day18_Snailfish"
	"Day19_BeaconScanner"
	"Day20_TrenchMap"
	"Day21_DiracDice"
	"Day22_ReactorReboot"
	"Day23_Amphipod"
	"Day24_ArithmeticLogicUnit"
)

foreach(FILE ${FILES})
	add_executable(${FILE} "${PROJECT_SOURCE_DIR}/src/${FILE}.cpp")
	set_property(
		TARGET ${FILE} 
		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)
	set_standard_cxx_flags(${FILE} PRIVATE)
	set_standard_linker_flags(${FILE} PRIVATE)
endforeach()