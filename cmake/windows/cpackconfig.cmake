# OBS CMake Windows CPack configuration module

include_guard(GLOBAL)

include(cpackconfig_common)

# Add GPLv2 license file to CPack
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/frontend/data/license/gplv2.txt")
set(CPACK_PACKAGE_VERSION "${OBS_VERSION_CANONICAL}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-windows-${CMAKE_VS_PLATFORM_NAME}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
set(CPACK_GENERATOR "NSIS;WIX;ZIP")
set(CPACK_THREADS 0)

include(CPack)
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/frontend/data/obs.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/frontend/data/obs.ico")
set(CPACK_NSIS_DISPLAY_NAME "OBS Studio")
set(CPACK_NSIS_PACKAGE_NAME "OBS Studio")
