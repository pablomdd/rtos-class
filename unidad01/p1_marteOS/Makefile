#REF para GNAT=... hasta SHELL := ...
#https://stackoverflow.com/questions/8941110/how-i-could-add-dir-to-path-in-makefile

#By design make parser executes lines in a separate shell invocations, that's why changing variable (e.g. PATH) in one line, the change may not be applied for the next lines (see this post).
#One way to workaround this problem, is to convert multiple commands into a single line (separated by ;), or use One Shell special target (.ONESHELL, as of GNU Make 3.82).
#Alternatively you can provide PATH variable at the time when shell is invoked. For example:
#
#PATH  := $(PATH):$(PWD)/bin:/my/other/path
#SHELL := env PATH=$(PATH) /bin/bash
USER := pablomdd
GNAT := /home/${USER}/myapps/gnat/bin
PATH  := $(GNAT):$(PATH):/home/${USER}/marte_2.0_22Feb2017/utils
PERL5LIB := /home/${USER}/marte_2.0_22Feb2017
SHELL := env PATH=$(PATH) PERL5LIB=$(PERL5LIB) /bin/bash
msetcurrentarch:
	echo $(PATH)
	echo $(PERL5LIB)
	gcc --version
#	SOTR_msetcurrentarch x86 i386
	msetcurrentarch x86 i386

mkrtsmarteuc:
#	SOTR_mkrtsmarteuc01
	mkrtsmarteuc

mkmarte:
#	SOTR_mkmarte
	mkmarte
