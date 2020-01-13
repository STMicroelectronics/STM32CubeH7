
# Relative location of the TouchGFX framework from root of application
touchgfx_path := ../Middlewares/ST/TouchGFX/touchgfx

# Location of the TouchGFX Environment
touchgfx_env := D:/TouchGFX/4.12.0/env
# Optional additional compiler flags
user_cflags := -DUSE_BPP=24 -Wno-format-nonliteral -Wno-shift-negative-value -DUSE_SDCARD -DUSE_USB_HS
text_converter_options := remap A4 BPP24
