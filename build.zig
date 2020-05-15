const std = @import("std");

pub fn build(b: *std.build.Builder) !void {
    const target = try std.zig.CrossTarget.parse(.{
        .arch_os_abi = "i386-freestanding-eabi", // target platform
        .cpu_features = "baseline", // no special CPU features, especially no SSE!
    });
    // const mode =  b.standardReleaseOptions(); // setup CLI options for release/debug mode
    const mode = .ReleaseFast; // this disables safety checks and ubsan. will make the kernel run, but at the cost of currently existing bugs.

    const kernel = b.addExecutable("kernel", null);
    kernel.setTarget(target);
    kernel.setBuildMode(mode);
    kernel.setLinkerScriptPath("./kernel/src/kernel.ld");
    kernel.addIncludeDir("./kernel/src/include");
    kernel.bundle_compiler_rt = true; // "libgcc"

    for (kernel_sources) |src| {
        kernel.addCSourceFile(src, &[_][]const u8{
            "-Wall",
            "-fno-stack-protector",
        });
    }

    kernel.install(); // build kernel by default

    const user = b.addExecutable("user", null);
    user.setTarget(target);
    user.setBuildMode(mode);
    user.setLinkerScriptPath("./user/src/user.ld");
    user.force_pic = false; // no position-independent

    for (user_sources) |src| {
        user.addCSourceFile(src, &[_][]const u8{
            "-Wall",
            "-fno-stack-protector",
        });
    }

    user.install(); // build user by default

    const run_kernel = b.addSystemCommand(&[_][]const u8{
        "qemu-system-i386",
        //"-S",
        "-no-shutdown",
        "-no-reboot",
        "-d",
        "in_asm",
        "-s",
        "-kernel",
    });
    run_kernel.addArtifactArg(kernel); // add kernel file as CLI option to qemu

    // when using "zig build run", execute the kernel
    const run = b.step("run", "Starts the kernel in QEMU.");
    run.dependOn(&run_kernel.step);
}

// Quellen für deinen Kernel
const kernel_sources = [_][]const u8{
    "kernel/src/init.c",
    "kernel/src/vmm.c",
    "kernel/src/pmm.c",
    "kernel/src/console.c",
    "kernel/src/irq.S",
    "kernel/src/com.c",
    "kernel/src/tasks.c",
    "kernel/src/kbc.c",
    "kernel/src/start.S",
    "kernel/src/commands.c",
    "kernel/src/debug.c",
    "kernel/src/intr.c",
};

// Quellen für dein User-Programm
const user_sources = [_][]const u8{
    "user/src/init.c",
};