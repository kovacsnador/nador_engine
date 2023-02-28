#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "atlas_maker::jpeg" for configuration "Release"
set_property(TARGET atlas_maker::jpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(atlas_maker::jpeg PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libjpeg.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libjpeg-62.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS atlas_maker::jpeg )
list(APPEND _IMPORT_CHECK_FILES_FOR_atlas_maker::jpeg "${_IMPORT_PREFIX}/lib/libjpeg.dll.a" "${_IMPORT_PREFIX}/bin/libjpeg-62.dll" )

# Import target "atlas_maker::turbojpeg" for configuration "Release"
set_property(TARGET atlas_maker::turbojpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(atlas_maker::turbojpeg PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libturbojpeg.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libturbojpeg.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS atlas_maker::turbojpeg )
list(APPEND _IMPORT_CHECK_FILES_FOR_atlas_maker::turbojpeg "${_IMPORT_PREFIX}/lib/libturbojpeg.dll.a" "${_IMPORT_PREFIX}/bin/libturbojpeg.dll" )

# Import target "atlas_maker::turbojpeg-static" for configuration "Release"
set_property(TARGET atlas_maker::turbojpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(atlas_maker::turbojpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_NASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libturbojpeg.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS atlas_maker::turbojpeg-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_atlas_maker::turbojpeg-static "${_IMPORT_PREFIX}/lib/libturbojpeg.a" )

# Import target "atlas_maker::jpeg-static" for configuration "Release"
set_property(TARGET atlas_maker::jpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(atlas_maker::jpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_NASM;C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libjpeg.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS atlas_maker::jpeg-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_atlas_maker::jpeg-static "${_IMPORT_PREFIX}/lib/libjpeg.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
