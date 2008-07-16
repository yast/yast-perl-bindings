
SUBDIRS = Namespace Simple

%:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ || $(if $(findstring k,$(MAKEFLAGS)),true,exit); done
