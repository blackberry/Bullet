ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

NAME=Bullet

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

CCFLAGS += -DBT_USE_NEON -mfpu=neon

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PRODUCT_ROOT)/../../Bullet/src

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PRODUCT_ROOT)/../../Bullet/src/BulletCollision/BroadphaseCollision \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletCollision/CollisionDispatch \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletCollision/CollisionShapes \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletCollision/Gimpact \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletCollision/NarrowPhaseCollision \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletDynamics/Character \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletDynamics/ConstraintSolver \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletDynamics/Dynamics \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletDynamics/Vehicle \
    $(PRODUCT_ROOT)/../../Bullet/src/BulletSoftBody \
    $(PRODUCT_ROOT)/../../Bullet/src/LinearMath

include $(MKFILES_ROOT)/qmacros.mk

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
