INSTALL(TARGETS otr DESTINATION lib/irssi/modules)
INSTALL(FILES README LICENSE DESTINATION share/doc/${CMAKE_PROJECT_NAME}-${IRSSIOTR_VERSION})

SET(CPACK_GENERATOR DEB)
#SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_SECTION "net")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "irssi")
