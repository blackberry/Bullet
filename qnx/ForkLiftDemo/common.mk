ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=
NAME=ForkLiftDemo

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PRODUCT_ROOT)/../src \
    $(PRODUCT_ROOT)/../Demos/OpenGL \
    $(QNX_TARGET)/usr/include/freetype2 \
	$(QNX_TARGET)/../target-override/usr/include

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PRODUCT_ROOT)/../Demos/ForkLiftDemo

# Extra library search path for target overrides and patches
EXTRA_LIBVPATH+=$(QNX_TARGET)/../target-override/$(CPUVARDIR)/lib \
	$(QNX_TARGET)/../target-override/$(CPUVARDIR)/usr/lib

# Basic libraries required by most native applications
LIBS+=bps pps screen EGL GLESv1_CM m freetype png gestures

include $(MKFILES_ROOT)/qmacros.mk

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))