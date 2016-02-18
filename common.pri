Release:DESTDIR = ..\bin\release
Debug:DESTDIR = ..\bin\debug

LIBS += -L"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64" -lopengl32 \
		-L"..\3rd_party\lib\x64" -lassimp -lglew32s

QMAKE_POST_LINK += $$QMAKE_COPY ..\\3rd_party\\bin\\x64\\Assimp64.dll $$DESTDIR\\ $$escape_expand(\\n\\t)

CONFIG(debug, debug|release) {
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Cored.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Guid.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Widgetsd.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5OpenGLd.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
} else:CONFIG(release, debug|release) {
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Core.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Guid.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5Widgets.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($$[QT_INSTALL_BINS]/Qt5OpenGL.dll) $$DESTDIR\\ $$escape_expand(\\n\\t)
}


DEFINES += GLEW_STATIC
INCLUDEPATH += ../3rd_party/include/
INSTALLS += shared
