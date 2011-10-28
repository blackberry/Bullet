ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

NAME=BulletWorldImporter

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PRODUCT_ROOT)/../src

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PRODUCT_ROOT)/../Extras/Serialize/BulletWorldImporter

include $(MKFILES_ROOT)/qmacros.mk

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))