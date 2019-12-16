const std = @import("std");
const builtin = @import("builtin");
const Builder = @import("std").build.Builder;

const cflags = &[_][]const u8{
    "-Wall",
    "-Wextra",
    "-fno-stack-protector",
};

pub fn build(b: *Builder) void {
    const mode = b.standardReleaseOptions();
    const kernel = b.addExecutable("kernel", null);

    kernel.addCSourceFile("init.c", cflags);
    kernel.addCSourceFile("console.c", cflags);
    kernel.addAssemblyFile("start.S");

    kernel.setBuildMode(mode);
    kernel.setLinkerScriptPath("kernel.ld");
    kernel.setTheTarget(std.build.Target{
        .Cross = std.build.CrossTarget{
            .arch = builtin.Arch.i386,
            .os = builtin.Os.freestanding,
            .abi = builtin.Abi.eabi,
        },
    });

    kernel.install();

    const qemu_step = b.addSystemCommand(&[_][]const u8{
        "qemu-system-i386",
        "-kernel",
        "zig-cache/bin/kernel",
    });
    qemu_step.step.dependOn(&kernel.step);

    const run_step = b.step("run", "Run the OS with QEMU");
    run_step.dependOn(&qemu_step.step);
}
