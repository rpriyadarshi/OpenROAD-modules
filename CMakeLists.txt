#################################################################
#
# Copyright (c) 2024, Daedel Inc.
# All rights reserved.
#
#################################################################

include("openroad")
project("prv")

swig_lib(NAME prv
         NAMESPACE prv
         I_FILE src/prv.i
         SCRIPTS src/prv.tcl
)

#################################################################
#
# Source files.
#
################################################################
target_sources(prv
  PRIVATE
  # Keep sorted
    src/Prv.cpp
    src/MakePrv.cpp
)


target_include_directories(prv
  PUBLIC
    include
)

target_link_libraries(prv
  PUBLIC
    odb
    dbSta
    OpenSTA
)

target_link_libraries(prv
  PRIVATE
)


if (Python3_FOUND AND BUILD_PYTHON)
  swig_lib(NAME          prv_py
           NAMESPACE     prv
           LANGUAGE      python
           I_FILE        src/PrvGen-py.i
           SWIG_INCLUDES ${ODB_HOME}/src/swig/common
                         ${ODB_HOME}/src/swig/python
                         ${ODB_HOME}/include
                         ${PROJECT_SOURCE_DIR}/include
           SCRIPTS       ${CMAKE_CURRENT_BINARY_DIR}/prv_py.py
  )

  target_include_directories(prv_py
    PUBLIC
      ../include
  )
  
  target_link_libraries(prv_py
    PUBLIC
      prv
    )

endif()

# Keep sorted
if(ENABLE_TESTS)
  add_subdirectory(test)
endif()

messages(
  TARGET prv
)
