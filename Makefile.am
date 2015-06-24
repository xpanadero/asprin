###########################################################################
#  __ _ ___ _ __  _ __(_)_ __                                             #
# / _` / __| '_ \| '__| | '_ \                                            #
#| (_| \__ \ |_) | |  | | | | |                                           #
# \__,_|___/ .__/|_|  |_|_| |_|                                           #
#          |_|                                                            #
#                                                                         #
# Copyright (C) 2003 - 2004, SagE <sage@t0s.org>                          #
#                                                                         #
###########################################################################

AUTOMAKE_OPTIONS = foreign

SUBDIRS = src
CLEANFILES = *~ src/*~ include/*~ include/asprin/*~

install-data-hook:
	rm -rf $(libdir)/libasprin.so
	rm -rf $(libdir)/libasprin.la
	rm -rf $(libdir)/libasprin.so.$(LT_CURRENT)
	mv $(libdir)/libasprin.so.$(VERSION) $(libdir)/lib$(ASPRIN_NAME).so
