
SUBDIRS = List ListAbc ListList String Types Hash ListHash HashList

%:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ || $(if $(findstring k,$(MAKEFLAGS)),true,exit); done
