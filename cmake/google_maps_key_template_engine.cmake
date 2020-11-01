set(maps_script ${CMAKE_CURRENT_LIST_DIR}/../scripts/google_maps_key_template_engine.py)

function(generate_google_maps_html_file)

	find_package(Python3 REQUIRED COMPONENTS Interpreter)

	if (NOT DEFINED Python3_FOUND)
		message(FATAL_ERROR "Python3 not found. Please check your installation and PATH variable.")
	endif()

	set(oneValueArgs INPUT_FILE OUTPUT_FILE API_KEY)
	cmake_parse_arguments(ARGS "" "${oneValueArgs}" "" ${ARGN} )

	if (NOT DEFINED ARGS_API_KEY)
		message(WARNING "Please provide a google maps API key to use the map view (-DGOOGLE_MAPS_API_KEY=<API_KEY>)")
	endif()

	set(GOOGLE_MAPS_API_KEY ${ARGS_API_KEY} CACHE STRING "Google maps API key")

	message(STATUS "Generating maps file: ${ARGS_INPUT_FILE} -> ${ARGS_OUTPUT_FILE}")

	execute_process(
		COMMAND
		${Python3_EXECUTABLE} ${maps_script} --input ${ARGS_INPUT_FILE} --output ${ARGS_OUTPUT_FILE} --key ${ARGS_API_KEY}
	)

endfunction()
