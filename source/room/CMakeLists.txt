set(ROOM_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/RoomManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RoomManager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Room.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Room.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Door.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Door.h"
    PARENT_SCOPE)

source_group("room" FILES ${ROOM_SRCS})
