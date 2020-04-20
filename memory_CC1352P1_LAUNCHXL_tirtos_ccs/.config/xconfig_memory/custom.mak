## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em4f linker.cmd package/cfg/memory_pem4f.oem4f

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/memory_pem4f.xdl
	$(SED) 's"^\"\(package/cfg/memory_pem4fcfg.cmd\)\"$""\"C:/Users/Baker/OneDrive/Uni/Motorsports/Production_Workspace/Telemetry20/memory_CC1352P1_LAUNCHXL_tirtos_ccs/.config/xconfig_memory/\1\""' package/cfg/memory_pem4f.xdl > $@
	-$(SETDATE) -r:max package/cfg/memory_pem4f.h compiler.opt compiler.opt.defs
