#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Marble" for configuration "Release"
set_property(TARGET Marble APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Marble PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libmarblewidget-qt5.dll.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Astro;Qt5::Core;Qt5::Svg;Qt5::PrintSupport;Qt5::Concurrent"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/./libmarblewidget-qt5.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Marble )
list(APPEND _IMPORT_CHECK_FILES_FOR_Marble "${_IMPORT_PREFIX}/lib/libmarblewidget-qt5.dll.a" "${_IMPORT_PREFIX}/./libmarblewidget-qt5.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
