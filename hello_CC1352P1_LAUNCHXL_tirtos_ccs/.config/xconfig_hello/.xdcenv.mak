#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/source;C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/kernel/tirtos/packages;C:/Users/Baker/OneDrive/Uni/Motorsports/Production_Workspace/Telemetry20/hello_CC1352P1_LAUNCHXL_tirtos_ccs/.config
override XDCROOT = C:/ti/ccs930/xdctools_3_60_02_34_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/source;C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/kernel/tirtos/packages;C:/Users/Baker/OneDrive/Uni/Motorsports/Production_Workspace/Telemetry20/hello_CC1352P1_LAUNCHXL_tirtos_ccs/.config;C:/ti/ccs930/xdctools_3_60_02_34_core/packages;..
HOSTOS = Windows
endif
