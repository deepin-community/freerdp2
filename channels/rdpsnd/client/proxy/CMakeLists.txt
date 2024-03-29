# FreeRDP: A Remote Desktop Protocol Implementation
# FreeRDP cmake build script
#
# Copyright 2019 Armin Novak <armin.novak@thincast.com>
# Copyright 2019 Thincast Technologies GmbH
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

define_channel_client_subsystem("rdpsnd" "proxy" "")

set(${MODULE_PREFIX}_SRCS
	rdpsnd_proxy.c)

include_directories(..)

add_channel_client_subsystem_library(${MODULE_PREFIX} ${MODULE_NAME} ${CHANNEL_NAME} "" TRUE "")

if (NOT BUILTIN_CHANNELS)
	list(APPEND ${MODULE_PREFIX}_LIBS freerdp-client)
endif()
list(APPEND ${MODULE_PREFIX}_LIBS freerdp)
list(APPEND ${MODULE_PREFIX}_LIBS winpr)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "Channels/${CHANNEL_NAME}/Client/proxy")
