cmake_minimum_required(VERSION 2.6)

project ( qsqldesigner )

set ( SOURCES
     ./src/main.cpp
     ./src/columndialog.cpp
     ./src/mainwindow.cpp
     ./src/tabledialog.cpp
     ./src/widgets/mainview.cpp
     ./src/models/modelmanager.cpp
     ./src/models/table.cpp
     ./src/models/column.cpp
     ./src/models/datatypes.cpp
)

set ( MOC_HEADERS
     ./src/columndialog.h
     ./src/mainwindow.h
     ./src/tabledialog.h
     ./src/widgets/mainview.h
     ./src/models/modelmanager.h
     ./src/models/table.h
     ./src/models/column.h
     ./src/models/datatypes.h
)

set ( UIS
     ./src/ui/columndialog.ui
     ./src/ui/mainwindow.ui
     ./src/ui/tabledialog.ui
)
  
set ( RESOURCES
     ./resources/main.qrc
)

#set ( TRANSLATIONS
#     ./resources/translations/*.ts
#)

set ( QT_USE_QTSQL TRUE )
set ( QT_USE_QTSVG TRUE )
set ( QT_USE_QTXML TRUE )

add_subdirectory (./src/widgets)

find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )


QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )
  
#option (UPDATE_TRANSLATIONS "Update source translation *.ts files (WARNING: make clean will delete the source .ts files! Danger!)")
#if (UPDATE_TRANSLATIONS)
#  QT4_CREATE_TRANSLATION(QM_FILES ${TRANSLATIONS})
#else (UPDATE_TRANSLATIONS)
#  QT4_ADD_TRANSLATION(QM_FILES ${TRANSLATIONS})
#endif (UPDATE_TRANSLATIONS) 

#add_custom_target (translations_target DEPENDS ${QM_FILES})

add_executable( qsqldesigner WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} ${QM_FILES} )
target_link_libraries( qsqldesigner ${QT_LIBRARIES} )

#install(FILES ${QM_FILES} DESTINATION
#${CMAKE_INSTALL_PREFIX}/translations)
