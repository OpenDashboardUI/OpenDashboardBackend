#
# CMake wrapper to call windeployqt in Windows
#
# Based on
#   Author: equalsraf
#   URL: https://github.com/equalsraf/neovim-qt/blob/master/cmake/WinDeployQt.cmake
#
# Example usage:
#	include(win_deploy_qt)
#	win_deploy_qt(
#		TARGET
#			${TARGET_NAME} # Name of the target
#		QML_DIR
#			"${CMAKE_SOURCE_DIR}/libs/library_name/resources/qml"
#		QML_MODULES_DIRS # Optional
#			"${CMAKE_SOURCE_DIR}/external/library_name/modules/"
#		DEPLOY_DIRECTORY_NAME
#			"Application_name_win_x64"
#		DEPLOY_ADDITIONAL_FILES # Optional
#			"${CMAKE_SOURCE_DIR}/external/some_precompiled_lib/win/x86_64/PCANBasic.dll"
#	)


function(win_deploy_qt)

	if(WIN32) # WIN32 is also true for x64

		cmake_parse_arguments(_deploy
			"COMPILER_RUNTIME;FORCE"
			"TARGET;QML_DIR"
			"INCLUDE_MODULES;EXCLUDE_MODULES;QML_MODULES_DIRS;DEPLOY_DIRECTORY_NAME;DEPLOY_ADDITIONAL_FILES"
			${ARGN}
		)

		if(NOT _deploy_TARGET)
			message(FATAL_ERROR "A TARGET must be specified")
		endif()

		if(NOT _deploy_QML_MODULES_DIRS)
			message(STATUS "QML_MODULES_DIRS specified: None")
		else()
			message(STATUS "QML_MODULES_DIRS specified: " ${_deploy_QML_MODULES_DIRS})
		endif()

		if(NOT _deploy_DEPLOY_DIRECTORY_NAME)
			message(FATAL_ERROR "A DEPLOY_DIRECTORY_NAME must be specified")
		endif()

		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			list(APPEND _ARGS --debug)
		elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
			 list(APPEND _ARGS --release-with-debug-info)
		elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
			list(APPEND _ARGS --release)
		endif()

		if(_deploy_COMPILER_RUNTIME)
			list(APPEND _ARGS --compiler-runtime)
		endif()

		if(_deploy_FORCE)
			list(APPEND _ARGS --force)
		endif()

		foreach(mod ${_deploy_INCLUDE_MODULES})
			string(TOLOWER ${mod} mod)
			string(REPLACE "qt5::" "" mod ${mod})
			list(APPEND _ARGS "--${mod}")
		endforeach()
		foreach(mod ${_deploy_EXCLUDE_MODULES})
			string(TOLOWER ${mod} mod)
			string(REPLACE "qt5::" "" mod ${mod})
			list(APPEND _ARGS "--no-${mod}")
		endforeach()

		find_program(_deploy_PROGRAM windeployqt
			PATHS $ENV{QTDIR}/bin/)
		if(_deploy_PROGRAM)
			message(STATUS "Found ${_deploy_PROGRAM}")
		else()
			message(FATAL_ERROR "Unable to find windeployqt")
		endif()

		if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
			message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
		endif()

		set(DEPLOY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/windeployqt/${_deploy_DEPLOY_DIRECTORY_NAME})

		message(STATUS "Deploy directory:" ${DEPLOY_DIRECTORY})
		message(STATUS "Deploy target: " $<TARGET_FILE:${_deploy_TARGET}>)
		message(STATUS "Deploy QML_MODULE_DIRS: " ${_deploy_QML_MODULES_DIRS})

		#
		# Create deploy directory and copy all files from the binary tree output directory of the target into it
		#
		add_custom_target(windeployqt_setup_${_deploy_TARGET}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPLOY_DIRECTORY}
			COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${_deploy_TARGET}>/ ${DEPLOY_DIRECTORY}
			DEPENDS ${_deploy_TARGET}
			COMMENT "Preparing Qt runtime dependencies"
		)

		#
		# Copy QML module directories to output dir, if specified
		#
		if(_deploy_QML_MODULES_DIRS)
			add_custom_target(windeployqt_qml_module_dirs_${_deploy_TARGET}
				COMMAND ${CMAKE_COMMAND} -E echo "Qml module directories to deploy: " ${_deploy_QML_MODULES_DIRS}
				COMMAND ${CMAKE_COMMAND} -E copy_directory ${_deploy_QML_MODULES_DIRS} ${DEPLOY_DIRECTORY}
				DEPENDS ${_deploy_TARGET}
				COMMENT "Copy qml module dirs"
			)
		else()
			add_custom_target(windeployqt_qml_module_dirs_${_deploy_TARGET}
				DEPENDS ${_deploy_TARGET}
				COMMENT "Don't copy qml module dirs since nothing was specified"
			)
		endif()

		#
		# Copy additional file to output dir, if specified
		#
		if(_deploy_DEPLOY_ADDITIONAL_FILES)
			add_custom_target(windeployqt_additional_files_${_deploy_TARGET}
				COMMAND ${CMAKE_COMMAND} -E echo "Additional files to deploy: " ${_deploy_DEPLOY_ADDITIONAL_FILES}
				COMMAND ${CMAKE_COMMAND} -E copy ${_deploy_DEPLOY_ADDITIONAL_FILES} ${DEPLOY_DIRECTORY}
				DEPENDS ${_deploy_TARGET}
				COMMENT "Copy additional files like libraries to the deploy directory"
			)
		else()
			add_custom_target(windeployqt_additional_files_${_deploy_TARGET}
				DEPENDS ${_deploy_TARGET}
				COMMENT "Don't copy additional files since nothing was specified"
			)
		endif()

		#
		# Run windeployqt
		#
		add_custom_target(windeployqt_${_deploy_TARGET}
			COMMAND ${_deploy_PROGRAM} --qmldir ${_deploy_QML_DIR} --dir ${DEPLOY_DIRECTORY} ${_ARGS} $<TARGET_FILE:${_deploy_TARGET}>
			DEPENDS ${_deploy_TARGET} windeployqt_setup_${_deploy_TARGET} windeployqt_additional_files_${_deploy_TARGET}
			COMMENT "Preparing Qt runtime dependencies"
		)

	endif()
endfunction()
