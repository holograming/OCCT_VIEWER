cmake_minimum_required(VERSION 3.17)


#EXT_LIB_DIR 
if (NOT DEFINED EXT_LIB_DIR)
  set (EXT_LIB_DIR "External lib root path" CACHE PATH "External lib folder not found")
  get_filename_component (EXT_LIB_DIR "${EXT_LIB_DIR}" ABSOLUTE)
  message(FATAL_ERROR "-- Need EXT_LIB_DIR path")
elseif (NOT DEFINED EXT_LIB_DIR)
  set (EXT_LIB_DIR "${EXT_LIB_DIR}" CACHE PATH "${EXT_LIB_DIR_DESCR}" FORCE)
endif()

# Project name
project(OCCT_VIEWER)

#--------------------------------------------------------------------------
# For dll copy
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}") 
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXECUTABLE_OUTPUT_PATH}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXECUTABLE_OUTPUT_PATH}")

FILE(GLOB_RECURSE HEADER_SRCS *.h)
FILE(GLOB_RECURSE CPP_SRCS *.cpp)
FILE(GLOB_RECURSE QRC_RES *.qrc)

REMOVE_DEFINITIONS(-D_MBCS) 
add_definitions(-D_USE_MATH_DEFINES)

include_directories(${PROJECT_SOURCE_DIR})

#----------------------------------------------------------------------------------------------------------------------------
FIND_PACKAGE(VTK)
message("Found VTK ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION}")
INCLUDE(${VTK_USE_FILE})
link_directories(${VTK_LIBRARY_DIRS})

find_package (Qt5OpenGL)
set(CMAKE_AUTOMOC ON)
qt5_add_resources(QRC_RES ${QRC_RES})

#----------------------------------------------------------------------------------------------------------------------------
find_package(OpenCASCADE)
include_directories(${OpenCASCADE_INCLUDE_DIR})
link_directories(${OpenCASCADE_LIBRARY_DIR})
message("Found OCE v. ${OpenCASCADE_MAJOR_VERSION}.${OpenCASCADE_MINOR_VERSION}.${OpenCASCADE_MAINTENANCE_VERSION}")
link_directories(${EXT_LIB_DIR}/3rdParty/freetype-2.5.5-vc14-64/lib)
link_directories(${EXT_LIB_DIR}/3rdParty/tcltk-86-64/lib)

#----------------------------------------------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} WIN32 ${HEADER_SRCS} ${CPP_SRCS} ${QRC_RES})
target_link_libraries(${PROJECT_NAME} ${OpenCASCADE_LIBRARIES} ${VTK_LIBRARIES} Qt5::OpenGL)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

#----------------------------------------------------------------------------------------------------------------------------

SET(VTK_BTN_DIR ${VTK_DIR}/../../../Bin)

#----------------------------------------------------------------------------------------------------------------------------

MESSAGE("	- Copy VTK DLLs path: ${VTK_BTN_DIR}")
foreach(A_LIB ${VTK_MODULES_REQUESTED})	
    #Message( - ${A_LIB})
    SET(A_LIB_NAME "${A_LIB}-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll")
    if(EXISTS ${VTK_BTN_DIR}/${A_LIB_NAME})
        FILE(COPY "${VTK_BTN_DIR}/${A_LIB_NAME}" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
    endif()
    SET(A_DLIB_NAME "${A_LIB}-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}_d.dll")
    if(EXISTS ${VTK_BTN_DIR}/${A_DLIB_NAME})
        FILE(COPY "${VTK_BTN_DIR}/${A_DLIB_NAME}"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    endif()
endforeach()

#----------------------------------------------------------------------------------------------------------------------------

SET(QT_ROOT_DIR ${EXT_LIB_DIR}/3rdParty/qt5.11.3-vc14-64/)
SET(QT_BINARY_DIR ${EXT_LIB_DIR}/3rdParty/qt5.11.3-vc14-64/bin)
MESSAGE("	- Copy Qt DLLs path: ${QT_BINARY_DIR}")
set(QT5__DLL_NAMES 
        "Qt5Core"               
		"Qt5Gui"               
		"Qt5Network"             
		"Qt5OpenGL"  
        "Qt5Widgets"
        "Qt5Xml"
	)

FOREACH(A_DLL_NAME ${QT5__DLL_NAMES})
    if(${${A_DLL_NAME}_FOUND})
        FILE(COPY "${QT_BINARY_DIR}/${A_DLL_NAME}.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
        FILE(COPY "${QT_BINARY_DIR}/${A_DLL_NAME}d.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)		
    endif()
ENDFOREACH(A_DLL_NAME)
	
set(QT5_PLUG_IN_PLATFORM_DLL_NAMES 
    "qwindows"               
)

FOREACH(A_DLL_NAME ${QT5_PLUG_IN_PLATFORM_DLL_NAMES})
    FILE(COPY "${QT_ROOT_DIR}/plugins/platforms/${A_DLL_NAME}.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/platforms)
    FILE(COPY "${QT_ROOT_DIR}/plugins/platforms/${A_DLL_NAME}d.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/platforms)
ENDFOREACH()

set(QT5_PLUG_IN_IMGFORMAT_DLL_NAMES 
    "qgif"
    "qjpeg"
    "qtga"
    "qtiff"
    "qwbmp"
    "qico"
)

FOREACH(A_DLL_NAME ${QT5_PLUG_IN_IMGFORMAT_DLL_NAMES})
    FILE(COPY "${QT_ROOT_DIR}/plugins/imageformats/${A_DLL_NAME}.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/imageformats)		
    FILE(COPY "${QT_ROOT_DIR}/plugins/imageformats/${A_DLL_NAME}d.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/imageformats)
ENDFOREACH()

#----------------------------------------------------------------------------------------------------------------------------

MESSAGE("	- Copy OSS DLLs path: ${OpenCASCADE_BINARY_DIR}d")
foreach(A_LIB ${OpenCASCADE_LIBRARIES})		
    if(EXISTS ${OpenCASCADE_BINARY_DIR}d/${A_LIB}.dll)
        FILE(COPY "${OpenCASCADE_BINARY_DIR}d/${A_LIB}.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
        FILE(COPY "${OpenCASCADE_BINARY_DIR}i/${A_LIB}.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
    endif()
endforeach()

SET(FREETYPE_BINARY_DIR ${EXT_LIB_DIR}/3rdParty/freetype-2.5.5-vc14-64/bin)
MESSAGE("	- Copy FreeType DLLs path: ${FREETYPE_BINARY_DIR}d")
if(EXISTS ${FREETYPE_BINARY_DIR}/freetype.dll)
    FILE(COPY "${FREETYPE_BINARY_DIR}/freetype.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
    FILE(COPY "${FREETYPE_BINARY_DIR}/freetype.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
endif()

SET(TCL_BINARY_DIR ${EXT_LIB_DIR}/3rdParty/tcltk-86-64/bin)
MESSAGE("	- Copy TCL DLLs path: ${TCL_BINARY_DIR}d")
foreach(A_LIB "tcl86" "tk86")	
    if(EXISTS ${TCL_BINARY_DIR}/${A_LIB}.dll)
        FILE(COPY "${TCL_BINARY_DIR}/${A_LIB}.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
        FILE(COPY "${TCL_BINARY_DIR}/${A_LIB}.dll"  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
    endif()
endforeach()