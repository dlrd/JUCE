#\-------------------------------------- . -----------------------------------/#
# Filename : CMakeList.txt               | Juce                                #
# Author   : Alexandre Buge              |                                     #
# Started  : 04/04/2014 11:21            |                                     #
#/-------------------------------------- . -----------------------------------\#

FIND_PACKAGE(OpenGL REQUIRED)

SET(JUCE_AUDIO_BASICS_SOURCES
  modules/juce_audio_basics/juce_audio_basics.cpp
  modules/juce_audio_basics/juce_audio_basics.h
)

SET(JUCE_AUDIO_DEVICES_SOURCES
  modules/juce_audio_devices/juce_audio_devices.cpp
  modules/juce_audio_devices/juce_audio_devices.h
)

SET(JUCE_AUDIO_FORMATS_SOURCES
  modules/juce_audio_formats/juce_audio_formats.cpp
  modules/juce_audio_formats/juce_audio_formats.h
)

SET(JUCE_CORE_SOURCES
  modules/juce_core/juce_core.cpp
  modules/juce_core/juce_core.h
)

SET(JUCE_CRYPTOGRAPHY_SOURCES
  modules/juce_cryptography/juce_cryptography.cpp
  modules/juce_cryptography/juce_cryptography.h
)

SET(JUCE_DATA_STRUCTURES_SOURCES
  modules/juce_data_structures/juce_data_structures.cpp
  modules/juce_data_structures/juce_data_structures.h
)

SET(JUCE_EVENTS_SOURCES
  modules/juce_events/juce_events.cpp
  modules/juce_events/juce_events.h
)

SET(JUCE_GRAPHICS_SOURCES
  modules/juce_graphics/juce_graphics.cpp
  modules/juce_graphics/juce_graphics.h
)

SET(JUCE_GUI_BASICS_SOURCES
  modules/juce_gui_basics/juce_gui_basics.cpp
  modules/juce_gui_basics/juce_gui_basics.h
)

SET(JUCE_GUI_EXTRA_SOURCES
  modules/juce_gui_extra/juce_gui_extra.cpp
  modules/juce_gui_extra/juce_gui_extra.h
)

SET(JUCE_OPENGL_SOURCES
  modules/juce_opengl/juce_opengl.cpp
  modules/juce_opengl/juce_opengl.h
)

SET(JUCE_OCS_SOURCES
  modules/juce_osc/juce_osc.cpp
  modules/juce_osc/juce_osc.h
)

SET(JUCE_SOURCES
  ${SMODE_EXTERN_PATH}/include/juce/AppConfig.h
  ${SMODE_EXTERN_PATH}/include/juce/juce.h
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

ADD_LIBRARY(juce ${SMODE_LIBRARY_TYPE} ${JUCE_SOURCES})

TARGET_INCLUDE_DIRECTORIES(juce PRIVATE ${OPENGL_INCLUDE_DIR})

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
  TARGET_LINK_LIBRARIES(juce kernel32 user32 shell32 gdi32 vfw32 comdlg32 winmm wininet rpcrt4 ole32 advapi32 ws2_32 OpenGL32 Version Imm32 Shlwapi)
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
      TARGET_LINK_LIBRARIES(juce PRIVATE ${CARBON_LIBRARY} ${COCOA_LIBRARY} ${CORE_AUDIO_LIBRARY} ${CORE_MIDI_LIBRARY} ${IO_KIT_LIBRARY} ${AGL_LIBRARY} ${QUARTZ_LIBRARY} ${QUARTZ_CORE_LIBRARY} ${AUDIO_TOOLBOX_LIBRARY} ${ACCELERATE_LIBRARY} ${WEB_KIT_LIBRARY} "-lobjc -lc++")
      set_source_files_properties(${JUCE_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++")
    ELSE (APPLE)
      FIND_PACKAGE(X11 REQUIRED)
      TARGET_INCLUDE_DIRECTORIES(juce PRIVATE ${X11_INCLUDE_DIR})
      TARGET_INCLUDE_DIRECTORIES(juce PRIVATE /usr/include/freetype2)
      ADD_DEFINITIONS(-DLINUX)      
      TARGET_LINK_LIBRARIES(juce ${X11_LIBRARIES} asound freetype Xinerama pthread rt dl curl)
    ENDIF(APPLE)
  ENDIF(UNIX)
ENDIF(WIN32)


OPTION(JUCE_ASIO_SUPPORT "Build Juce Asio support." ON)
IF (JUCE_ASIO_SUPPORT)
  TARGET_INCLUDE_DIRECTORIES(juce PRIVATE ${SMODE_EXTERN_PATH}/include/asio)
  ADD_DEFINITIONS(-DJUCE_ASIO=1)
ELSE (JUCE_ASIO_SUPPORT)
  ADD_DEFINITIONS(-DJUCE_ASIO=0)
ENDIF(JUCE_ASIO_SUPPORT)

if(APPLE)
  TARGET_LINK_LIBRARIES(juce PRIVATE ${OPENGL_LIBRARIES})
else()
  TARGET_LINK_LIBRARIES(juce ${OPENGL_LIBRARIES})
endif()

# The juce dll is part of SmodeSDK because the LibraryGenerator uses it.
smode_install_target(juce)
SET_TARGET_PROPERTIES(juce PROPERTIES 
  DEBUG_POSTFIX "${SMODE_DEBUG_POSTFIX}" # for SmodeSDK
  RELWITHDEBINFO_POSTFIX "${SMODE_RELEASE_POSTFIX}"
)

SMODE_BINARY_PACK(juce) 
SMODE_BINARY_SIGN(juce)
