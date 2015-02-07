cmd_drivers/staging/android/logger.o := arm-eabi-gcc -Wp,-MD,drivers/staging/android/.logger.o.d  -nostdinc -isystem /local/gin2_build/v343-7-band18/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/../lib/gcc/arm-eabi/4.4.3/include -I/local/gin2_build/v343-7-band18/kernel/arch/arm/include -Iinclude  -include include/generated/autoconf.h -I..//mediatek/custom/jrd73_gb/common -I../mediatek/platform/mt6573/kernel/core/include/ -I../mediatek/source/kernel/include/ -I../mediatek/custom/out/jrd73_gb/kernel/leds/ -I../mediatek/custom/out/jrd73_gb/kernel/magnetometer/ -I../mediatek/custom/out/jrd73_gb/kernel/core/ -I../mediatek/custom/out/jrd73_gb/kernel/alsps/ -I../mediatek/custom/out/jrd73_gb/kernel/rtc/ -I../mediatek/custom/out/jrd73_gb/kernel/dct/ -I../mediatek/custom/out/jrd73_gb/kernel/ofn/ -I../mediatek/custom/out/jrd73_gb/kernel/touchpanel/ -I../mediatek/custom/out/jrd73_gb/kernel/battery/ -I../mediatek/custom/out/jrd73_gb/kernel/headset/ -I../mediatek/custom/out/jrd73_gb/kernel/kpd/ -I../mediatek/custom/out/jrd73_gb/kernel/accelerometer/ -I../mediatek/custom/out/jrd73_gb/kernel/camera/ -I../mediatek/custom/out/jrd73_gb/kernel/sound/ -I../mediatek/custom/out/jrd73_gb/kernel/usb/ -I../mediatek/custom/out/jrd73_gb/kernel/lens/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/lens/ -I../mediatek/custom/out/jrd73_gb/kernel/gyroscope/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/lcm/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/lcm/ -I../mediatek/custom/out/jrd73_gb/kernel/leds/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/magnetometer/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/eeprom/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/eeprom/ -I../mediatek/custom/out/jrd73_gb/kernel/alsps/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/fm/ -I../mediatek/custom/out/jrd73_gb/kernel/ofn/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/jogball/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/accelerometer/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/barometer/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/imgsensor/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/imgsensor/ -I../mediatek/custom/out/jrd73_gb/kernel/./ -I../mediatek/custom/out/jrd73_gb/kernel/sound/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/flashlight/inc/ -I../mediatek/custom/out/jrd73_gb/kernel/flashlight/ -I../mediatek/custom/out/jrd73_gb/hal/audioflinger/ -I../mediatek/custom/out/jrd73_gb/hal/sensors/ -I../mediatek/custom/out/jrd73_gb/hal/camera/ -I../mediatek/custom/out/jrd73_gb/hal/lens/ -I../mediatek/custom/out/jrd73_gb/hal/inc/ -I../mediatek/custom/out/jrd73_gb/hal/bluetooth/ -I../mediatek/custom/out/jrd73_gb/hal/eeprom/ -I../mediatek/custom/out/jrd73_gb/hal/combo/ -I../mediatek/custom/out/jrd73_gb/hal/ant/ -I../mediatek/custom/out/jrd73_gb/hal/matv/ -I../mediatek/custom/out/jrd73_gb/hal/imgsensor/ -I../mediatek/custom/out/jrd73_gb/hal/flashlight/ -D__KERNEL__ -mlittle-endian -DMODEM_3G -I../mediatek/platform/mt6573/kernel/core/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=6 -march=armv6k -mtune=arm1136j-s -msoft-float -Uarm -DMTK_WAPI_SUPPORT -DMTK_SENSOR_SUPPORT -DCUSTOM_KERNEL_ALSPS -DCUSTOM_KERNEL_MAGNETOMETER -DCUSTOM_KERNEL_ACCELEROMETER -DMTK_BT_SUPPORT -DMTK_CAMERA_APP_3DHW_SUPPORT -DMTK_VT3G324M_SUPPORT -DMTK_M4U_SUPPORT -DMTK_MATV_ANALOG_SUPPORT -DMTK_WLAN_SUPPORT -DMTK_BT_21_SUPPORT -DMTK_BT_30_SUPPORT -DMTK_BT_40_SUPPORT -DMTK_BT_PROFILE_OPP -DMTK_BT_PROFILE_SIMAP -DMTK_BT_PROFILE_PRXM -DMTK_BT_PROFILE_PRXR -DMTK_BT_PROFILE_HIDH -DMTK_BT_PROFILE_FTP -DMTK_BT_PROFILE_PBAP -DMTK_BT_PROFILE_MANAGER -DMTK_BT_PROFILE_BPP -DMTK_BT_PROFILE_BIP -DMTK_BT_PROFILE_DUN -DMTK_BT_PROFILE_PAN -DMTK_BT_PROFILE_HFP -DMTK_BT_PROFILE_A2DP -DMTK_BT_PROFILE_AVRCP -DMTK_BT_PROFILE_AVRCP14 -DMTK_BT_PROFILE_MAPS -DMTK_BT_PROFILE_MAPC -DMTK_BT_PROFILE_SPP -DMTK_WB_SPEECH_SUPPORT -DHAVE_AWBENCODE_FEATURE -DHAVE_AACENCODE_FEATURE -DMTK_WIFI_HOTSPOT_SUPPORT -DMTK_FM_SUPPORT -DMTK_GPS_SUPPORT -DMTK_MAV_SUPPORT -DHAVE_AEE_FEATURE -DMTK_SIM_AUTHENTICATION_SUPPORT -DMTK_EAP_SIM_AKA -DMT6573 -DMT6620_FM -DMTK_MT6620 -DS5K5CAGX_YUV -DOV7690_YUV -DDUMMY -DDUMMY_LENS -DMT6620 -DFM_DIGITAL_INPUT -DMTK_GPS_MT6620 -DMT6620E3 -DDUMMY_FLASHLIGHT -DMODEM_3G -DILI9481 -DHX8357B -DLCM_WIDTH=\"320\" -DLCM_HEIGHT=\"480\" -DMTK_LCM_PHYSICAL_ROTATION=\"0\" -DMTK_SHARE_MODEM_SUPPORT=\"2\" -DMTK_SHARE_MODEM_CURRENT=\"1\" -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(logger)"  -D"KBUILD_MODNAME=KBUILD_STR(logger)"  -c -o drivers/staging/android/logger.o drivers/staging/android/logger.c

deps_drivers/staging/android/logger.o := \
  drivers/staging/android/logger.c \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/preempt/notifiers.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/mm/owner.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/capability.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/posix_types.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/align.h \
  /local/gin2_build/v343-7-band18/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/../lib/gcc/arm-eabi/4.4.3/include/stdarg.h \
  include/linux/linkage.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/bitops.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/irqflags.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/hwcap.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/timex.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/thread_info.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/processor.h \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  include/asm-generic/atomic-long.h \
  include/linux/math64.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/div64.h \
  include/linux/param.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/timex.h \
  ../mediatek/platform/mt6573/kernel/core/include/mach/timex.h \
  include/linux/jiffies.h \
  include/linux/rbtree.h \
  include/linux/cpumask.h \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/string.h \
  include/linux/errno.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/completion.h \
  include/linux/wait.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/current.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/sparsemem.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/glue.h \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/abrt/lv4t.h) \
    $(wildcard include/config/cpu/abrt/ev4.h) \
    $(wildcard include/config/cpu/abrt/ev4t.h) \
    $(wildcard include/config/cpu/abrt/ev5tj.h) \
    $(wildcard include/config/cpu/abrt/ev5t.h) \
    $(wildcard include/config/cpu/abrt/ev6.h) \
    $(wildcard include/config/cpu/abrt/ev7.h) \
    $(wildcard include/config/cpu/pabrt/legacy.h) \
    $(wildcard include/config/cpu/pabrt/v6.h) \
    $(wildcard include/config/cpu/pabrt/v7.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/discontigmem.h) \
  include/linux/const.h \
  ../mediatek/platform/mt6573/kernel/core/include/mach/memory.h \
    $(wildcard include/config/have/tcm.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/cpu/has/asid.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/sem.h \
  include/linux/ipc.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/ipcbuf.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/sembuf.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
  include/linux/rcutree.h \
  include/linux/signal.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/signal.h \
  include/asm-generic/signal-defs.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/sigcontext.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/path.h \
  include/linux/pid.h \
  include/linux/percpu.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
  include/linux/srcu.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/topology.h \
  include/asm-generic/topology.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rculist.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
    $(wildcard include/config/sysfs.h) \
  include/linux/kref.h \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/sysctl.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/aio.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/ftrace/module/support.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
  include/linux/stat.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/stat.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/elf.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/user.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/immediate.h \
  include/linux/marker.h \
  include/linux/tracepoint.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/module.h \
    $(wildcard include/config/arm/unwind.h) \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/radix-tree.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/inet.h \
  include/linux/fcntl.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \
  include/linux/miscdevice.h \
  include/linux/major.h \
  include/linux/uaccess.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/uaccess.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
  include/linux/poll.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/poll.h \
  include/asm-generic/poll.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
  include/linux/kmemtrace.h \
    $(wildcard include/config/kmemtrace.h) \
  include/trace/events/kmem.h \
  include/linux/kmalloc_sizes.h \
  drivers/staging/android/logger.h \
  /local/gin2_build/v343-7-band18/kernel/arch/arm/include/asm/ioctls.h \
  ../mediatek/custom/out/jrd73_gb/kernel/core/logger_custom.h \
  include/linux/autoconf.h \
    $(wildcard include/config/mtk/usb/gadget.h) \
    $(wildcard include/config/kernel/gzip.h) \
    $(wildcard include/config/input/keyboard.h) \
    $(wildcard include/config/ip/nf/target/redirect.h) \
    $(wildcard include/config/crc32.h) \
    $(wildcard include/config/i2c/boardinfo.h) \
    $(wildcard include/config/have/aout.h) \
    $(wildcard include/config/vfp.h) \
    $(wildcard include/config/fb/tileblitting.h) \
    $(wildcard include/config/cpu/freq/gov/conservative.h) \
    $(wildcard include/config/blk/dev/dm.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/flatmem/manual.h) \
    $(wildcard include/config/mtk/aee/kdump.h) \
    $(wildcard include/config/network/filesystems.h) \
    $(wildcard include/config/cpu/freq/gov/ondemand.h) \
    $(wildcard include/config/mtk/fgadc.h) \
    $(wildcard include/config/experimental.h) \
    $(wildcard include/config/arch/suspend/possible.h) \
    $(wildcard include/config/mtk/g2d.h) \
    $(wildcard include/config/ssb/possible.h) \
    $(wildcard include/config/nf/nat/sip.h) \
    $(wildcard include/config/stp.h) \
    $(wildcard include/config/inet6/tunnel.h) \
    $(wildcard include/config/nf/conntrack/sip.h) \
    $(wildcard include/config/have/kernel/lzma.h) \
    $(wildcard include/config/default/security/dac.h) \
    $(wildcard include/config/fib/rules.h) \
    $(wildcard include/config/netfilter/xt/match/realm.h) \
    $(wildcard include/config/netdev/1000.h) \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/crypto/aead.h) \
    $(wildcard include/config/default/tcp/cong.h) \
    $(wildcard include/config/uevent/helper/path.h) \
    $(wildcard include/config/mtk/serial.h) \
    $(wildcard include/config/nf/nat/proto/gre.h) \
    $(wildcard include/config/android/binder/ipc.h) \
    $(wildcard include/config/keyboard/hid.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/namespaces.h) \
    $(wildcard include/config/inet6/xfrm/mode/routeoptimization.h) \
    $(wildcard include/config/kgdb/serial/console.h) \
    $(wildcard include/config/xfrm/ipcomp.h) \
    $(wildcard include/config/crypto/rng2.h) \
    $(wildcard include/config/netfilter/netlink/queue.h) \
    $(wildcard include/config/mtk/combo/bt.h) \
    $(wildcard include/config/msdos/fs.h) \
    $(wildcard include/config/ipv6/pimsm/v2.h) \
    $(wildcard include/config/ramzswap.h) \
    $(wildcard include/config/dm/crypt.h) \
    $(wildcard include/config/have/proc/cpu.h) \
    $(wildcard include/config/lzo/decompress.h) \
    $(wildcard include/config/cpu/32v6.h) \
    $(wildcard include/config/mtk/net/ccmni.h) \
    $(wildcard include/config/rd/lzma.h) \
    $(wildcard include/config/crypto/hmac.h) \
    $(wildcard include/config/ip/nf/arptables.h) \
    $(wildcard include/config/mtk/gps.h) \
    $(wildcard include/config/mtd/partitions.h) \
    $(wildcard include/config/nf/conntrack/proc/compat.h) \
    $(wildcard include/config/timerfd.h) \
    $(wildcard include/config/mtd/cfi/i2.h) \
    $(wildcard include/config/crypto/authenc.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/mtd.h) \
    $(wildcard include/config/have/trace/clock/generic.h) \
    $(wildcard include/config/decompress/lzma.h) \
    $(wildcard include/config/mtk/combo/gps.h) \
    $(wildcard include/config/ask/ip/fib/hash.h) \
    $(wildcard include/config/input/mousedev.h) \
    $(wildcard include/config/crypto/des.h) \
    $(wildcard include/config/nls/codepage/437.h) \
    $(wildcard include/config/mtd/nand/ids.h) \
    $(wildcard include/config/input/mouse.h) \
    $(wildcard include/config/rtc/intf/sysfs.h) \
    $(wildcard include/config/blk/dev/initrd.h) \
    $(wildcard include/config/leds/trigger/timer.h) \
    $(wildcard include/config/mtd/nand/platform.h) \
    $(wildcard include/config/zlib/inflate.h) \
    $(wildcard include/config/hwmon.h) \
    $(wildcard include/config/crypto/twofish/common.h) \
    $(wildcard include/config/logo/linux/clut224.h) \
    $(wildcard include/config/ip/pnp.h) \
    $(wildcard include/config/rtc/intf/proc.h) \
    $(wildcard include/config/mtd/concat.h) \
    $(wildcard include/config/net/cls/route.h) \
    $(wildcard include/config/stacktrace/support.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/crypto/manager/tests.h) \
    $(wildcard include/config/netfilter/xt/match/string.h) \
    $(wildcard include/config/has/wakelock.h) \
    $(wildcard include/config/logo.h) \
    $(wildcard include/config/standalone.h) \
    $(wildcard include/config/cpu/freq/gov/performance.h) \
    $(wildcard include/config/arch/has/cpufreq.h) \
    $(wildcard include/config/ashmem.h) \
    $(wildcard include/config/have/ide.h) \
    $(wildcard include/config/init/env/arg/limit.h) \
    $(wildcard include/config/nf/conntrack/pptp.h) \
    $(wildcard include/config/context/switch/tracer.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/switch.h) \
    $(wildcard include/config/devkmem.h) \
    $(wildcard include/config/textsearch/kmp.h) \
    $(wildcard include/config/vt.h) \
    $(wildcard include/config/netfilter/xt/target/classify.h) \
    $(wildcard include/config/power/supply.h) \
    $(wildcard include/config/have/trace/clock/32/to/64.h) \
    $(wildcard include/config/cpu/cache/vipt.h) \
    $(wildcard include/config/netfilter/xt/target/nfqueue.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/nls.h) \
    $(wildcard include/config/mfd/support.h) \
    $(wildcard include/config/pppoe.h) \
    $(wildcard include/config/netfilter/xt/match/owner.h) \
    $(wildcard include/config/syn/cookies.h) \
    $(wildcard include/config/ip/advanced/router.h) \
    $(wildcard include/config/cpu/freq/gov/userspace.h) \
    $(wildcard include/config/inet/ipcomp.h) \
    $(wildcard include/config/nls/iso8859/1.h) \
    $(wildcard include/config/crypto/workqueue.h) \
    $(wildcard include/config/cpu/freq/table.h) \
    $(wildcard include/config/textsearch/bm.h) \
    $(wildcard include/config/ppp/mppe.h) \
    $(wildcard include/config/rfkill.h) \
    $(wildcard include/config/netdevices.h) \
    $(wildcard include/config/iosched/deadline.h) \
    $(wildcard include/config/eventfd.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/defconfig/list.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ip/nf/target/masquerade.h) \
    $(wildcard include/config/proc/page/monitor.h) \
    $(wildcard include/config/cc/optimize/for/size.h) \
    $(wildcard include/config/rd/lzo.h) \
    $(wildcard include/config/android/low/memory/killer.h) \
    $(wildcard include/config/mtk/bt/net/dev.h) \
    $(wildcard include/config/nf/defrag/ipv4.h) \
    $(wildcard include/config/select/memory/model.h) \
    $(wildcard include/config/inet/xfrm/mode/beet.h) \
    $(wildcard include/config/mmc/unsafe/resume.h) \
    $(wildcard include/config/ipv6/route/info.h) \
    $(wildcard include/config/netfilter/advanced.h) \
    $(wildcard include/config/crypto/deflate.h) \
    $(wildcard include/config/ipv6/router/pref.h) \
    $(wildcard include/config/magic/sysrq.h) \
    $(wildcard include/config/mtk/mflexvideo/driver.h) \
    $(wildcard include/config/mtk/sched/tracers.h) \
    $(wildcard include/config/default/cfq.h) \
    $(wildcard include/config/inet6/xfrm/mode/tunnel.h) \
    $(wildcard include/config/ip/nf/filter.h) \
    $(wildcard include/config/cpu/v6.h) \
    $(wildcard include/config/netfilter/xt/match/length.h) \
    $(wildcard include/config/fat/fs.h) \
    $(wildcard include/config/textsearch/fsm.h) \
    $(wildcard include/config/inet/tunnel.h) \
    $(wildcard include/config/cpu/freq/mt6573.h) \
    $(wildcard include/config/mmc/block/bounce.h) \
    $(wildcard include/config/generic/clockevents.h) \
    $(wildcard include/config/iosched/cfq.h) \
    $(wildcard include/config/cpu/cp15/mmu.h) \
    $(wildcard include/config/rfkill/leds.h) \
    $(wildcard include/config/console/translations.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/nf/nat/snmp/basic.h) \
    $(wildcard include/config/cpu/freq.h) \
    $(wildcard include/config/dummy/console.h) \
    $(wildcard include/config/nls/ascii.h) \
    $(wildcard include/config/mediatek/solution.h) \
    $(wildcard include/config/qfmt/v2.h) \
    $(wildcard include/config/leds/triggers.h) \
    $(wildcard include/config/rd/gzip.h) \
    $(wildcard include/config/have/ltt/dump/tables.h) \
    $(wildcard include/config/inet/xfrm/mode/transport.h) \
    $(wildcard include/config/crypto/md5.h) \
    $(wildcard include/config/pm/ops.h) \
    $(wildcard include/config/usb/android/adb.h) \
    $(wildcard include/config/binfmt/elf.h) \
    $(wildcard include/config/ip/pimsm/v1.h) \
    $(wildcard include/config/inet6/ah.h) \
    $(wildcard include/config/cpu/cp15.h) \
    $(wildcard include/config/usb/android.h) \
    $(wildcard include/config/inet/xfrm/tunnel.h) \
    $(wildcard include/config/netfilter/xtables.h) \
    $(wildcard include/config/mtk/mmc.h) \
    $(wildcard include/config/slabinfo.h) \
    $(wildcard include/config/gps.h) \
    $(wildcard include/config/logo/linux/mono.h) \
    $(wildcard include/config/hardirqs/sw/resend.h) \
    $(wildcard include/config/vt/hw/console/binding.h) \
    $(wildcard include/config/mtk/i2c.h) \
    $(wildcard include/config/earlysuspend.h) \
    $(wildcard include/config/crc16.h) \
    $(wildcard include/config/generic/calibrate/delay.h) \
    $(wildcard include/config/cpu/has/pmu.h) \
    $(wildcard include/config/broken/on/smp.h) \
    $(wildcard include/config/tmpfs.h) \
    $(wildcard include/config/anon/inodes.h) \
    $(wildcard include/config/ip/pnp/dhcp.h) \
    $(wildcard include/config/vmsplit/3g.h) \
    $(wildcard include/config/rtc/hctosys.h) \
    $(wildcard include/config/serial/core/console.h) \
    $(wildcard include/config/android.h) \
    $(wildcard include/config/nf/conntrack/events.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/mtk/fb/support/assertion/layer.h) \
    $(wildcard include/config/nf/conntrack/ftp.h) \
    $(wildcard include/config/usb/gadget.h) \
    $(wildcard include/config/sound.h) \
    $(wildcard include/config/mtk/smart/battery.h) \
    $(wildcard include/config/unix.h) \
    $(wildcard include/config/yaffs/yaffs1.h) \
    $(wildcard include/config/crypto/hash2.h) \
    $(wildcard include/config/cpu/freq/gov/powersave.h) \
    $(wildcard include/config/netdev/10000.h) \
    $(wildcard include/config/inet/esp.h) \
    $(wildcard include/config/md.h) \
    $(wildcard include/config/crypto/algapi.h) \
    $(wildcard include/config/bridge.h) \
    $(wildcard include/config/misc/devices.h) \
    $(wildcard include/config/rd/bzip2.h) \
    $(wildcard include/config/input/uinput.h) \
    $(wildcard include/config/mtprof/cputime.h) \
    $(wildcard include/config/mtd/cfi/i1.h) \
    $(wildcard include/config/nf/nat.h) \
    $(wildcard include/config/crypto/hash.h) \
    $(wildcard include/config/log/buf/shift.h) \
    $(wildcard include/config/extra/firmware.h) \
    $(wildcard include/config/cache/l2x0.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/virt/to/bus.h) \
    $(wildcard include/config/vfat/fs.h) \
    $(wildcard include/config/generic/time.h) \
    $(wildcard include/config/blk/dev/loop.h) \
    $(wildcard include/config/wakelock.h) \
    $(wildcard include/config/input/misc.h) \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/mtd/nand/ecc.h) \
    $(wildcard include/config/crypto/cbc.h) \
    $(wildcard include/config/rtc/class.h) \
    $(wildcard include/config/have/latencytop/support.h) \
    $(wildcard include/config/generic/tracer.h) \
    $(wildcard include/config/yaffs/auto/yaffs2.h) \
    $(wildcard include/config/mtk/sound.h) \
    $(wildcard include/config/have/function/tracer.h) \
    $(wildcard include/config/nf/nat/tftp.h) \
    $(wildcard include/config/netfilter/xt/match/multiport.h) \
    $(wildcard include/config/crypto/manager2.h) \
    $(wildcard include/config/usb/gadget/vbus/draw.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/i2c.h) \
    $(wildcard include/config/frame/pointer.h) \
    $(wildcard include/config/net/ipgre/broadcast.h) \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/relay.h) \
    $(wildcard include/config/crypto/ecb.h) \
    $(wildcard include/config/debug/fs.h) \
    $(wildcard include/config/mtk/combo/fm.h) \
    $(wildcard include/config/base/full.h) \
    $(wildcard include/config/fb/cfb/imageblit.h) \
    $(wildcard include/config/zlib/deflate.h) \
    $(wildcard include/config/yaffs/fs.h) \
    $(wildcard include/config/fw/loader.h) \
    $(wildcard include/config/rtc/hctosys/device.h) \
    $(wildcard include/config/netfilter/xt/match/recent.h) \
    $(wildcard include/config/signalfd.h) \
    $(wildcard include/config/crypto/sha1.h) \
    $(wildcard include/config/ipv6/privacy.h) \
    $(wildcard include/config/dummy.h) \
    $(wildcard include/config/mtk/combo.h) \
    $(wildcard include/config/usb/gadget/dualspeed.h) \
    $(wildcard include/config/has/iomem.h) \
    $(wildcard include/config/ring/buffer/allow/swap.h) \
    $(wildcard include/config/pppopns.h) \
    $(wildcard include/config/usb/android/mass/storage.h) \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/cpu/freq/default/gov/ondemand.h) \
    $(wildcard include/config/mtd/map/bank/width/1.h) \
    $(wildcard include/config/arch/mt6573.h) \
    $(wildcard include/config/yaffs/xattr.h) \
    $(wildcard include/config/net.h) \
    $(wildcard include/config/input/evdev.h) \
    $(wildcard include/config/quota/tree.h) \
    $(wildcard include/config/packet.h) \
    $(wildcard include/config/netfilter/xt/match/iprange.h) \
    $(wildcard include/config/nf/conntrack/tftp.h) \
    $(wildcard include/config/nop/tracer.h) \
    $(wildcard include/config/inet.h) \
    $(wildcard include/config/mtk/aee/aed.h) \
    $(wildcard include/config/ip/pnp/bootp.h) \
    $(wildcard include/config/prevent/firmware/build.h) \
    $(wildcard include/config/crypto/twofish.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/mtk/aee/ipanic.h) \
    $(wildcard include/config/rtc/lib.h) \
    $(wildcard include/config/usb/android/acm.h) \
    $(wildcard include/config/netfilter/xt/match/policy.h) \
    $(wildcard include/config/have/kprobes.h) \
    $(wildcard include/config/cpu/tlb/v6.h) \
    $(wildcard include/config/nf/conntrack/mark.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/nls/codepage/950.h) \
    $(wildcard include/config/mtk/pmu.h) \
    $(wildcard include/config/blk/dev/ram/count.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/inet/xfrm/mode/tunnel.h) \
    $(wildcard include/config/nf/nat/needed.h) \
    $(wildcard include/config/lockdep/support.h) \
    $(wildcard include/config/cpu/freq/debug.h) \
    $(wildcard include/config/rtc/intf/alarm.h) \
    $(wildcard include/config/generic/hardirqs/no//do/irq.h) \
    $(wildcard include/config/cpu/freq/stat.h) \
    $(wildcard include/config/mtd/blkdevs.h) \
    $(wildcard include/config/mtk/touchpanel.h) \
    $(wildcard include/config/inet6/esp.h) \
    $(wildcard include/config/mt65xx/tracer.h) \
    $(wildcard include/config/sysctl/syscall.h) \
    $(wildcard include/config/mtk/leds.h) \
    $(wildcard include/config/mtk/accdet.h) \
    $(wildcard include/config/input/mousedev/screen/x.h) \
    $(wildcard include/config/need/dma/map/state.h) \
    $(wildcard include/config/android/paranoid/network.h) \
    $(wildcard include/config/lock/kernel.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/zboot/rom/bss.h) \
    $(wildcard include/config/netfilter/xt/match/time.h) \
    $(wildcard include/config/mtk/fb.h) \
    $(wildcard include/config/have/kernel/gzip.h) \
    $(wildcard include/config/dm/uevent.h) \
    $(wildcard include/config/netfilter/xt/match/mac.h) \
    $(wildcard include/config/generic/allocator.h) \
    $(wildcard include/config/android/timed/output.h) \
    $(wildcard include/config/libcrc32c.h) \
    $(wildcard include/config/inet/tcp/diag.h) \
    $(wildcard include/config/hw/console.h) \
    $(wildcard include/config/dm/debug.h) \
    $(wildcard include/config/devmem.h) \
    $(wildcard include/config/max/dram/size/support.h) \
    $(wildcard include/config/iosched/noop.h) \
    $(wildcard include/config/quotactl.h) \
    $(wildcard include/config/debug/kernel.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/localversion.h) \
    $(wildcard include/config/crypto.h) \
    $(wildcard include/config/default/mmap/min/addr.h) \
    $(wildcard include/config/ip/nf/iptables.h) \
    $(wildcard include/config/cmdline.h) \
    $(wildcard include/config/ip/pimsm/v2.h) \
    $(wildcard include/config/usb/arch/has/hcd.h) \
    $(wildcard include/config/mtk/combo/wifi.h) \
    $(wildcard include/config/mtk/wd/kicker.h) \
    $(wildcard include/config/ipv6/optimistic/dad.h) \
    $(wildcard include/config/alignment/trap.h) \
    $(wildcard include/config/mtk/aee/feature.h) \
    $(wildcard include/config/scsi/mod.h) \
    $(wildcard include/config/oprofile.h) \
    $(wildcard include/config/crypto/crc32c.h) \
    $(wildcard include/config/serial/core.h) \
    $(wildcard include/config/have/kretprobes.h) \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/fb/cfb/fillrect.h) \
    $(wildcard include/config/nf/nat/pptp.h) \
    $(wildcard include/config/font/8x16.h) \
    $(wildcard include/config/inet6/ipcomp.h) \
    $(wildcard include/config/mtk/jpeg.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/misc/filesystems.h) \
    $(wildcard include/config/decompress/bzip2.h) \
    $(wildcard include/config/print/quota/warning.h) \
    $(wildcard include/config/ip/nf/target/netmap.h) \
    $(wildcard include/config/mtk/keypad.h) \
    $(wildcard include/config/yaffs/yaffs2.h) \
    $(wildcard include/config/profiling.h) \
    $(wildcard include/config/inet6/xfrm/mode/transport.h) \
    $(wildcard include/config/crypto/arc4.h) \
    $(wildcard include/config/mtk/rtc.h) \
    $(wildcard include/config/slhc.h) \
    $(wildcard include/config/crypto/manager.h) \
    $(wildcard include/config/usb/android/rndis/wceis.h) \
    $(wildcard include/config/mtd/nand.h) \
    $(wildcard include/config/vectors/base.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
    $(wildcard include/config/mmc/block.h) \
    $(wildcard include/config/mtk/serial/console.h) \
    $(wildcard include/config/wireless.h) \
    $(wildcard include/config/wext/proc.h) \
    $(wildcard include/config/perf/use/vmalloc.h) \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/fat/default/iocharset.h) \
    $(wildcard include/config/frame/warn.h) \
    $(wildcard include/config/generic/hweight.h) \
    $(wildcard include/config/initramfs/source.h) \
    $(wildcard include/config/lzo/compress.h) \
    $(wildcard include/config/mmc.h) \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/pppolac.h) \
    $(wildcard include/config/has/ioport.h) \
    $(wildcard include/config/ramzswap/stats.h) \
    $(wildcard include/config/kgdb/kdb.h) \
    $(wildcard include/config/fb/earlysuspend.h) \
    $(wildcard include/config/i2c/helper/auto.h) \
    $(wildcard include/config/cpu/cache/v6.h) \
    $(wildcard include/config/inet/ah.h) \
    $(wildcard include/config/default/iosched.h) \
    $(wildcard include/config/ipv6/mip6.h) \
    $(wildcard include/config/nlattr.h) \
    $(wildcard include/config/tcp/cong/cubic.h) \
    $(wildcard include/config/mtprof/irq/duration.h) \
    $(wildcard include/config/suspend/freezer.h) \
    $(wildcard include/config/firmware/in/kernel.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
    $(wildcard include/config/input/touchscreen.h) \
    $(wildcard include/config/arpd.h) \
    $(wildcard include/config/netfilter/xt/match/limit.h) \
    $(wildcard include/config/fb.h) \
    $(wildcard include/config/i2c/compat.h) \
    $(wildcard include/config/msdos/partition.h) \
    $(wildcard include/config/have/oprofile.h) \
    $(wildcard include/config/have/generic/dma/coherent.h) \
    $(wildcard include/config/logo/linux/vga16.h) \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/ip/fib/hash.h) \
    $(wildcard include/config/nf/conntrack/ipv4.h) \
    $(wildcard include/config/zone/dma/flag.h) \
    $(wildcard include/config/inet6/xfrm/tunnel.h) \
    $(wildcard include/config/mtd/map/bank/width/2.h) \
    $(wildcard include/config/ip/multicast.h) \
    $(wildcard include/config/wakelock/stat.h) \
    $(wildcard include/config/has/earlysuspend.h) \
    $(wildcard include/config/default/security.h) \
    $(wildcard include/config/tick/oneshot.h) \
    $(wildcard include/config/nf/nat/proto/udplite.h) \
    $(wildcard include/config/mtk/ccci/driver.h) \
    $(wildcard include/config/usb/android/rndis.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/android/logger.h) \
    $(wildcard include/config/hw/random.h) \
    $(wildcard include/config/have/function/graph/tracer.h) \
    $(wildcard include/config/crypto/blkcipher2.h) \
    $(wildcard include/config/mtd/block.h) \
    $(wildcard include/config/wext/priv.h) \
    $(wildcard include/config/mtd/nand/denali/scratch/reg/addr.h) \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/rfkill/input.h) \
    $(wildcard include/config/ikconfig.h) \
    $(wildcard include/config/suspend/nvs.h) \
    $(wildcard include/config/bridge/igmp/snooping.h) \
    $(wildcard include/config/have/perf/events.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/uid/stat.h) \
    $(wildcard include/config/debug/memory/init.h) \
    $(wildcard include/config/sys/supports/apm/emulation.h) \
    $(wildcard include/config/rtc/intf/alarm/dev.h) \
    $(wildcard include/config/fat/default/codepage.h) \
    $(wildcard include/config/blk/dev.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/mtk/vibrator.h) \
    $(wildcard include/config/tracing/support.h) \
    $(wildcard include/config/unix98/ptys.h) \
    $(wildcard include/config/printk/time.h) \
    $(wildcard include/config/input/mousedev/screen/y.h) \
    $(wildcard include/config/ppp.h) \
    $(wildcard include/config/have/kernel/lzo.h) \
    $(wildcard include/config/inet/diag.h) \
    $(wildcard include/config/nf/nat/ftp.h) \
    $(wildcard include/config/nf/ct/proto/udplite.h) \
    $(wildcard include/config/ikconfig/proc.h) \
    $(wildcard include/config/proc.h) \
    $(wildcard include/config/elf/core.h) \
    $(wildcard include/config/textsearch.h) \
    $(wildcard include/config/usb/support.h) \
    $(wildcard include/config/staging.h) \
    $(wildcard include/config/mtd/char.h) \
    $(wildcard include/config/pvr/sgx.h) \
    $(wildcard include/config/netfilter/xt/match/physdev.h) \
    $(wildcard include/config/blk/dev/ram.h) \
    $(wildcard include/config/netfilter/xt/match/state.h) \
    $(wildcard include/config/inet6/xfrm/mode/beet.h) \
    $(wildcard include/config/mtk/ccci/devices.h) \
    $(wildcard include/config/posix/mqueue/sysctl.h) \
    $(wildcard include/config/fb/cfb/copyarea.h) \
    $(wildcard include/config/cpu/freq/stat/details.h) \
    $(wildcard include/config/generic/clockevents/build.h) \
    $(wildcard include/config/sysvipc/sysctl.h) \
    $(wildcard include/config/decompress/gzip.h) \
    $(wildcard include/config/yaffs/disable/tags/ecc.h) \
    $(wildcard include/config/decompress/lzo.h) \
    $(wildcard include/config/llc.h) \
    $(wildcard include/config/cross/compile.h) \
    $(wildcard include/config/usb/gadget/selected.h) \
    $(wildcard include/config/nls/utf8.h) \
    $(wildcard include/config/new/leds.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/android/pmem.h) \
    $(wildcard include/config/netfilter/netlink.h) \
    $(wildcard include/config/nf/ct/acct.h) \
    $(wildcard include/config/rcu/fanout.h) \
    $(wildcard include/config/bitreverse.h) \
    $(wildcard include/config/blk/dev/ram/size.h) \
    $(wildcard include/config/console/poll.h) \
    $(wildcard include/config/crypto/pcomp.h) \
    $(wildcard include/config/fb/mode/helpers.h) \
    $(wildcard include/config/crypto/blkcipher.h) \
    $(wildcard include/config/yaffs/short/names/in/ram.h) \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/ip/nf/target/reject.h) \
    $(wildcard include/config/mtk/mtd/nand.h) \
    $(wildcard include/config/leds/class.h) \
    $(wildcard include/config/pvr/build/release.h) \
    $(wildcard include/config/rtc/intf/dev.h) \
    $(wildcard include/config/mtd/map/bank/width/4.h) \
    $(wildcard include/config/net/activity/stats.h) \
    $(wildcard include/config/nls/default.h) \
    $(wildcard include/config/nf/ct/proto/gre.h) \
    $(wildcard include/config/nf/ct/netlink.h) \
    $(wildcard include/config/crypto/aead2.h) \
    $(wildcard include/config/debug/info.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/crypto/algapi2.h) \
    $(wildcard include/config/zboot/rom/text.h) \
    $(wildcard include/config/input.h) \
    $(wildcard include/config/ip/nf/match/addrtype.h) \
    $(wildcard include/config/kallsyms/extra/pass.h) \
    $(wildcard include/config/proc/sysctl.h) \
    $(wildcard include/config/user/wakelock.h) \
  include/linux/proc_fs.h \
    $(wildcard include/config/proc/devicetree.h) \
    $(wildcard include/config/proc/kcore.h) \
  include/linux/magic.h \

drivers/staging/android/logger.o: $(deps_drivers/staging/android/logger.o)

$(deps_drivers/staging/android/logger.o):
