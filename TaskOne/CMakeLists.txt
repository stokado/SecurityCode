project("TaskOne")

add_executable (${PROJECT_NAME} main.c "parsejson.c" "parsejson.h")

set_target_properties (${PROJECT_NAME}
		PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}/"
)