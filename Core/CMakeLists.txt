qt_add_qml_module(Core STATIC
        URI Core       # ↔ QML 端 import AmpSnap.Core 1.0
        VERSION 1.0
        SOURCES
        ColorPointProxy.h
        core.h
        core.cpp
        Spectrometer.cpp
        Spectrometer.h
        UserApplication.h
        SUSI_control.h
        ModbusHandler.cpp
        ModbusHandler.h
        specpredict.cpp
        specpredict.h
        Inference.h
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(Core
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::QuickTimeline
        Qt${QT_VERSION_MAJOR}::ShaderTools
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::SerialBus
#        Qt${QT_VERSION_MAJOR}::OpcUa
        "${CMAKE_CURRENT_SOURCE_DIR}/UserApplication.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/Susi4.lib" 
)
