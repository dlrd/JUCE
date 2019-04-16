#\-------------------------------------- . -----------------------------------/#
# Filename : CMakeList.txt               | Juce                                #
# Author   : Alexandre Buge              |                                     #
# Started  : 04/04/2014 11:21            |                                     #
#/-------------------------------------- . -----------------------------------\#

FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# Enable stack-unwinding support in c objects on gcc-based platforms.
# Failing to do so will cause your program to be terminated when a png
# or a jpeg exception is thrown on linux or macosx.
IF (CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# define plateform specific build environment
IF (WIN32)
  IF (NOT SMODE_OPTION_STATIC_LIBS)
    ADD_DEFINITIONS(-DJUCE_DLL_BUILD=1)
  ENDIF (NOT SMODE_OPTION_STATIC_LIBS)
  SET(JUCE_PLATFORM_SPECIFIC_LIBRARIES
    kernel32.lib
    user32.lib
    shell32.lib
    gdi32.lib
    vfw32.lib
    comdlg32.lib
    winmm.lib
    wininet.lib
    rpcrt4.lib
    ole32.lib
    advapi32.lib
    ws2_32.lib
    OpenGL32.Lib
    Version.lib
    Imm32.lib
    Shlwapi.lib 
  )
ELSE (WIN32)
  IF   (UNIX)
    IF   (APPLE)
      FIND_LIBRARY(CARBON_LIBRARY Carbon)
      FIND_LIBRARY(COCOA_LIBRARY Cocoa)
      FIND_LIBRARY(QUARTZ_LIBRARY Quartz)
      FIND_LIBRARY(AUDIO_TOOLBOX_LIBRARY AudioToolbox)
      FIND_LIBRARY(CORE_AUDIO_LIBRARY CoreAudio)
      FIND_LIBRARY(CORE_MIDI_LIBRARY CoreMidi)
      FIND_LIBRARY(IO_KIT_LIBRARY IOKit)
      FIND_LIBRARY(AGL_LIBRARY AGL)
      FIND_LIBRARY(ACCELERATE_LIBRARY Accelerate)
      FIND_LIBRARY(WEB_KIT_LIBRARY WebKit)
      MARK_AS_ADVANCED (CARBON_LIBRARY COCOA_LIBRARY CORE_AUDIO_LIBRARY CORE_MIDI_LIBRARY IO_KIT_LIBRARY AGL_LIBRARY QUARTZ_LIBRARY QUARTZ_CORE_LIBRARY AUDIO_TOOLBOX_LIBRARY ACCELERATE_LIBRARY WEB_KIT_LIBRARY)

      SET(JUCE_PLATFORM_SPECIFIC_LIBRARIES ${CARBON_LIBRARY} ${COCOA_LIBRARY} ${CORE_AUDIO_LIBRARY} ${CORE_MIDI_LIBRARY} ${IO_KIT_LIBRARY} ${AGL_LIBRARY} ${QUARTZ_LIBRARY} ${QUARTZ_CORE_LIBRARY} ${AUDIO_TOOLBOX_LIBRARY} ${ACCELERATE_LIBRARY} ${WEB_KIT_LIBRARY} "-lobjc -lc++")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
    ELSE (APPLE)
      FIND_PACKAGE(X11 REQUIRED)
      INCLUDE_DIRECTORIES(${X11_INCLUDE_DIR})
      INCLUDE_DIRECTORIES(/usr/include/freetype2)
      ADD_DEFINITIONS(-DLINUX)      
      SET(JUCE_PLATFORM_SPECIFIC_LIBRARIES ${X11_LIBRARIES} asound freetype Xinerama pthread rt dl)
    ENDIF(APPLE)
  ENDIF(UNIX)
ENDIF(WIN32)

##########################
# define juce files 

FILE(GLOB JUCE_AUDIO_BASICS_SOURCES
  modules/juce_audio_basics/*.cpp
  modules/juce_audio_basics/*.h
)

FILE(GLOB JUCE_AUDIO_DEVICES_SOURCES
  modules/juce_audio_devices/*.cpp
  modules/juce_audio_devices/*.h
)

FILE(GLOB JUCE_AUDIO_FORMATS_SOURCES
  modules/juce_audio_formats/*.cpp
  modules/juce_audio_formats/*.h
)

FILE(GLOB JUCE_CORE_SOURCES
  modules/juce_core/*.cpp
  modules/juce_core/*.h
)

FILE(GLOB JUCE_CRYPTOGRAPHY_SOURCES
  modules/juce_cryptography/*.cpp
  modules/juce_cryptography/*.h
)

FILE(GLOB JUCE_DATA_STRUCTURES_SOURCES
  modules/juce_data_structures/*.cpp
  modules/juce_data_structures/*.h
)

FILE(GLOB JUCE_EVENTS_SOURCES
  modules/juce_events/*.cpp
  modules/juce_events/*.h
)
FILE(GLOB JUCE_GRAPHICS_SOURCES
  modules/juce_graphics/*.cpp
  modules/juce_graphics/*.h
)

FILE(GLOB JUCE_GUI_BASICS_SOURCES
  modules/juce_gui_basics/*.cpp
  modules/juce_gui_basics/*.h
)

FILE(GLOB JUCE_GUI_EXTRA_SOURCES
  modules/juce_gui_extra/*.cpp
  modules/juce_gui_extra/*.h
)

FILE(GLOB JUCE_OPENGL_SOURCES
  modules/juce_opengl/*.cpp
  modules/juce_opengl/*.h
)

FILE(GLOB JUCE_OCS_SOURCES
  modules/juce_osc/*.cpp
  modules/juce_osc/*.h
)

##########################
# define JUCE sources and files groups

SET(JUCE_SOURCES
  ${EXTERN_PATH}/include/juce/AppConfig.h
  ${EXTERN_PATH}/include/juce/juce.h
  ${JUCE_AUDIO_BASICS_SOURCES}
  ${JUCE_AUDIO_DEVICES_SOURCES}
  ${JUCE_AUDIO_FORMATS_SOURCES}
  ${JUCE_CORE_SOURCES}
  ${JUCE_CRYPTOGRAPHY_SOURCES}
  ${JUCE_DATA_STRUCTURES_SOURCES}
  ${JUCE_EVENTS_SOURCES}
  ${JUCE_GRAPHICS_SOURCES}
  ${JUCE_GUI_BASICS_SOURCES}
  ${JUCE_GUI_EXTRA_SOURCES}
  ${JUCE_OPENGL_SOURCES}
  ${JUCE_OCS_SOURCES}
)

# define JUCE library

##########################
ADD_LIBRARY(juce ${SMODE_LIBRARY_TYPE} ${JUCE_SOURCES})


SET_TARGET_PROPERTIES(juce PROPERTIES 
  PROJECT_LABEL "EXTERN LIB Juce"
  PDB_OUTPUT_DIRECTORY "${SMODE_PDB_OUTPUT_DIRECTORY}"
)

TARGET_LINK_LIBRARIES(juce
  ${JUCE_PLATFORM_SPECIFIC_LIBRARIES}
  ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS juce
  LIBRARY DESTINATION ${SMODE_INSTALL_DIRECTORY}
  ARCHIVE DESTINATION ${SMODE_EXTERN_LIB_DIR}
  RUNTIME DESTINATION ${SMODE_INSTALL_DIRECTORY}
  COMPONENT Smode
)

SMODE_BINARY_PACKER(juce) 
SMODE_BINARY_SIGN(juce)
