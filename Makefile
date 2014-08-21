#
# OMNeT++/OMNEST Makefile for Oddci
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common -I/opt/omnetpp-4.4.1/include/platdep -I/usr/include/ -I/usr/include/c++ -I/usr/include/c++/4.6 -I/usr/include/c++/4.6.3 -I/usr/include/x86_64-linux-gnu -I/usr/include/c++/4.6.3/x86_64-linux-gnu -L/usr/lib -L/opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/lib -lpthread -loppcommond
#

# Name of target to be created (-o option)
TARGET = Oddci$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common \
    -I/opt/omnetpp-4.4.1/include/platdep \
    -I/usr/include/ \
    -I/usr/include/c++ \
    -I/usr/include/c++/4.6 \
    -I/usr/include/c++/4.6.3 \
    -I/usr/include/x86_64-linux-gnu \
    -I/usr/include/c++/4.6.3/x86_64-linux-gnu \
    -I. \
    -Icore \
    -Icore/heuristics \
    -Icore/heuristics/estimation \
    -Icore/heuristics/provisioning \
    -Icore/heuristics/selection \
    -Iexceptions \
    -Iexperiments \
    -Iexperiments/statistics \
    -Iexperiments/workloads \
    -Imessages \
    -Ineds \
    -Iresults \
    -Iresults/out \
    -Iutils

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L/usr/lib -L/opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/lib  -lpthread -loppcommond
LIBS += -Wl,-rpath,`abspath /usr/lib` -Wl,-rpath,`abspath /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/lib`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/core/Controller.o \
    $O/core/Cycle.o \
    $O/core/Instance.o \
    $O/core/PNA.o \
    $O/core/Provider.o \
    $O/core/Router.o \
    $O/core/SubCycle.o \
    $O/core/Transmitter.o \
    $O/core/heuristics/estimation/ExploratoryPNAEstimator.o \
    $O/core/heuristics/estimation/InformativePNAEstimator.o \
    $O/core/heuristics/estimation/PNAEstimator.o \
    $O/core/heuristics/provisioning/ProactiveProvisioner.o \
    $O/core/heuristics/provisioning/Provisioner.o \
    $O/core/heuristics/provisioning/ReactiveProvisioner.o \
    $O/core/heuristics/selection/PNASelector.o \
    $O/core/heuristics/selection/RankingSelector.o \
    $O/core/heuristics/selection/TempestiveSelector.o \
    $O/experiments/Experiment.o \
    $O/experiments/ExpInstance.o \
    $O/experiments/oddci_runner.o \
    $O/experiments/statistics/statistic.o \
    $O/experiments/statistics/test.o \
    $O/experiments/workloads/workloadAdapter.o \
    $O/utils/IpGenerator.o \
    $O/utils/OddciUtils.o \
    $O/messages/DieMsg_m.o \
    $O/messages/HeartBeatMsg_m.o \
    $O/messages/IdleProbeMsg_m.o \
    $O/messages/KillMsg_m.o \
    $O/messages/NetPkt_m.o \
    $O/messages/OddciMessage_m.o \
    $O/messages/WaitingMsg_m.o \
    $O/messages/WakeupMsg_m.o \
    $O/messages/WorkMsg_m.o

# Message files
MSGFILES = \
    messages/DieMsg.msg \
    messages/HeartBeatMsg.msg \
    messages/IdleProbeMsg.msg \
    messages/KillMsg.msg \
    messages/NetPkt.msg \
    messages/OddciMessage.msg \
    messages/WaitingMsg.msg \
    messages/WakeupMsg.msg \
    messages/WorkMsg.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f Oddci Oddci.exe libOddci.so libOddci.a libOddci.dll libOddci.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f core/*_m.cc core/*_m.h
	$(Q)-rm -f core/heuristics/*_m.cc core/heuristics/*_m.h
	$(Q)-rm -f core/heuristics/estimation/*_m.cc core/heuristics/estimation/*_m.h
	$(Q)-rm -f core/heuristics/provisioning/*_m.cc core/heuristics/provisioning/*_m.h
	$(Q)-rm -f core/heuristics/selection/*_m.cc core/heuristics/selection/*_m.h
	$(Q)-rm -f exceptions/*_m.cc exceptions/*_m.h
	$(Q)-rm -f experiments/*_m.cc experiments/*_m.h
	$(Q)-rm -f experiments/statistics/*_m.cc experiments/statistics/*_m.h
	$(Q)-rm -f experiments/workloads/*_m.cc experiments/workloads/*_m.h
	$(Q)-rm -f messages/*_m.cc messages/*_m.h
	$(Q)-rm -f neds/*_m.cc neds/*_m.h
	$(Q)-rm -f results/*_m.cc results/*_m.h
	$(Q)-rm -f results/out/*_m.cc results/out/*_m.h
	$(Q)-rm -f utils/*_m.cc utils/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc core/*.cc core/heuristics/*.cc core/heuristics/estimation/*.cc core/heuristics/provisioning/*.cc core/heuristics/selection/*.cc exceptions/*.cc experiments/*.cc experiments/statistics/*.cc experiments/workloads/*.cc messages/*.cc neds/*.cc results/*.cc results/out/*.cc utils/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/core/Controller.o: core/Controller.cc \
  messages/DieMsg_m.h \
  messages/WakeupMsg_m.h \
  /usr/include//bits/pthreadtypes.h \
  messages/HeartBeatMsg_m.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  messages/KillMsg_m.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  messages/WaitingMsg_m.h \
  /usr/include//ctype.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//gnu/stubs-64.h \
  core/heuristics/estimation/ExploratoryPNAEstimator.h \
  /usr/include//stdio.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/unitconversion.h \
  /usr/include//bits/sys_errlist.h \
  messages/IdleProbeMsg_m.h \
  messages/MessageType.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//wctype.h \
  messages/OddciMessage_m.h \
  /usr/include//bits/sigset.h \
  core/heuristics/selection/RankingSelector.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  core/heuristics/selection/PNASelector.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  exceptions/OddciException.h \
  /usr/include//pthread.h \
  core/Cycle.h \
  core/heuristics/provisioning/ReactiveProvisioner.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  core/heuristics/provisioning/ProactiveProvisioner.h \
  /usr/include//bits/string.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/commondefs.h \
  messages/WorkMsg_m.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/time.h \
  /usr/include//bits/wchar2.h \
  /usr/include//alloca.h \
  /usr/include//bits/waitstatus.h \
  core/heuristics/selection/TempestiveSelector.h \
  /usr/include//bits/stdio.h \
  core/Instance.h \
  core/heuristics/estimation/InformativePNAEstimator.h \
  core/Controller.h \
  core/PNA.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /opt/omnetpp-4.4.1/include/platdep/platdefs.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/exception.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/core/Cycle.o: core/Cycle.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/Instance.o: core/Instance.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/Cycle.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/string.h \
  /usr/include//bits/time.h \
  /usr/include//alloca.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/PNA.o: core/PNA.cc \
  messages/DieMsg_m.h \
  messages/WakeupMsg_m.h \
  /usr/include//bits/pthreadtypes.h \
  messages/HeartBeatMsg_m.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  messages/KillMsg_m.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  messages/WaitingMsg_m.h \
  /usr/include//ctype.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  core/heuristics/estimation/ExploratoryPNAEstimator.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/unitconversion.h \
  /usr/include//bits/sys_errlist.h \
  messages/IdleProbeMsg_m.h \
  messages/MessageType.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//wctype.h \
  messages/OddciMessage_m.h \
  /usr/include//bits/sigset.h \
  core/heuristics/selection/RankingSelector.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  core/heuristics/selection/PNASelector.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/commondefs.h \
  messages/WorkMsg_m.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/wchar2.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  core/heuristics/selection/TempestiveSelector.h \
  core/Instance.h \
  core/Controller.h \
  core/heuristics/estimation/InformativePNAEstimator.h \
  core/PNA.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /opt/omnetpp-4.4.1/include/platdep/platdefs.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /opt/omnetpp-4.4.1/samples/omnetpp-4.4.1/src/common/exception.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/core/Provider.o: core/Provider.cc \
  messages/WakeupMsg_m.h \
  messages/MessageType.h \
  core/Provider.h \
  messages/OddciMessage_m.h \
  messages/NetPkt_m.h
$O/core/Router.o: core/Router.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/core/SubCycle.o: core/SubCycle.cc \
  core/SubCycle.h \
  core/Cycle.h
$O/core/Transmitter.o: core/Transmitter.cc \
  messages/DieMsg_m.h \
  messages/WakeupMsg_m.h \
  /usr/include//bits/pthreadtypes.h \
  messages/HeartBeatMsg_m.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  messages/KillMsg_m.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  core/Transmitter.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  messages/WaitingMsg_m.h \
  /usr/include//ctype.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  core/heuristics/estimation/ExploratoryPNAEstimator.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  messages/MessageType.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  messages/OddciMessage_m.h \
  core/heuristics/selection/RankingSelector.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  core/heuristics/selection/PNASelector.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  core/Cycle.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  messages/WorkMsg_m.h \
  /usr/include//bits/time.h \
  /usr/include//alloca.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/waitstatus.h \
  core/heuristics/selection/TempestiveSelector.h \
  /usr/include//bits/stdio.h \
  core/Instance.h \
  core/heuristics/estimation/InformativePNAEstimator.h \
  core/Controller.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/core/heuristics/estimation/ExploratoryPNAEstimator.o: core/heuristics/estimation/ExploratoryPNAEstimator.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  core/heuristics/estimation/ExploratoryPNAEstimator.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/estimation/InformativePNAEstimator.o: core/heuristics/estimation/InformativePNAEstimator.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  core/heuristics/estimation/InformativePNAEstimator.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/estimation/PNAEstimator.o: core/heuristics/estimation/PNAEstimator.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/Cycle.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/string.h \
  /usr/include//bits/time.h \
  /usr/include//alloca.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/provisioning/ProactiveProvisioner.o: core/heuristics/provisioning/ProactiveProvisioner.cc \
  /usr/include//bits/pthreadtypes.h \
  messages/WakeupMsg_m.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//gnu/stubs-64.h \
  messages/MessageType.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  messages/OddciMessage_m.h \
  /usr/include//bits/sigset.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/heuristics/selection/PNASelector.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  core/heuristics/provisioning/ProactiveProvisioner.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  core/Controller.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/provisioning/Provisioner.o: core/heuristics/provisioning/Provisioner.cc \
  messages/WakeupMsg_m.h \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//gnu/stubs-64.h \
  messages/MessageType.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  messages/OddciMessage_m.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/heuristics/selection/PNASelector.h \
  core/Cycle.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/string.h \
  /usr/include//bits/time.h \
  /usr/include//alloca.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  core/Controller.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/provisioning/ReactiveProvisioner.o: core/heuristics/provisioning/ReactiveProvisioner.cc \
  /usr/include//bits/pthreadtypes.h \
  messages/WakeupMsg_m.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  core/heuristics/estimation/PNAEstimator.h \
  /usr/include//endian.h \
  core/heuristics/provisioning/Provisioner.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  messages/MessageType.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//wctype.h \
  messages/OddciMessage_m.h \
  /usr/include//bits/sigset.h \
  messages/NetPkt_m.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  core/heuristics/selection/PNASelector.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  core/Cycle.h \
  core/heuristics/provisioning/ReactiveProvisioner.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  core/Instance.h \
  core/Controller.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  utils/OddciUtils.h \
  /usr/include//bits/select.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/core/heuristics/selection/PNASelector.o: core/heuristics/selection/PNASelector.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/heuristics/selection/PNASelector.h \
  core/Cycle.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/string.h \
  /usr/include//bits/time.h \
  /usr/include//alloca.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/selection/RankingSelector.o: core/heuristics/selection/RankingSelector.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  core/heuristics/selection/RankingSelector.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/heuristics/selection/PNASelector.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/core/heuristics/selection/TempestiveSelector.o: core/heuristics/selection/TempestiveSelector.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//sys/select.h \
  /usr/include//endian.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//features.h \
  core/SubCycle.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  core/heuristics/selection/PNASelector.h \
  core/Cycle.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  core/heuristics/selection/TempestiveSelector.h \
  core/Instance.h \
  /usr/include//time.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h
$O/experiments/Experiment.o: experiments/Experiment.cc \
  experiments/Experiment.h \
  experiments/ExpInstance.h
$O/experiments/ExpInstance.o: experiments/ExpInstance.cc \
  experiments/ExpInstance.h
$O/experiments/oddci_runner.o: experiments/oddci_runner.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  experiments/rapidxml.hpp \
  /usr/include//sys/sysmacros.h \
  experiments/Experiment.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  experiments/ExpInstance.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/wchar2.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/experiments/statistics/statistic.o: experiments/statistics/statistic.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  experiments/statistics/statistic.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/time.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//xlocale.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/experiments/statistics/test.o: experiments/statistics/test.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/byteswap.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  experiments/statistics/statistic.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//alloca.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/experiments/workloads/workloadAdapter.o: experiments/workloads/workloadAdapter.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/mathcalls.h \
  /usr/include//bits/nan.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//bits/mathinline.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//bits/inf.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/huge_val.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//math.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/mathdef.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  experiments/ExpInstance.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/wchar2.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//bits/huge_vall.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /usr/include//gconv.h \
  /usr/include//bits/huge_valf.h \
  /usr/include//getopt.h
$O/utils/IpGenerator.o: utils/IpGenerator.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//bits/wchar2.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
$O/utils/OddciUtils.o: utils/OddciUtils.cc \
  /usr/include//bits/pthreadtypes.h \
  /usr/include//bits/wordsize.h \
  /usr/include//stdlib.h \
  /usr/include//bits/string3.h \
  /usr/include//bits/stdio2.h \
  /usr/include//bits/wchar-ldbl.h \
  /usr/include//bits/byteswap.h \
  /usr/include//bits/typesizes.h \
  /usr/include//sys/cdefs.h \
  /usr/include//sys/types.h \
  /usr/include//gnu/stubs.h \
  /usr/include//bits/libio-ldbl.h \
  /usr/include//sys/select.h \
  /usr/include//bits/wchar.h \
  /usr/include//wchar.h \
  /usr/include//ctype.h \
  /usr/include//endian.h \
  /usr/include//bits/libc-lock.h \
  /usr/include//gnu/stubs-64.h \
  /usr/include//stdio.h \
  /usr/include//bits/sys_errlist.h \
  /usr/include//bits/stdio-ldbl.h \
  /usr/include//bits/stdlib-ldbl.h \
  /usr/include//bits/waitflags.h \
  /usr/include//sys/sysmacros.h \
  /usr/include//libio.h \
  /usr/include//bits/setjmp.h \
  /usr/include//features.h \
  /usr/include//wctype.h \
  /usr/include//bits/sigset.h \
  /usr/include//bits/string2.h \
  /usr/include//bits/types.h \
  /usr/include//bits/stdio_lim.h \
  /usr/include//bits/sched.h \
  /usr/include//_G_config.h \
  /usr/include//pthread.h \
  /usr/include//bits/stdlib.h \
  /usr/include//bits/endian.h \
  /usr/include//bits/string.h \
  /usr/include//bits/stdio-lock.h \
  /usr/include//alloca.h \
  /usr/include//bits/time.h \
  /usr/include//bits/wchar2.h \
  /usr/include//bits/waitstatus.h \
  /usr/include//bits/stdio.h \
  /usr/include//time.h \
  /usr/include//sched.h \
  /usr/include//string.h \
  /usr/include//xlocale.h \
  /usr/include//bits/select.h \
  utils/OddciUtils.h \
  /usr/include//gconv.h \
  /usr/include//getopt.h
