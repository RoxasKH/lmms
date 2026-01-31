include(BuildPlugin)

include_directories(SYSTEM ${FFTW3F_INCLUDE_DIRS})
link_libraries(${FFTW3F_LIBRARIES})

build_plugin(waver
	Waver.cpp
	Waver.h
	WaverWidgetView.cpp
	WaverWidgetView.h
	WaverSampleMap.cpp
	WaverSampleMap.h
	MOCFILES Waver.h WaverWidgetView.h WaverSampleMap.h
	EMBEDDED_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml" "${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.ttf"
)

target_link_libraries(waver SampleRate::samplerate)
