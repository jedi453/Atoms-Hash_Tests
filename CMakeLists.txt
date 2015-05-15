# Set Project Name
project(Atoms-Hash_Tests)

# Set Minimum CMake Version Required
cmake_minimum_required(VERSION 2.8)

# Enforce "Out of Source" Build
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
  IF(insource)
    MESSAGE(FATAL_ERROR "To Build, Please run: mkdir _build && cd _build && cmake .. && make")
  ENDIF(insource)

set(atoms_SOURCES atom.c atom-main.c hash.c)
# For a Large Number of Source files you can Create it Simply By Using:
#file(GLOB atoms_SOURCES *.c)


# Copy Word and Number Files to Directory Where Binary is to Be Stored
#set(atoms_FILES aspell-word-list-en-95.txt numbers2.txt numbers.txt tests.txt word-list.txt wordsEn.txt)
file(GLOB atoms_FILES *.txt)
file(COPY ${atoms_FILES} DESTINATION ${CMAKE_BINARY_DIR})

# Create an Executable file Called atom from sources
add_executable(atom ${atoms_SOURCES})

IF(UNIX)
  TARGET_LINK_LIBRARIES(atom m)
ENDIF(UNIX)
