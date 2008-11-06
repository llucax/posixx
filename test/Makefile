
LOCAL_CFLAGS = -std=c++98 -Wall -pedantic -Iinclude
LOCAL_LDFLAGS = -lboost_unit_test_framework

TESTPRG = test_suite
TESTCMD = ./$(TESTPRG) --detect_memory_leak=1 --report_level=short \
		#--build_info=yes --log_level=test_suite \
		#| grep -v "Entering test case " | grep -v "Leaving test case"

VALGRIND = valgrind --tool=memcheck --leak-check=yes --db-attach=no \
		--num-callers=24 --leak-resolution=high --track-fds=yes \
		--suppressions=valgrind.suppressions --error-exitcode=1


SOURCES = $(shell find -name '*.cpp')
OBJS = $(SOURCES:.cpp=.o)



ALL_CFLAGS = $(LOCAL_CFLAGS) $(LI_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(LOCAL_LDFLAGS) $(LI_LDFLAGS) $(LDFLAGS)

ifdef RELEASE
ALL_CFLAGS += -O3 -DNDEBUG
endif

ifdef DEBUG
ALL_CFLAGS += -g -DDEBUG
endif

ifdef PROFILE
ALL_CFLAGS += -g -pg -fprofile-arcs -ftest-coverage
endif


ifneq ($(V), 1)
NICE_CXX = @echo "  CXX $@"; $(CXX)
NICE_LINK = @echo "  LINK $@"; $(CXX)
NICE_TEST = @echo "  TEST"; $(TESTCMD) > $(TESTPRG).log 2>&1
NICE_MEMTEST = @echo "  MEMTEST"; $(VALGRIND) --log-file=valgrind.log \
		$(TESTCMD) > valgrind.$(TESTPRG).log 2>&1
NICE_CLEAN = @echo "  CLEAN"; $(RM)
else
NICE_CXX = $(CXX)
NICE_LINK = $(CXX)
NICE_TEST = $(TESTCMD)
NICE_MEMTEST = $(VALGRIND) $(TESTCMD)
NICE_CLEAN = $(RM) -v
endif


default: all

all: $(TESTPRG)


# dependency handling

# XXX the mkdir *has* to be inside the rule, otherwise make will build the
#     dependencies over and over again, because of this:
#     When a file that is included in the Makefile need to be built by itself,
#     the Makefile is re-executed after building the files to include. The
#     problem is, the directory .deps is updated *after* the files are created
#     in it, so when the Makefile re-executes, the files to include are created
#     but one of it's dependencies (the .deps directory) is outdated, so the
#     the files to include are re-built and the Makefile re-executed again,
#     entering in an infinite loop.
.deps/%.deps: %.cpp
	@mkdir -p $(dir $@)
	@set -e; rm -f $@; \
		$(CC) -MM -MG -MP $(CPPFLAGS) $(ALL_CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $<)\1.o $@ : ,g' \
				< $@.$$$$ > $@; \
		rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
-include $(addprefix .deps/,$(OBJS:.o=.deps))
endif


# main build


$(TESTPRG): $(OBJS)
	$(NICE_LINK) $(ALL_LDFLAGS) -o $@ $^

$(OBJS): include

.cpp.o:
	$(NICE_CXX) $(ALL_CFLAGS) -c $< -o $@

include:
	@mkdir -p include
	@ln -fs ../../src include/posixx

test: .test-stamp

.test-stamp: $(TESTPRG)
	$(NICE_TEST)
	@touch $@

memtest: .memtest-stamp

.memtest-stamp: $(TESTPRG)
	$(NICE_MEMTEST)
	@touch $@

clean:
	$(NICE_CLEAN) -r $(TESTPRG) include $(OBJS) .deps valgrind.log \
		*.bb *.bbg *.da *.gcov *.gcda *.gcno gmon.out .*-stamp

.PHONY: default all test memtest clean


