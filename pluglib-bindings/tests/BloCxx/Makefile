
SUBDIRS = Bool List String Types Hash HashList

%:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ || $(if $(findstring k,$(MAKEFLAGS)),true,exit); done
