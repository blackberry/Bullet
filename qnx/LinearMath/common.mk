ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

NAME=LinearMath

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PRODUCT_ROOT)/../src

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PRODUCT_ROOT)/../src/LinearMath

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))