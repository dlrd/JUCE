#\-------------------------------------- . -----------------------------------/#
# Filename : CMakeList.txt               | Juce                                #
# Author   : Alexandre Buge              |                                     #
# Started  : 04/04/2014 11:21            |                                     #
#/-------------------------------------- . -----------------------------------\#

OPTION(JUCE_MODULE_DISABLE_OPENGL "Disable build of Juce OpenGL Module." ON)
if (NOT JUCE_MODULE_DISABLE_OPENGL)
  FIND_PACKAGE(OpenGL REQUIRED)
  SET(JUCE_OPENGL_SOURCES
    modules/juce_opengl/juce_opengl.cpp
    modules/juce_opengl/juce_opengl.h
  )
endif ()


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

OPTION(JUCE_MODULE_DISABLE_GUI_BASICS "Disable build of Juce GUI basics module." OFF)
if (NOT JUCE_MODULE_DISABLE_GUI_BASICS)
  SET(JUCE_GUI_BASICS_SOURCES
    modules/juce_gui_basics/juce_gui_basics.cpp
    modules/juce_gui_basics/juce_gui_basics.h
  )
endif ()

OPTION(JUCE_MODULE_DISABLE_GUI_EXTRA "Disable build of Juce GUI extra module." OFF)
if (NOT JUCE_MODULE_DISABLE_GUI_EXTRA)
  SET(JUCE_GUI_EXTRA_SOURCES
    modules/juce_gui_extra/juce_gui_extra.cpp
    modules/juce_gui_extra/juce_gui_extra.h
  )
endif ()

SET(JUCE_OCS_SOURCES
  modules/juce_osc/juce_osc.cpp
  modules/juce_osc/juce_osc.h
)

SET(JUCE_SOURCES
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

if (JUCE_MODULE_DISABLE_OPENGL)
  target_compile_definitions(juce PUBLIC JUCE_MODULE_DISABLE_OPENGL=1)
else ()
  target_include_directories(juce PRIVATE ${OPENGL_INCLUDE_DIR})
  target_link_libraries(juce PRIVATE ${OPENGL_LIBRARIES})
endif ()

if (JUCE_MODULE_DISABLE_GUI_BASICS)
  target_compile_definitions(juce PUBLIC JUCE_MODULE_DISABLE_GUI_BASICS=1)
endif ()
if (JUCE_MODULE_DISABLE_GUI_EXTRA)
  target_compile_definitions(juce PUBLIC JUCE_MODULE_DISABLE_GUI_EXTRA=1)
endif ()

# Enable stack-unwinding support in c objects on gcc-based platforms.
# Failing to do so will cause your program to be terminated when a png
# or a jpeg exception is thrown on linux or macosx.
if (CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")
endif()

if (WIN32)
  target_link_libraries(juce PRIVATE kernel32 user32 shell32 gdi32 vfw32 comdlg32 winmm wininet rpcrt4 ole32 advapi32 ws2_32 Version Imm32 Shlwapi)
else ()
  if   (UNIX)
    if   (APPLE)
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
      target_link_libraries(juce PRIVATE ${CARBON_LIBRARY} ${COCOA_LIBRARY} ${CORE_AUDIO_LIBRARY} ${CORE_MIDI_LIBRARY} ${IO_KIT_LIBRARY} ${AGL_LIBRARY} ${QUARTZ_LIBRARY} ${QUARTZ_CORE_LIBRARY} ${AUDIO_TOOLBOX_LIBRARY} ${ACCELERATE_LIBRARY} ${WEB_KIT_LIBRARY} "-lobjc -lc++")
      set_source_files_properties(${JUCE_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++")
    else ()
      if (NOT JUCE_MODULE_DISABLE_GUI_BASICS)
        FIND_PACKAGE(X11 REQUIRED)
        target_include_directories(juce PRIVATE ${X11_INCLUDE_DIR})
        target_link_libraries(juce PRIVATE ${X11_LIBRARIES})
      endif ()
      target_include_directories(juce PRIVATE /usr/include/freetype2)
      target_compile_definitions(juce PRIVATE LINUX)
      target_link_libraries(juce PRIVATE asound freetype Xinerama pthread rt dl curl)
    endif()
  endif()
endif()

smode_install_target(juce EXPORT)

oil_binary_sign(juce)
