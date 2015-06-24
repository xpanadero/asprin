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

lib_LTLIBRARIES = libasprin.la
bin_PROGRAMS = asprin-sh

INCLUDES = -I$(top_srcdir)/include/asprin/ -I$(top_builddir)/

ASPRIN_VERSION= -version-info ${LT_CURRENT}:${LT_REVISION}:${LT_AGE}

libasprin_la_CFLAGS = -fpic -DNET_SSL -D_REENTRANT -DMCC_HTTPD -DSPAPI20

libasprin_la_LDFLAGS = -lutil -shared ${ASPRIN_VERSION}


libasprin_la_SOURCES = \
	shell.c\
	asprin.c

asprin_sh_SOURCES = \
	asprin-sh.c
