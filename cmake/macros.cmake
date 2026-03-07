function(x8k_add_lib
    name
    sources
    includes
    library_deps
    )
    add_library(${name} ${sources})

    # On windows vcpkg goes out of its way to make its libs the preferred
    # libs, and needs to be explicitly be told not to do that.
    if(WIN32)
        set_target_properties(${name} PROPERTIES VS_GLOBAL_VcpkgEnabled "false")
    endif()

    blender_target_include_dirs(${name} ${includes})

    if(library_deps)
        blender_link_libraries(${name} "${library_deps}")
    endif()

    # works fine without having the includes
    # listed is helpful for IDE's (QtCreator/MSVC)
    blender_source_group("${name}" "${sources}")
    blender_user_header_search_paths("${name}" "${includes}")

    list_assert_duplicates("${sources}")
    list_assert_duplicates("${includes}")
    # Not for system includes because they can resolve to the same path
    # list_assert_duplicates("${includes_sys}")
endfunction()