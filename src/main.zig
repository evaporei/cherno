const std = @import("std");
const glfw = @import("mach-glfw");
const gl = @import("gl");

fn errorCallback(error_code: glfw.ErrorCode, description: [:0]const u8) void {
    std.log.err("glfw: {}: {s}\n", .{ error_code, description });
}

fn processInput(window: *const glfw.Window) void {
    if (window.getKey(glfw.Key.escape) == glfw.Action.press) {
        window.setShouldClose(true);
    }
}

var gl_procs: gl.ProcTable = undefined;

pub fn main() !void {
    glfw.setErrorCallback(errorCallback);
    if (!glfw.init(.{})) {
        std.log.err("failed to initialize GLFW: {?s}", .{glfw.getErrorString()});
        std.process.exit(1);
    }
    defer glfw.terminate();

    const window = glfw.Window.create(640, 480, "cherno opengl tutorial", null, null, .{
        .context_version_major = gl.info.version_major,
        .context_version_minor = gl.info.version_minor,
        .opengl_profile = .opengl_core_profile,
        .opengl_forward_compat = true,
    }) orelse {
        std.log.err("failed to create GLFW window: {?s}", .{glfw.getErrorString()});
        std.process.exit(1);
    };
    defer window.destroy();

    glfw.makeContextCurrent(window);
    defer glfw.makeContextCurrent(null);

    // enables VSync, to avoid unnecessary drawing
    glfw.swapInterval(1);

    if (!gl_procs.init(glfw.getProcAddress)) {
        std.log.err("failed to load OpenGL functions: {?s}", .{glfw.getErrorString()});
        std.process.exit(1);
    }

    gl.makeProcTableCurrent(&gl_procs);
    defer gl.makeProcTableCurrent(null);

    const positions = [_]f32{
        -0.5, -0.5,
        0.0,  0.5,
        0.5,  -0.5,
    };

    var buffer: c_uint = undefined;
    // create(how many, id)
    gl.GenBuffers(1, (&buffer)[0..1]);
    defer gl.DeleteBuffers(1, (&buffer)[0..1]);
    // select
    gl.BindBuffer(gl.ARRAY_BUFFER, buffer);
    defer gl.BindBuffer(gl.ARRAY_BUFFER, 0);
    // load buffer to GPU
    gl.BufferData(gl.ARRAY_BUFFER, positions.len * @sizeOf(f32), &positions, gl.STATIC_DRAW);

    // zig fmt: off
    gl.VertexAttribPointer(
        0, // position in shader
        2, // each has 2
        gl.FLOAT, // type
        gl.FALSE, // no normals translation
        2 * @sizeOf(f32), // each vertex has 2 floats
        undefined
    );
    // zig fmt: on
    // enable attrib position 0 (above)
    gl.EnableVertexAttribArray(0);

    while (!window.shouldClose()) {
        processInput(&window);

        gl.ClearColor(0.0, 0.0, 0.0, 1.0);
        gl.Clear(gl.COLOR_BUFFER_BIT);

        // (from index zero, count = 3)
        // nothing yet
        gl.DrawArrays(gl.TRIANGLES, 0, 3);

        window.swapBuffers();
        glfw.pollEvents();
    }
}
