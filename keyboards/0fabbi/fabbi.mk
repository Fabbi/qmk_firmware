FDEBUG = yes # Master of debug
# OPT_DEFS += -DDEBUG_MATRIX_SCAN_RATE

# RAW_ENABLE = yes

# SSD1306_ENABLE = yes
# TRACKPOINT_ENABLE = yes

# ######################
# Display stuff..
ifeq ($(SSD1306_ENABLE), yes)
  OPT_DEFS += -DSSD1306OLED
  OPT_DEFS += -DSSD1306_USE_BITMAP
  OPT_DEFS += -DSSD1306_DISPLAY_HEIGHT=64
  SRC += $(TOP_DIR)/drivers/avr/ssd1306.c
endif

# Trackpoint
ifeq ($(TRACKPOINT_ENABLE), yes)
  PS2_MOUSE_ENABLE        = yes
  PS2_USE_USART           = yes
endif

# bootmagic.. dunno if needed..
# BOOTMAGIC_ENABLE				= no
