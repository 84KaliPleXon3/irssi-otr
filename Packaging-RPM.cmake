INSTALL(TARGETS otr DESTINATION lib/irssi/modules)
INSTALL(FILES README DESTINATION share/doc/${CMAKE_PROJECT_NAME}-${IRSSIOTR_VERSION})

SET(CPACK_GENERATOR RPM)
SET(CPACK_RPM_PACKAGE_DEBUG)
SET(CPACK_RPM_PACKAGE_LICENSE "GPLv2")
SET(CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})

EXECUTE_PROCESS(COMMAND bash -c
  "ARCH=`which arch` && $ARCH | tr -d '\n'"
  OUTPUT_VARIABLE CPACK_RPM_PACKAGE_ARCHITECTURE)

#IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
#  SET(CPACK_RPM_PACKAGE_ARCHITECTURE i386)
#ELSE
#  SET(CPACK_RPM_PACKAGE_ARCHITECTURE i386)
#ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

SET(CPACK_RPM_PACKAGE_RELEASE 1)
SET(CPACK_RPM_PACKAGE_GROUP "unknown")
SET(CPACK_RPM_FILE_NAME
  ${CPACK_PACKAGE_FILE_NAME}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE})
