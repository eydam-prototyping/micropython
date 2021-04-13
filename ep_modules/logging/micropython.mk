EP_MODULE_DIR := ../../ep_modules/logging

#MICROPY_VFS = 1
#MICROPY_VFS_FAT = 1
# MICROPY_VFS_LFS2 = 1

# Add our source files to the respective variables.
SRC_USERMOD += $(EP_MODULE_DIR)/mp_module_logger.c
SRC_USERMOD += $(EP_MODULE_DIR)/mp_class_logger.c
SRC_USERMOD += $(EP_MODULE_DIR)/mp_class_formatter.c
SRC_USERMOD += $(EP_MODULE_DIR)/mp_class_handler.c
SRC_USERMOD_CXX += $(EP_MODULE_DIR)/c_funcs_logger.cpp

# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(EP_MODULE_DIR)
CXXFLAGS_USERMOD += -I$(EP_MODULE_DIR) 
CXXFLAGS_USERMOD += -I$(BOARD_DIR) -Wno-error=cpp -Wno-error=sign-compare

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++ 
