const std = @import("std");
const builtin = @import("builtin");
const Builder = @import("std").build.Builder;

const cflags = &[_][]const u8{
    "-Wall",
    "-Wextra",
    "-fno-stack-protector",
    "-fno-omit-frame-pointer",
    };


pub fn build(b: *Builder) void {
    const mode = b.standardReleaseOptions();
    const kernel = b.addExecutable("kernel", null);

    kernel.addIncludeDir("include");
    kernel.addCSourceFile("src/init.c", cflags);
    kernel.addCSourceFile("src/console.c", cflags);
    kernel.addCSourceFile("src/intr.c", cflags);
    kernel.addCSourceFile("src/kbc.c", cflags);
    kernel.addCSourceFile("src/commands.c", cflags);
    kernel.addCSourceFile("src/tasks.c", cflags);
    kernel.addCSourceFile("src/debug.c", cflags);
    kernel.addCSourceFile("src/pmm.c", cflags);
    kernel.addAssemblyFile("src/start.S");
    kernel.addAssemblyFile("src/irq.S");

    kernel.setBuildMode(mode);
    kernel.setLinkerScriptPath("src/kernel.ld");
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
    });
    qemu_step.addArtifactArg(kernel);
    qemu_step.step.dependOn(&kernel.step);
    const run_step = b.step("run", "Run the OS with QEMU");
    run_step.dependOn(&qemu_step.step);
}