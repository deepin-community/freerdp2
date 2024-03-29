# FreeRDP: A Remote Desktop Protocol Implementation
# FreeRDP cmake build script
#
# Copyright 2018 David Fort <contact@hardening-consulting.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

define_channel_client("video")

set(${MODULE_PREFIX}_SRCS
	video_main.c
	video_main.h)

include_directories(..)

add_channel_client_library(${MODULE_PREFIX} ${MODULE_NAME} ${CHANNEL_NAME} TRUE "DVCPluginEntry")

if (NOT BUILTIN_CHANNELS)
	set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} freerdp-client)
	set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} rdpgfx-client)
endif()
set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} winpr)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})


if (WITH_DEBUG_SYMBOLS AND MSVC AND NOT BUILTIN_CHANNELS AND BUILD_SHARED_LIBS)
	install(FILES ${CMAKE_BINARY_DIR}/${MODULE_NAME}.pdb DESTINATION ${FREERDP_ADDIN_PATH} COMPONENT symbols)
endif()

set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "Channels/${CHANNEL_NAME}/Client")
