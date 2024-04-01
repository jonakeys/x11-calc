#
#  makefile - RPN (Reverse Polish) calculator simulator.
#
#  Copyright(C) 2019 - MT
#
#  This  program is free software: you can redistribute it and/or modify it
#  under  the terms of the GNU General Public License as published  by  the
#  Free  Software Foundation, either version 3 of the License, or (at  your
#  option) any later version.
#
#  This  program  is distributed in the hope that it will  be  useful,  but
#  WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#  Public License for more details.
#
#  You  should have received a copy of the GNU General Public License along
#  with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Note separator (tab) at the beginning of the line CANNOT be a space..!
#
#  09 Oct 21         - Initial version - MT
#  22 Mar 24         - Simple redirection, without extra make.sh - macmpi
#                    - Fixed default rule so it works on NetBSD, Tru64, and
#                      Linux (now we can get rid of make.sh) - MT
#  01 Apr 24         - Consolidate Linux/NetBSD/Darwin and OSF1 - macmpi
#

all:
	@_flavor="`uname | tr '[:upper:]' '[:lower:]'`"; \
	if [ "$$_flavor" = "osf1" ]; then \
		$(MAKE) -s -f "makefile.common" $@ ; \
	else \
		$(MAKE) -s -f "makefile.complete" $@ ; \
	fi

.DEFAULT:
	@_flavor="`uname | tr '[:upper:]' '[:lower:]'`"; \
	if [ "$$_flavor" = "osf1" ]; then \
		$(MAKE) -s -f "makefile.common" $@ ; \
	else \
		$(MAKE) -s -f "makefile.complete" $@ ; \
	fi

