#-----------------------------------------------------------------------------
#
#  TSDuck - The MPEG Transport Stream Toolkit
#  Copyright (c) 2005-2019, Thierry Lelegard
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
#  THE POSSIBILITY OF SUCH DAMAGE.
#
#-----------------------------------------------------------------------------
#
#  Makefile for unitary tests.
#
#-----------------------------------------------------------------------------

include ../../Makefile.tsduck

default: execs $(OBJDIR)/setenv.sh
	@true

.PHONY: execs
execs: $(OBJDIR)/utest $(OBJDIR)/utest_static

LDLIBS := -lcppunit $(LDLIBS)

# Build two versions of the test executable.
# 1) Using shared object. Skip the module which create static references.
$(OBJDIR)/utest: $(subst $(OBJDIR)/dependenciesForStaticLib.o,,$(OBJS)) $(LIBTSDUCKDIR)/$(OBJDIR)/$(SHARED_LIBTSDUCK)

# 2) Using static library. Skipt plugin tests since they use the shared object.
$(OBJDIR)/utest_static: $(subst $(OBJDIR)/utestPlugin.o,,$(OBJS)) $(LIBTSDUCKDIR)/$(OBJDIR)/$(STATIC_LIBTSDUCK)
	@echo '  [LD] $@'; \
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# A script to create the appropriate execution environment.
$(OBJDIR)/setenv.sh: Makefile
	echo '[[ ":$$PATH:" != *:$(realpath $(OBJDIR)):* ]] && export PATH="$(realpath $(OBJDIR)):$$PATH"' >$@
	echo 'export LD_LIBRARY_PATH="$(realpath $(LIBTSDUCKDIR)/$(OBJDIR))"' >>$@
	echo 'export TSPLUGINS_PATH="$(realpath $(TSPLUGINSDIR)/$(OBJDIR)):$(realpath $(LIBTSDUCKDIR))"' >>$@

.PHONY: test
test: execs
	source $(OBJDIR)/setenv.sh && $(OBJDIR)/utest
	source $(OBJDIR)/setenv.sh && $(OBJDIR)/utest_static

.PHONY: install install-devel
install install-devel:
	@true
