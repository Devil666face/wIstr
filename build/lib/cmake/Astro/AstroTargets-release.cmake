#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Astro" for configuration "Release"
set_property(TARGET Astro APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Astro PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libastro.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/./libastro.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Astro )
list(APPEND _IMPORT_CHECK_FILES_FOR_Astro "${_IMPORT_PREFIX}/lib/libastro.dll.a" "${_IMPORT_PREFIX}/./libastro.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
