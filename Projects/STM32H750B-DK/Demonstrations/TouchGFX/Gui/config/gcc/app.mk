###############################################################################
# This file is part of the TouchGFX 4.9.0rc1 distribution.
# Copyright (C) 2017 Draupner Graphics A/S <http://www.touchgfx.com>.
###############################################################################
# This is licensed software. Any use hereof is restricted by and subject to 
# the applicable license terms. For further information see "About/Legal
# Notice" in TouchGFX Designer or in your TouchGFX installation directory.
###############################################################################

# Relative location of the TouchGFX framework from root of application
touchgfx_path := ../../../../../Middlewares/ST/TouchGFX/touchgfx

# Location of the TouchGFX Environment
touchgfx_env := D:/TouchGFX/4.12.3/env
# Optional additional compiler flags
user_cflags := -DUSE_BPP=24
opaque_image_format := RGB888
non_opaque_image_format := ARGB8888
dither_algorithm := 2
alpha_dither := yes

text_converter_options := remap A4 BPP24
