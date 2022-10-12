
set(pkgdatadir "${CMAKE_INSTALL_FULL_PKGDATADIR}")
configure_file("lomiri-app-test.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/lomiri-app-test.desktop" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lomiri-app-test.desktop" DESTINATION "${CMAKE_INSTALL_DATADIR}/applications/")

install(FILES "lomiri-app-test.qml" DESTINATION "${CMAKE_INSTALL_FULL_PKGDATADIR}")

install(FILES "lomiri-app-test.svg" DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps")

