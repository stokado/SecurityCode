project("TaskTwo")

add_executable (${PROJECT_NAME} parseedid.h parseedid.c main.c)

set_target_properties (${PROJECT_NAME}
		PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}/"
)