set(SRCS
    "main.cpp"
    "VogueGame.h"
    "VogueGame.cpp"
    "VogueWindow.h"
    "VogueWindow.cpp"
    "VogueInput.cpp"
    "VogueControls.cpp"
    "VogueCamera.cpp"
    "VogueRender.cpp"
    "VogueUpdate.cpp"
    "VogueGUI.cpp"
    "VogueSettings.h"
    "VogueSettings.cpp")

add_subdirectory(utils)
add_subdirectory(freetype)
add_subdirectory(glew)
add_subdirectory(Maths)
add_subdirectory(Renderer)
add_subdirectory(glm)
add_subdirectory(gui)
add_subdirectory(lua)
add_subdirectory(selene)
add_subdirectory(ini)
add_subdirectory(simplex)
add_subdirectory(tinythread)
add_subdirectory(libnoise)
add_subdirectory(room)
add_subdirectory(Player)

source_group("source" FILES ${SRCS})
source_group("source\\utils" FILES ${UTIL_SRCS})
source_group("source\\freetype" FILES ${FREETYPE_SRCS})
source_group("source\\glew\\src" FILES ${GLEW_SRCS})
source_group("source\\glew\\include\\GL" FILES ${GLEW_HEADERS})
source_group("source\\maths" FILES ${MATHS_SRCS})
source_group("source\\renderer" FILES ${RENDERER_SRCS})
source_group("source\\gui" FILES ${GUI_SRCS})
source_group("source\\glm" FILES ${GLM_SRCS})
source_group("source\\glm\\detail" FILES ${GLM_DETAIL_SRCS})
source_group("source\\glm\\gtc" FILES ${GLM_GTC_SRCS})
source_group("source\\glm\\gtx" FILES ${GLM_GTX_SRCS})
source_group("source\\lua" FILES ${LUA_SRCS})
source_group("source\\selene" FILES ${SELENE_HEADERS})
source_group("source\\selene\\selene" FILES ${SELENE_SRCS})
source_group("source\\ini" FILES ${INI_SRCS})
source_group("source\\simplex" FILES ${SIMPLEX_SRCS})
source_group("source\\tinythread" FILES ${TINYTHREAD_SRCS})
source_group("source\\libnoise" FILES ${LIBNOISE_SRCS})
source_group("source\\room" FILES ${ROOM_SRCS})
source_group("source\\Player" FILES ${PLAYER_SRCS})

add_executable(Vogue
               ${SRCS}
               ${UTIL_SRCS}
               ${FREETYPE_SRCS}
               ${GLEW_SRCS}
               ${GLEW_HEADERS}
               ${MATHS_SRCS}
               ${RENDERER_SRCS}
			   ${GUI_SRCS}
               ${GLM_SRCS}
               ${GLM_DETAIL_SRCS}
               ${GLM_GTC_SRCS}
               ${GLM_GTX_SRCS}
               ${LUA_SRCS}
               ${SELENE_HEADERS}
               ${SELENE_SRCS}
               ${INI_SRCS}
               ${SIMPLEX_SRCS}
               ${TINYTHREAD_SRCS}
               ${LIBNOISE_SRCS}
               ${ROOM_SRCS}
               ${PLAYER_SRCS})

include_directories(".")			   
include_directories("glfw\\include")
include_directories("glew\\include")
include_directories("freetype\\include")
include_directories("lua")
include_directories("selene")
include_directories("libnoise")

if(MSVC)
target_link_libraries(Vogue "opengl32.lib")
target_link_libraries(Vogue "winmm.lib")
elseif(UNIX)
target_link_libraries(Vogue "GL")
target_link_libraries(Vogue "GLU")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/freetype/libs/linux/libfreetype261_64.a")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/glfw/libs/linux/d/libglfw3_64.a")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/libnoise/libs/linux/libnoise_64.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/freetype/libs/linux/libfreetype261_64.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/glfw/libs/linux/r/libglfw3_64.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/libnoise/libs/linux/libnoise_64.a")
	else()
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/glfw/libs/linux/d/libglfw3.a")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/freetype/libs/linux/libfreetype261.a")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}/libnoise/libs/linux/libnoise.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/glfw/libs/linux/r/libglfw3.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/freetype/libs/linux/libfreetype261.a")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}/libnoise/libs/linux/libnoise.a")
	endif()
target_link_libraries(Vogue "X11")
target_link_libraries(Vogue "Xrandr")
target_link_libraries(Vogue "Xi")
target_link_libraries(Vogue "Xxf86vm")
target_link_libraries(Vogue "Xcursor")
target_link_libraries(Vogue "Xinerama")
target_link_libraries(Vogue "pthread")
target_link_libraries(Vogue "dl")
endif()

if(MSVC11)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2012\\d\\glfw3_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2012\\freetype261d_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2012\\noise64_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2012\\r\\glfw3_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2012\\freetype261_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2012\\noise64.lib")
	else()
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2012\\d\\glfw3.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2012\\freetype261d.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2012\\noise_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2012\\r\\glfw3.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2012\\freetype261.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2012\\noise.lib")
	endif()
endif(MSVC11)
if(MSVC12)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2013\\d\\glfw3_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2013\\freetype261d_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2013\\noise64_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2013\\r\\glfw3_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2013\\freetype261_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2013\\noise64.lib")
	else()
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2013\\d\\glfw3.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2013\\freetype261d.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2013\\noise_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2013\\r\\glfw3.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2013\\freetype261.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2013\\noise.lib")
	endif()
endif(MSVC12)
if(MSVC14)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2015\\d\\glfw3_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2015\\freetype261d_64.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2015\\noise64_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2015\\r\\glfw3_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2015\\freetype261_64.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2015\\noise64.lib")
	else()
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2015\\d\\glfw3.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2015\\freetype261d.lib")
		target_link_libraries(Vogue debug "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2015\\noise_d.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\glfw\\libs\\2015\\r\\glfw3.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\freetype\\libs\\2015\\freetype261.lib")
		target_link_libraries(Vogue optimized "${CMAKE_CURRENT_SOURCE_DIR}\\libnoise\\libs\\2015\\noise.lib")
	endif()
endif(MSVC14)

if(MSVC)
	set_target_properties(Vogue PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../")
	set_target_properties(Vogue PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "../../")
	set_target_properties(Vogue PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "../../")
	
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif(MSVC)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
