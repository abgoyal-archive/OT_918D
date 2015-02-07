cmd_kernel/debug/built-in.o :=  arm-eabi-ld.bfd -EL    -r -o kernel/debug/built-in.o kernel/debug/debug_core.o kernel/debug/gdbstub.o kernel/debug/kdb/built-in.o 
