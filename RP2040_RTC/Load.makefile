#######################################
# download
# CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program debugprobe.elf verify reset exit"
#######################################
OPENOCD_ROOT = D:\OpenOCD-20231002\share\openocd
OPENOCD_INTERFACE = cmsis-dap.cfg
OPENOCD_TARGET = rp2040.cfg
PWD_DIR = D:/RP2040/pico-project-generator/RP2040_RTC/build/RP2040_RTC

upload:
 openocd -f $(OPENOCD_ROOT)\scripts\interface\$(OPENOCD_INTERFACE)\
 -f $(OPENOCD_ROOT)\scripts\target\$(OPENOCD_TARGET)\
-c  "adapter speed 5000"\
-c "program $(PWD_DIR).elf verify reset exit"

#######################################
# upload
#######################################
