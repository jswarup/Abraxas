# CMakeLists.txt ____________________________________________________________________________________________________________

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)  
set( CMAKE_VERBOSE_MAKEFILE ON)

set( AX_VERSIONID "2.9") 

project( Abraxas VERSION ${AX_VERSIONID} DESCRIPTION "..Sitting Ducks")

enable_language( CXX) 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

#____________________________________________________________________________________________________________________________

message(STATUS "${CMAKE_PROJECT_NAME}: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_BUILD_TYPE}\n")

#____________________________________________________________________________________________________________________________

add_compile_options("$<$<CONFIG:DEBUG>:-DCY_DEBUG>")

set(ROOT_PATH "${CMAKE_SOURCE_DIR}/../..")    
set(SOURCE_PATH "${ROOT_PATH}/src") 
set(TOOLS_PATH "${ROOT_PATH}/tools")  
set(THIRDPARTY_PATH "${ROOT_PATH}/3rdparty")   

set(ELMER_PATH ${THIRDPARTY_PATH}/ElmLib)   


set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${ELMER_PATH}) 
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/glfw/include)
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/glew-2.2.0/include)
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/glm)
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/assimp/include)
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/imgui ${THIRDPARTY_PATH}/imgui/backends)
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/implot/) 
set(AX_3RD_PARTY_INCLUDES ${AX_3RD_PARTY_INCLUDES} ${THIRDPARTY_PATH}/imgui_plugins/) 

include_directories(  ${SOURCE_PATH} ${SOURCE_PATH}/console ${AX_3RD_PARTY_INCLUDES})

link_directories( ${THIRDPARTY_PATH}/glfw/lib/ ${THIRDPARTY_PATH}/assimp/lib ${THIRDPARTY_PATH}/glew-2.2.0/lib/Release/x64  ${ELMER_PATH})

#____________________________________________________________________________________________________________________________

set ( AX_IMGUI_SOURCES
    ${THIRDPARTY_PATH}/imgui/imgui.cpp   
    ${THIRDPARTY_PATH}/imgui/imgui_demo.cpp
    ${THIRDPARTY_PATH}/imgui/imgui_draw.cpp
    ${THIRDPARTY_PATH}/imgui/imgui_tables.cpp
    ${THIRDPARTY_PATH}/imgui/imgui_widgets.cpp
    ${THIRDPARTY_PATH}/imgui/backends/imgui_impl_glfw.cpp
    ${THIRDPARTY_PATH}/imgui/backends/imgui_impl_opengl3.cpp
    ${THIRDPARTY_PATH}/implot/implot.cpp
    ${THIRDPARTY_PATH}/implot/implot_items.cpp
    ${THIRDPARTY_PATH}/implot/implot_demo.cpp
)

add_library( imgui STATIC ${AX_IMGUI_SOURCES} )

#____________________________________________________________________________________________________________________________


set ( AX_LATTICE_SOURCES 
    ${SOURCE_PATH}/abraxas/elems/ax_mesh.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_uictxt.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_appscreen.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_scenevw.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_plotvw.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_proppanel.cpp
    ${SOURCE_PATH}/abraxas/render/ax_glbuffers.cpp
    ${SOURCE_PATH}/abraxas/render/ax_glctxt.cpp
    ${SOURCE_PATH}/abraxas/render/ax_shader.cpp
    ${SOURCE_PATH}/abraxas/mosaic/ax_abraxasapp.cpp    
    ${SOURCE_PATH}/abraxas/carton/ax_biomeifc.cpp
)

add_library( lattice STATIC ${AX_LATTICE_SOURCES} )

#____________________________________________________________________________________________________________________________

set( AX_CONSOLE_SOURCES 
	${SOURCE_PATH}/console/ax_main.cpp	 
    ${SOURCE_PATH}/console/ax_docktest.cpp	 
    ${SOURCE_PATH}/console/ax_plottest.cpp	 
)

#____________________________________________________________________________________________________________________________
   
set(AX_3RD_PARTY_LIB ${AX_3RD_PARTY_LIB} opengl32 glfw3 assimp-vc143-mt.lib glew32 biome$<$<CONFIG:Debug>:d> sqlite3)
#set(AX_3RD_PARTY_LIB ${AX_3RD_PARTY_LIB} d3d12 d3dcompiler dxgi) 

add_executable( abraxas ${AX_CONSOLE_SOURCES} )  
target_link_libraries( abraxas   lattice imgui ${AX_3RD_PARTY_LIB})
set_target_properties( abraxas PROPERTIES DEBUG_POSTFIX "d")

add_custom_command(TARGET abraxas POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${THIRDPARTY_PATH}/assimp/bin/assimp-vc143-mt.dll"        
        "${THIRDPARTY_PATH}/glew-2.2.0/bin/Release/x64/glew32.dll"
        $<TARGET_FILE_DIR:abraxas>)                 # <--this is out-file path


add_custom_command(TARGET abraxas POST_BUILD
                   COMMAND  echo  "${CMAKE_PROJECT_NAME}: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_BUILD_TYPE}")

#____________________________________________________________________________________________________________________________