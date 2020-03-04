#############################################################################
# By installing, copying or using this software you, on behalf of
# yourself or the entity you represent, agree to be bound by the terms
# of the ST-TouchGFX License, which you may download, review, and print
# from www.st.com/st-touchgfx_license.
#
# In addition to any other applicable terms, you may not use the
# software for production purposes, except in connection with the list
# of Dedicated Parts provided here: www.st.com/st-touchgfx_license.
#
# THIS SOFTWARE IS PROVIDED “AS IS” WITHOUT ANY WARRANTY, EXPRESS OR
# IMPLIED.
#
# This file is part of the ST-TouchGFX 4.9.4 distribution.
#
# Copyright (C) 2018 Draupner Graphics A/S <http://www.touchgfx.com>
#############################################################################

# Check that the TouchGFX environment has been installed
ifeq ($(OS),Windows_NT)
ifeq (,$(wildcard C:/touchgfx-env))
$(error Please install the TouchGFX enviroment)
endif
endif

# Only set alpha_dither if not already set by project
alpha_dither ?= no
dither_algorith ?= 2
remap_identical_texts ?= yes

ifeq ($(MAKECMDGOALS),_all_)
GOAL:=build
endif
ifeq ($(MAKECMDGOALS),_assets_)
GOAL:=build
endif

# write image filenames to a temp file used by the image converter. This is
# necessary to avoid exceeding 32Kb limit in a mingw command line for large
# touchgfx projects
ifeq ($(GOAL),build)
$(shell mkdir -p $(object_output_path))
ifeq ($(MAKE_VERSION),4.1)
$(file >$(object_output_path)/image_files.txt) $(foreach F,$(imageconvert_image_files),$(file >>$(object_output_path)/image_files.txt,$F))
else
$(shell echo "$(imageconvert_image_files)">$(object_output_path)/image_files.txt)
endif
endif

$(asset_images_output)/include/BitmapDatabase.hpp $(asset_images_output)/src/BitmapDatabase.cpp: $(imageconvert_cpp_files) $(imageconvert_image_files) $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $(asset_images_output)/include/BitmapDatabase.hpp
	@echo Generating $(asset_images_output)/src/BitmapDatabase.cpp
	@$(imageconvert_executable) -header -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $(object_output_path)/image_files.txt
	@rm -f $(object_output_path)/image_files.txt
	@mkdir -p $(asset_images_output)/include
	@mkdir -p $(asset_images_output)/src
	@mv BitmapDatabase.hpp $(asset_images_output)/include/BitmapDatabase.hpp
	@mv BitmapDatabase.cpp $(asset_images_output)/src/BitmapDatabase.cpp


$(asset_images_output)/src/%.0.cpp: $(asset_images_input)/%.0.bmp $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)

$(asset_images_output)/src/%.90.cpp: $(asset_images_input)/%.90.bmp $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate90

$(asset_images_output)/src/%.180.cpp: $(asset_images_input)/%.180.bmp $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate180

$(asset_images_output)/src/%.270.cpp: $(asset_images_input)/%.270.bmp $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate270

$(asset_images_output)/src/%.cpp: $(asset_images_input)/%.bmp $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)


$(asset_images_output)/src/%.0.cpp: $(asset_images_input)/%.0.BMP $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)

$(asset_images_output)/src/%.90.cpp: $(asset_images_input)/%.90.BMP $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate90

$(asset_images_output)/src/%.180.cpp: $(asset_images_input)/%.180.BMP $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate180

$(asset_images_output)/src/%.270.cpp: $(asset_images_input)/%.270.BMP $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate270

$(asset_images_output)/src/%.cpp: $(asset_images_input)/%.BMP $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)


$(asset_images_output)/src/%.0.cpp: $(asset_images_input)/%.0.png $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)

$(asset_images_output)/src/%.90.cpp: $(asset_images_input)/%.90.png $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate90

$(asset_images_output)/src/%.180.cpp: $(asset_images_input)/%.180.png $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate180

$(asset_images_output)/src/%.270.cpp: $(asset_images_input)/%.270.png $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate270

$(asset_images_output)/src/%.cpp: $(asset_images_input)/%.png $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)


$(asset_images_output)/src/%.0.cpp: $(asset_images_input)/%.0.PNG $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)

$(asset_images_output)/src/%.90.cpp: $(asset_images_input)/%.90.PNG $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate90

$(asset_images_output)/src/%.180.cpp: $(asset_images_input)/%.180.PNG $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate180

$(asset_images_output)/src/%.270.cpp: $(asset_images_input)/%.270.PNG $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation) -rotate270

$(asset_images_output)/src/%.cpp: $(asset_images_input)/%.PNG $(application_path)/config/gcc/app.mk $(imageconvert_executable)
	@echo Generating $@ from $<
	@mkdir -p $(@D)
	@$(imageconvert_executable) -dither $(dither_algorithm) -alpha_dither $(alpha_dither) -opaque_image_format $(opaque_image_format) -non_opaque_image_format $(non_opaque_image_format) -f $< -o $@ $(screen_orientation)


$(asset_texts_output)/include/texts/TextKeysAndLanguages.hpp: $(text_database) $(textconvert_executable) $(fontconvert_executable) $(fontconvert_font_files)
	@rm -f $(asset_fonts_output)/src/*
	@rm -f $(asset_fonts_output)/include/fonts/*
	@rm -f $(asset_fonts_output)/UnicodeList*.txt
	@rm -f $(asset_fonts_output)/CharSizes*.csv
	@mkdir -p $(asset_texts_output)/include/texts
	@ruby -C$(textconvert_script_path) main.rb $(abspath $(text_database)) $(abspath $(fontconvert_executable)) $(abspath $(asset_fonts_output)) $(abspath $(asset_texts_output)) $(abspath $(asset_fonts_input)) $(abspath ./) $(remap_identical_texts) $(text_data_format)
