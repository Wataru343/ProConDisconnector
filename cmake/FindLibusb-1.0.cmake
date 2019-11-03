include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)

set(libname "libusb-1.0")
set(headername "libusb.h")

find_path("${libname}_INCLUDE_PATH"
    NAMES "${libname}/${headername}"
    PATHS "${LIBUSB-1.0_DIR}/include"
        "~/Library/Frameworks"
        "/Library/Frameworks"
        "/usr/local/include"
        "/usr/include")


mark_as_advanced("${${libname}_INCLUDE_PATH}")

find_library("${libname}_LIBRARY_PATH"
    NAMES "usb-1.0" "usb1.0"
    PATHS "${LIBUSB-1.0_DIR}/lib"
        "${LIBUSB-1.0_DIR}/bin"
        "~/Library/Frameworks"
        "/Library/Frameworks"
        "/usr/local/lib"
        "/usr/local/lib64"
        "/usr/lib"
        "/usr/lib64")

mark_as_advanced("${${libname}_LIBRARY_PATH}")

get_filename_component(${libname}_LIBRARY_DIR "${${libname}_LIBRARY_PATH}" DIRECTORY)


if(${libname}_INCLUDE_PATH AND ${libname}_LIBRARY_PATH)
    set(version_file "${${libname}_LIBRARY_DIR}/pkgconfig/${libname}.pc")
    if(EXISTS ${version_file})
        file(STRINGS ${version_file} version_line
            REGEX "Version: .+")

        foreach(line ${version_line})
            if(line MATCHES "Version: (.+)")
                set(LIBUSB-1.0_VERSION ${CMAKE_MATCH_1})
            endif()
        endforeach()
    endif()

    set(LIBUSB-1.0_FOUND TRUE)
else()
    set(LIBUSB-1.0_FOUND FALSE)
endif()


list(APPEND LIBUSB-1.0_INCLUDE_DIRS ${${libname}_INCLUDE_PATH})
list(APPEND LIBUSB-1.0_LIBRARY_DIRS ${${libname}_LIBRARY_DIR})
list(APPEND LIBUSB-1.0_LIBRARIES ${${libname}_LIBRARY_PATH})

list(REMOVE_DUPLICATES LIBUSB-1.0_INCLUDE_DIRS)
list(REMOVE_DUPLICATES LIBUSB-1.0_LIBRARY_DIRS)
list(REMOVE_DUPLICATES LIBUSB-1.0_LIBRARIES)

set(LIBUSB-1.0_INCLUDE_DIRS ${LIBUSB-1.0_INCLUDE_DIRS} CACHE STRING "")
set(LIBUSB-1.0_LIBRARY_DIRS ${LIBUSB-1.0_LIBRARY_DIRS} CACHE STRING "")
set(LIBUSB-1.0_LIBRARIES ${LIBUSB-1.0_LIBRARIES} CACHE STRING "")


find_package_handle_standard_args(LIBUSB-1.0
    REQUIRED_VARS LIBUSB-1.0_INCLUDE_DIRS LIBUSB-1.0_LIBRARIES LIBUSB-1.0_LIBRARY_DIRS
    VERSION_VAR LIBUSB-1.0_VERSION)
