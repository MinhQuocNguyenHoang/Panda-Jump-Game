include sources/app/screens/Makefile.mk

CFLAGS		+= -I./sources/app
CFLAGS		+= -I./sources/app/Panda_Jump
CPPFLAGS	+= -I./sources/app
CPPFLAGS	+= -I./sources/app/Panda_Jump

VPATH += sources/app
VPATH += sources/app/Panda_Jump

# CPP source files
SOURCES_CPP += sources/app/task_system.cpp
SOURCES_CPP += sources/app/Panda_Jump/PJ_panda_game.cpp
SOURCES_CPP += sources/app/Panda_Jump/PJ_bug_game.cpp
SOURCES_CPP += sources/app/Panda_Jump/PJ_arrow_game.cpp
SOURCES_CPP += sources/app/app.cpp
SOURCES_CPP += sources/app/app_data.cpp
SOURCES_CPP += sources/app/app_non_clear_ram.cpp
SOURCES_CPP += sources/app/app_bsp.cpp
SOURCES_CPP += sources/app/app_modbus_pull.cpp
SOURCES_CPP += sources/app/shell.cpp
SOURCES_CPP += sources/app/task_shell.cpp
SOURCES_CPP += sources/app/task_life.cpp
SOURCES_CPP += sources/app/task_fw.cpp
SOURCES_CPP += sources/app/task_list.cpp
SOURCES_CPP += sources/app/task_display.cpp

ifeq ($(TASK_ZIGBEE_OPTION),-DTASK_ZIGBEE_EN)
SOURCES_CPP += sources/app/task_zigbee.cpp
endif

SOURCES_CPP += sources/app/task_if.cpp
SOURCES_CPP += sources/app/task_rf24_if.cpp
SOURCES_CPP += sources/app/task_uart_if.cpp
SOURCES_CPP += sources/app/task_dbg.cpp
