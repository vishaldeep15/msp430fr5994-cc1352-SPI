#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.msp430.elf.MSP430X{1,0,16.9,11
#
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/msp430tirtos_pe430X.oe430X.dep
package/cfg/msp430tirtos_pe430X.oe430X.dep: ;
endif

package/cfg/msp430tirtos_pe430X.oe430X: | .interfaces
package/cfg/msp430tirtos_pe430X.oe430X: package/cfg/msp430tirtos_pe430X.c package/cfg/msp430tirtos_pe430X.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle430X $< ...
	$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/cl430 --abi=eabi -c  -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_16.9.11.LTS/include" --advice:power=all --advice:power_severity=suppress --advice:hw_config=all --define=__MSP430FR5994__ --define=ccs --define=MSP430WARE -g --printf_support=minimal --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40   -qq -pdsw225  -vmspx --near_data=none --code_model=large --data_model=restricted -eo.oe430X -ea.se430X   -Dxdc_cfg__xheader__='"xconfig_msp430tirtos/package/cfg/msp430tirtos_pe430X.h"'  -Dxdc_target_name__=MSP430X -Dxdc_target_types__=ti/targets/msp430/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_16_9_11 -O2 -g --optimize_with_debug  $(XDCINCS) -I$(ti.targets.msp430.elf.MSP430X.rootDir)/include  -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe430X $< -C   -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_16.9.11.LTS/include" --advice:power=all --advice:power_severity=suppress --advice:hw_config=all --define=__MSP430FR5994__ --define=ccs --define=MSP430WARE -g --printf_support=minimal --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40   -qq -pdsw225  -vmspx --near_data=none --code_model=large --data_model=restricted -eo.oe430X -ea.se430X   -Dxdc_cfg__xheader__='"xconfig_msp430tirtos/package/cfg/msp430tirtos_pe430X.h"'  -Dxdc_target_name__=MSP430X -Dxdc_target_types__=ti/targets/msp430/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_16_9_11 -O2 -g --optimize_with_debug  $(XDCINCS) -I$(ti.targets.msp430.elf.MSP430X.rootDir)/include  -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/msp430tirtos_pe430X.oe430X: export C_DIR=
package/cfg/msp430tirtos_pe430X.oe430X: PATH:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)
package/cfg/msp430tirtos_pe430X.oe430X: Path:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)

package/cfg/msp430tirtos_pe430X.se430X: | .interfaces
package/cfg/msp430tirtos_pe430X.se430X: package/cfg/msp430tirtos_pe430X.c package/cfg/msp430tirtos_pe430X.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle430X -n $< ...
	$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/cl430 --abi=eabi -c -n -s --symdebug:none -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_16.9.11.LTS/include" --advice:power=all --advice:power_severity=suppress --advice:hw_config=all --define=__MSP430FR5994__ --define=ccs --define=MSP430WARE -g --printf_support=minimal --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40   -qq -pdsw225  -vmspx --near_data=none --code_model=large --data_model=restricted -eo.oe430X -ea.se430X   -Dxdc_cfg__xheader__='"xconfig_msp430tirtos/package/cfg/msp430tirtos_pe430X.h"'  -Dxdc_target_name__=MSP430X -Dxdc_target_types__=ti/targets/msp430/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_16_9_11 -O2 -g --optimize_with_debug  $(XDCINCS) -I$(ti.targets.msp430.elf.MSP430X.rootDir)/include  -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe430X $< -C  -n -s --symdebug:none -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="F:/research/msp430-cc1352-spi/msp430master_tirtos" --include_path="C:/ti/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_16.9.11.LTS/include" --advice:power=all --advice:power_severity=suppress --advice:hw_config=all --define=__MSP430FR5994__ --define=ccs --define=MSP430WARE -g --printf_support=minimal --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40   -qq -pdsw225  -vmspx --near_data=none --code_model=large --data_model=restricted -eo.oe430X -ea.se430X   -Dxdc_cfg__xheader__='"xconfig_msp430tirtos/package/cfg/msp430tirtos_pe430X.h"'  -Dxdc_target_name__=MSP430X -Dxdc_target_types__=ti/targets/msp430/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_16_9_11 -O2 -g --optimize_with_debug  $(XDCINCS) -I$(ti.targets.msp430.elf.MSP430X.rootDir)/include  -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/msp430tirtos_pe430X.se430X: export C_DIR=
package/cfg/msp430tirtos_pe430X.se430X: PATH:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)
package/cfg/msp430tirtos_pe430X.se430X: Path:=$(ti.targets.msp430.elf.MSP430X.rootDir)/bin/;$(PATH)

clean,e430X ::
	-$(RM) package/cfg/msp430tirtos_pe430X.oe430X
	-$(RM) package/cfg/msp430tirtos_pe430X.se430X

msp430tirtos.pe430X: package/cfg/msp430tirtos_pe430X.oe430X package/cfg/msp430tirtos_pe430X.mak

clean::
	-$(RM) package/cfg/msp430tirtos_pe430X.mak
