#############################################################################
# By installing, copying or using this software you, on behalf of
# yourself or the entity you represent, agree to be bound by the terms
# of the TouchGFX License, which you may download, review, and print
# from www.st.com/st-touchgfx_license.
#
# In addition to any other applicable terms, you may not use the
# software for production purposes, except in connection with the list
# of Dedicated Parts provided here: www.st.com/st-touchgfx_license.
#
# THIS SOFTWARE IS PROVIDED “AS IS” WITHOUT ANY WARRANTY, EXPRESS OR
# IMPLIED.
#
# This file is part of the TouchGFX 4.9.3 distribution.
#
# Copyright (C) 2018 Draupner Graphics A/S <http://www.touchgfx.com>
#############################################################################

# Relative location of the TouchGFX framework from root of application
touchgfx_path := ../../../../../Middlewares/ST/TouchGFX/touchgfx

# Optional additional compiler flags
user_cflags := -DUSE_BPP=24

c_compiler_options_local := -Wno-format-nonliteral -Wwrite-strings

# Settings for image converter output format
alpha_dither := yes
opaque_image_format := RGB888
non_opaque_image_format := ARGB8888

# Settings for image converter screen orientation (empty string = default value, -rotate90 rotates the image 90 degrees)
screen_orientation :=

#Settings for Hardware accelerated text rendering on ST32F4 devices. Must correspond to value of bitsPerPixel for font to have any effect.
#If A4 blitcap is enabled for target specific HAL the fontconverter must generate compliant data format, otherwise resulting in a DMA Controller Configuration Error.
text_data_format := A4
