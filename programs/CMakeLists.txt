# this is executed via cmake in libpalattice/ (..) as subdirectory


project(palattice-programs CXX)
set(CMAKE_BUILD_TYPE Debug)


# build
add_executable(lattice lattice.cpp)
add_executable(convertlattice convertlattice.cpp)
add_executable(interptwiss interptwiss.cpp)
add_executable(fields fields.cpp)


# link
target_link_libraries(lattice palattice ${Z_LIBRARY})
target_link_libraries(convertlattice palattice ${Z_LIBRARY})
target_link_libraries(interptwiss palattice ${GSL_LIBRARY} ${GSLCBLAS_LIBRARY} ${Z_LIBRARY})
target_link_libraries(fields palattice ${GSL_LIBRARY} ${GSLCBLAS_LIBRARY} ${Z_LIBRARY})


# target 'programs'
add_custom_target(programs
  DEPENDS lattice convertlattice interptwiss fields
  )


# install convertlattice
install(TARGETS convertlattice
   DESTINATION bin
  )