
add_osgb_dir(osgbCollision)

add_osgb_dir(osgbDynamics)

add_osgb_dir(osgbInteraction)


# Set Properties->General->Configuration Type to Application(.exe)
# Creates app.exe with the listed sources (main.cxx)
# Adds sources to the Solution Explorer
add_library (osgBullet ${OSGB_FILES} )

target_include_directories(osgBullet PRIVATE .)

set_target_properties(osgBullet PROPERTIES LINKER_LANGUAGE CXX)