# Copyright (c) 2018 Institute of Sound and Vibration Research, University of Southampton.
# All rights reserved.

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  #TODO add installation line guides for installing Doxygen via website www. ....
  message(FATAL_ERROR "Doxygen is needed to build the documentation. You can install it here at ...")
endif()

# Latex is used to create PDFs from the sphinx documentation
find_package( LATEX COMPONENTS PDFLATEX )

set( doxyfile_in          ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in )
set( doxyfile             ${PROJECT_BINARY_DIR}/doc/Doxyfile )
set( doxy_output_root     ${PROJECT_BINARY_DIR}/doc/doxygen/ )
set( doxy_input           ${PROJECT_SOURCE_DIR} )
# Used for checking dependencies when using Sphinx custom commands
set( doxy_xml_index_file ${PROJECT_BINARY_DIR}/doc/doxygen/xml/index.xml )
# Used as a varible config.py.in with Sphinx to configure the breathe extension.
set( doxy_xml_index       ${PROJECT_BINARY_DIR}/doc/doxygen/xml/ )

configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

# TODO There should be seperate commands for generating html and xml seperately. This however, requires cmake to edit the doxyfile . May have to be done manually.
# TODO Doxygen generates files on any edit to the code source. It will regenerate each time, causing slow downs. The only possible way I can think of regeneration to stop is to check git somehow, if even possible.
# HTML command
add_custom_command( OUTPUT ${doxy_xml_index_file}
		  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		  # The following should be ${doxyfile} only but it
		  # will break the dependency.
		  # The optimal solution would be creating a
		  # custom_command for ${doxyfile} generation
		  # but I still have to figure out how...
		  MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
		  # DEPENDS ${doxy_extra_files} # TODO <AF> Decide whether the subprojects should be added as dependency.
		  COMMENT "Generating Doxygen HTML and XML documentation")
		  
add_custom_target( doxygen-doc DEPENDS ${doxy_html_index_file}  )

# "Meta-target" to trigger build of all documentation outputs
add_custom_target( doc DEPENDS doxygen-doc )

find_package(Sphinx)
if(NOT Sphinx_FOUND)
  message(FATAL_ERROR "Sphinx is needed to build HTML-based documentation and VISR's web-api. You should install it here at ...")
endif()

# HTML output directory
set ( doc_html_dir     ${PROJECT_BINARY_DIR}/doc/html/)
set ( doc_pdf_output   ${PROJECT_BINARY_DIR}/doc/userdoc/pdf/)
# Used as dependencies when making custom command directive for Sphinx

set( doc_config_file    ${PROJECT_BINARY_DIR}/doc/conf.py)
set( doc_config_in      ${PROJECT_SOURCE_DIR}/doc/conf.py.in)
set( doc_source_root    ${PROJECT_SOURCE_DIR}/doc/)

set( doc_output_root   ${PROJECT_BINARY_DIR}/doc/ )

if(NOT DEFINED SPHINX_THEME)
    set(SPHINX_THEME default)
endif(NOT DEFINED SPHINX_THEME)

if(NOT DEFINED SPHINX_THEME_DIR)
    set(SPHINX_THEME_DIR)
endif(NOT DEFINED SPHINX_THEME_DIR)

# Function to define a target building HTML documentation from a source directory containing Sphinx rst code.
function( HtmlDocBuild targetName sourceDir outputDir depends )
  # message( STATUS "configure_file( ${configTemplate} ${outputDir}/html/conf.py @ONLY )" )
  configure_file( ${sourceDir}/conf.py.in ${outputDir}/html/conf.py @ONLY )
  add_custom_target( ${targetName}-html
    ${SPHINX_EXECUTABLE}
               -q
               -b html
               -c ${outputDir}/html
               -d ${outputDir}/cache
                ${sourceDir}
                ${outputDir}/html
  DEPENDS ${depends}
  COMMENT "HTML generation for target ${targetName}-html."
  )
endfunction(HtmlDocBuild)

# Function to define a target building PDF documentation from a source directory containing Sphinx rst code.
function( PdfDocBuild targetName sourceDir outputDir depends )
  #message( STATUS "configure_file( ${configTemplate} ${outputDir}/latex/conf.py @ONLY )" )
  configure_file( ${sourceDir}/conf.py.in ${outputDir}/latex/conf.py @ONLY )

  add_custom_command(
    OUTPUT ${outputDir}/latex/visr-cxx-tutorial.tex
    COMMAND ${SPHINX_EXECUTABLE}
               -q
               -b latex
               -c ${outputDir}/latex
               -d ${outputDir}/cache
                ${sourceDir}
                ${outputDir}/latex
  DEPENDS ${depends}
  COMMENT "LaTeX generation for target ${targetName}-pdf."
  )

  add_custom_target( ${targetName}-pdf
    # Multiple runs to sort undefined reference (very crude at the moment
    COMMAND ${PDFLATEX_COMPILER} visr-cxx-tutorial
    COMMAND ${PDFLATEX_COMPILER} visr-cxx-tutorial
	COMMAND ${PDFLATEX_COMPILER} visr-cxx-tutorial
	COMMAND ${CMAKE_COMMAND} -E rename ${outputDir}/latex/visr-cxx-tutorial.pdf ${outputDir}/${targetName}-${VISR_MAJOR_VERSION}.${VISR_MINOR_VERSION}.${VISR_PATCH_VERSION}.pdf
    WORKING_DIRECTORY ${outputDir}/latex
    DEPENDS ${outputDir}/latex/visr-cxx-tutorial.tex
    COMMENT "PDF compilation for target ${targetName}-pdf." )
endfunction(PdfDocBuild)

PdfDocBuild( doc ${doc_source_root} ${doc_output_root} doxygen-doc )

HtmlDocBuild( doc ${doc_source_root} ${doc_output_root} doxygen-doc )

# Add the Sphinx-generated targets to the "doc" meta-target
add_dependencies( doc doc-pdf doc-html ) 
