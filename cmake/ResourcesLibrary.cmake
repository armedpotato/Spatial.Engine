include(CMakeParseArguments)

find_program(MATC_PROGRAM matc HINTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
find_program(CMGEN_PROGRAM cmgen HINTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

function(add_resources_library TARGET_NAME)
    set(oneValueArgs FOLDER)
    set(multiValueArgs MATERIALS MODELS TEXTURES DEPENDEES)
    cmake_parse_arguments(RESOURCE_LIBRARY "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    foreach(MATERIAL_FILE ${RESOURCE_LIBRARY_MATERIALS})
        _add_custom_material_command(${MATERIAL_FILE} ${RESOURCE_LIBRARY_FOLDER})
        list(APPEND MATERIAL_OUTPUT_FILES ${MATERIAL_OUTPUT_FILE})
    endforeach()

    foreach(TEXTURE_FILE ${RESOURCE_LIBRARY_TEXTURES})

        if (${TEXTURE_FILE} MATCHES ".+\.hdr$")
            _add_custom_hdr_texture_command(${TEXTURE_FILE} ${RESOURCE_LIBRARY_FOLDER})
        elseif()
            _add_custom_texture_command(${TEXTURE_FILE} ${RESOURCE_LIBRARY_FOLDER})
        endif()

        list(APPEND TEXTURE_OUTPUT_FILES ${TEXTURE_OUTPUT_FILE})
    endforeach()
    
    add_custom_target(${TARGET_NAME}
        DEPENDS 
            ${MATERIAL_OUTPUT_FILES}
            ${TEXTURE_OUTPUT_FILES}
    )

    foreach(DEPENDEE ${RESOURCE_LIBRARY_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET_NAME})
    endforeach()

endfunction()


function(_add_custom_texture_command TEXTURE_FILE FOLDER)
    get_filename_component(TEXTURE_FILE_NAME ${TEXTURE_FILE} NAME)

    if (FOLDER)
        set(TEXTURE_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${RESOURCE_LIBRARY_FOLDER}/textures")
    else()
        set(TEXTURE_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures")
    endif()

    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}")
    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FILE}" PARENT_SCOPE)

    add_custom_command(
        OUTPUT ${TEXTURE_OUTPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURE_OUTPUT_FOLDER}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE} ${TEXTURE_OUTPUT_FILE} 
        COMMENT "Copying textures: ${TEXTURE_FILE} => ${TEXTURE_OUTPUT_FILE}"
    )
endfunction()

function(_add_custom_hdr_texture_command TEXTURE_FILE FOLDER)
    get_filename_component(TEXTURE_FILE_NAME ${TEXTURE_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" TEXTURE_FILE_NAME ${TEXTURE_FILE_NAME})

    if (FOLDER)
        set(TEXTURE_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${FOLDER}/textures")
    else()
        set(TEXTURE_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures")
    endif()

    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/${TEXTURE_FILE_NAME}_ibl.ktx")
    list(APPEND TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/${TEXTURE_FILE_NAME}_skybox.ktx")
    list(APPEND TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/sh.txt")

    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FILE}" PARENT_SCOPE)

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURE_OUTPUT_FOLDER})

    add_custom_command(
        OUTPUT ${TEXTURE_OUTPUT_FILE}
        COMMAND ${CMGEN_PROGRAM} -q --format=ktx --size=256 --extract-blur=0.1 ${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}
        COMMENT "${CMGEN_PROGRAM} -q --format=ktx --size=256 --extract-blur=0.1 ${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}"
        WORKING_DIRECTORY ${TEXTURE_OUTPUT_FOLDER}
    )
endfunction()

function(_add_custom_material_command MATERIAL_FILE FOLDER)
    get_filename_component(MATERIAL_FILE_NAME ${MATERIAL_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" MATERIAL_FILE_NAME ${MATERIAL_FILE_NAME})

    if (FOLDER)
        set(MATERIAL_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${FOLDER}/materials")
    else()
        set(MATERIAL_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/materials")
    endif()

    set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FOLDER}/${MATERIAL_FILE_NAME}.bin")
    set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FILE}" PARENT_SCOPE)

    add_custom_command(
        OUTPUT ${MATERIAL_OUTPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MATERIAL_OUTPUT_FOLDER}
        COMMAND ${MATC_PROGRAM} -a opengl -o ${MATERIAL_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${MATERIAL_FILE}
        COMMENT "Compiling material: ${MATERIAL_FILE} => ${MATERIAL_OUTPUT_FILE}"
    )
endfunction()
