DTOO_FIND_LIBRARY_EXT(
  "OpenFOAMMixingPlane_FOUND" "lnInclude/mixingInterfaceFvPatchField.H" "libmixingPlane.so;libwixingPlane.so" 
  "OpenFOAMMixingPlane_INCLUDE_DIR" "OpenFOAMMixingPlane_LINK_DIRECTORY" "OpenFOAMMixingPlane_LIBRARIES" 
  "OpenFOAMMixingPlane_FIND_COMPONENTS"
  "$ENV{FOAMXDOGBALL_DIR}/of/mixingPlane" 
  "$ENV{FOAM_USER_LIBBIN};$ENV{FOAM_SITE_LIBBIN}"
)