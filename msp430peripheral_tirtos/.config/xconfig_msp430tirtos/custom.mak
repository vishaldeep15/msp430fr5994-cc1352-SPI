## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e430X linker.cmd package/cfg/msp430tirtos_pe430X.oe430X

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/msp430tirtos_pe430X.xdl
	$(SED) 's"^\"\(package/cfg/msp430tirtos_pe430Xcfg.cmd\)\"$""\"F:/research/msp430-cc1352-spi/msp430slave_tirtos/.config/xconfig_msp430tirtos/\1\""' package/cfg/msp430tirtos_pe430X.xdl > $@
	-$(SETDATE) -r:max package/cfg/msp430tirtos_pe430X.h compiler.opt compiler.opt.defs
