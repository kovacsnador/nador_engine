#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "atlas_maker::turbojpeg-static" for configuration "Debug"
set_property(TARGET atlas_maker::turbojpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(atlas_maker::turbojpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "ASM_NASM;C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libturbojpeg.a"
  )

list(APPEND _cmake_import_check_targets atlas_maker::turbojpeg-static )
list(APPEND _cmake_import_check_files_for_atlas_maker::turbojpeg-static "${_IMPORT_PREFIX}/lib/libturbojpeg.a" )

# Import target "atlas_maker::jpeg-static" for configuration "Debug"
set_property(TARGET atlas_maker::jpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(atlas_maker::jpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "ASM_NASM;C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libjpeg.a"
  )

list(APPEND _cmake_import_check_targets atlas_maker::jpeg-static )
list(APPEND _cmake_import_check_files_for_atlas_maker::jpeg-static "${_IMPORT_PREFIX}/lib/libjpeg.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
