include(ExternalProject)

if (NOT TARGET CAENlibs)
# ----- CAENlibs package -----
ExternalProject_Add( 
                    CAENlibs
                    GIT_REPOSITORY ${CAENlibs_repository}
                    GIT_TAG ${CAENlibs_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCAENALL=TRUE -DINCLUDE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/CAEN
                    PREFIX ${CMAKE_BINARY_DIR}/CAENlibs-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/CAENlibs
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )      

if(UNIX)
   set(CAEN_DEFS "-DUNIX" "-DLINUX" CACHE INTERNAL "CAEN definitions" FORCE)
elseif(WIN32)
   set(CAEN_DEFS "-D_WIN32" "-DWIN32" CACHE INTERNAL "CAEN definitions" FORCE)
endif()
mark_as_advanced(FORCE CAEN_DEFS)
endif()
