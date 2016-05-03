
#Lovingly taken from openmw
macro (add_adit_dir dir)
    set (files)
    set (cppfiles)
    foreach (u ${ARGN})

        # Add cpp and h to ADIT_FILES
        file (GLOB ALL "${dir}/${u}.[ch]*")
        foreach (f ${ALL})
            list (APPEND files "${f}")
            list (APPEND ADIT_FILES "${f}")
        endforeach (f)

    endforeach (u)

    source_group ("apps\\adit\\${dir}" FILES ${files})
endmacro (add_adit_dir)

#Lovingly taken from openmw
macro (add_osgb_dir dir)
    set (files)
    set (cppfiles)

    # Add cpp and h to OSGB_FILES
    file (GLOB ALL "${dir}/*.[ch]*")
    foreach (f ${ALL})
        list (APPEND files "${f}")
        list (APPEND OSGB_FILES "${f}")
    endforeach (f)

    source_group ("${dir}" FILES ${files})
endmacro (add_osgb_dir)