
# Create the include directory symbolic link
setup_include_dir__ := $(call symlink_include_dir,posixx)

# Symbolic target to add to all
.PHONY: posixx
all += posixx

# pkg-config specification file
posixx-PC-PREFIX := $(prefix)
posixx-PC-NAME := posixx
posixx-PC-DESC := Thin C++ wrapper for POSIX API
posixx-PC-URL := https://spec-dev.integratech.net/redmine/projects/show/posixx
posixx-PC-VERSION := 0.1
posixx-PC-VARS := PREFIX NAME DESC URL VERSION CFLAGS
$L/posixx.pc: PC_VARS := $(posixx-PC-VARS)
$L/posixx.pc: $C/posixx.pc.in $O/$S/posixx.pc-flags
# trigger a rebuild when flags change
setup_flags_files__ := $(call gen_rebuild_flags,$O/$S/posixx.pc-flags,\
		$(call varcat,$(posixx-PC-VARS),posixx-PC-))
posixx: $L/posixx.pc
# install
$I/lib/pkgconfig/posixx.pc: $L/posixx.pc
install += $I/lib/pkgconfig/posixx.pc

# Build the documentation using doxygen
.PHONY: posixx-doc
posixx-doc: $D/posixx/doxygen-stamp
$D/posixx/doxygen-stamp: $C/Doxyfile $(call find_files,.hpp)
doc += posixx-doc

# Install the library's headers
$I/include/posixx/%.hpp: $C/%.hpp
	$(call install_file)
# XXX: we can't use += here, call will be resolved lazily if we do
install := $(install) $(call find_files,.hpp,$I/include/posixx)

