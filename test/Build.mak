
# Build the test executable
$B/test-posixx: LINKER := $(CXX)
$B/test-posixx: $(call find_objects,cpp)

# Run the test executable (though valgrind if $(VALGRIND) is non-empty)
.PHONY: test-posixx
test-posixx: LDFLAGS += -lboost_unit_test_framework-mt
test-posixx: VALGRIND_SUPP := $C/valgrind.suppressions
test-posixx: $B/test-posixx
	$(call valgrind,$< --detect_memory_leak=1 --detect_fp_exceptions=1 \
			--report_level=$(if $V,no,short))

# Run our unit tests when the "test" goal is built
test += test-posixx

