DTOO_FIND_LIBRARY_EXT(
  "OpenFOAMWixingPlane_FOUND" "lnInclude/mixingInterfaceFvPatchField.H" "libmixingPlane.so;libwixingPlane.so" 
  "OpenFOAMWixingPlane_INCLUDE_DIR" "OpenFOAMWixingPlane_LINK_DIRECTORY" "OpenFOAMWixingPlane_LIBRARIES" 
  "OpenFOAMWixingPlane_FIND_COMPONENTS"
  "$ENV{FOAMXDOGBALL_DIR}/of/wixingPlane" 
  "$ENV{FOAM_USER_LIBBIN};$ENV{FOAM_SITE_LIBBIN}"
)