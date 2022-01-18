#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# CMakeLists.txt: Geant4 Tutorial for Hep/Medicine Users
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----------------------------------------------------------------------------
# Setup Section: You have to set the following parameter
#----------------------------------------------------------------------------
# Set the name of your main program
  set( NAME_MAIN_PROGRAM Application_Main)


#----------------------------------------------------------------------------
# Intact Section: You don't need to touch the following part
#----------------------------------------------------------------------------
# Set the project name
  project( ${NAME_MAIN_PROGRAM} )

# Find Geant4 package
  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()

# Setting ROOT
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  find_package(ROOT CONFIG REQUIRED)
  include(${ROOT_USE_FILE})

# Set up Geant4 include directories and compile definition
  include(${Geant4_USE_FILE})
  include_directories(${PROJECT_SOURCE_DIR}/include)

# CRY
  include(FetchContent)
  FetchContent_Declare(
      cry
      URL http://nuclear.llnl.gov/simulation/cry_v1.7.tar.gz
      URL_MD5 85f240bebe81fe0b257e92ef1d390a83
  )
  FetchContent_MakeAvailable(cry)
  file(GLOB cry_headers ${cry_SOURCE_DIR}/src/*.h)
  file(GLOB cry_sources ${cry_SOURCE_DIR}/src/*.cc)
  include_directories(${cry_SOURCE_DIR}/src/)

# Locate sources and headers for this project
  file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
  file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Compile and link your application
  add_definitions(${ROOT_CXX_FLAGS})
  add_executable(${NAME_MAIN_PROGRAM} ${NAME_MAIN_PROGRAM}.cc ${sources} ${headers} ${cry_sources} ${cry_headers})
  target_link_libraries(${NAME_MAIN_PROGRAM} ${Geant4_LIBRARIES} ROOT::MathCore ROOT::Hist ROOT::Tree)

# Install the executable
  set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/..)
  install(TARGETS ${NAME_MAIN_PROGRAM} DESTINATION bin)
