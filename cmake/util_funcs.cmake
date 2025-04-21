function(copy_resources_to_current_bin_dir)
	foreach(DIR ${RESOURCE_DIRS})
		if (EXISTS ${CMAKE_SOURCE_DIR}/${DIR})
			add_custom_command(
				TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_SOURCE_DIR}/${DIR}
				${CMAKE_CURRENT_BINARY_DIR}/${DIR}
			)
		endif()
	endforeach()
endfunction()
