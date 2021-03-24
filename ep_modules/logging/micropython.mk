EP_MODULE_DIR := ../../ep_modules/logging

# Add our source files to the respective variables.
SRC_USERMOD += $(EP_MODULE_DIR)/mp_loggermodule.c
SRC_USERMOD += $(EP_MODULE_DIR)/mp_logger.c
SRC_USERMOD += $(EP_MODULE_DIR)/mp_handler.c
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/loc_c_functions.cpp
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/log_formatter.cpp
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/log_handler.cpp
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/log_logger.cpp
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/log_message.cpp

# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(EP_MODULE_DIR)
CXXFLAGS_USERMOD += -I$(EP_MODULE_DIR) 

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++ 
