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

fn compileShader(kind: c_uint, src: []const u8) c_uint {
    const shader = gl.CreateShader(kind);
    gl.ShaderSource(shader, 1, (&src.ptr)[0..1], null);
    gl.CompileShader(shader);

    var result: c_int = undefined;
    gl.GetShaderiv(shader, gl.COMPILE_STATUS, &result);
    if (result == gl.FALSE) {
        var message: [512]u8 = undefined;
        gl.GetShaderInfoLog(shader, message.len, null, &message);
        std.log.err("failed to compile vertex shader: {s}", .{std.mem.sliceTo(&message, 0)});
    }

    return shader;
}

fn createProgram(vertexShader: []const u8, fragmentShader: []const u8) c_uint {
    const program = gl.CreateProgram();

    const vs = compileShader(gl.VERTEX_SHADER, vertexShader);
    defer gl.DeleteShader(vs);
    const fs = compileShader(gl.FRAGMENT_SHADER, fragmentShader);
    defer gl.DeleteShader(fs);

    gl.AttachShader(program, vs);
    gl.AttachShader(program, fs);
    gl.LinkProgram(program);
    gl.ValidateProgram(program);

    return program;
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

    // not in the cherno tutorial, I found out in the comments
    var vao: c_uint = undefined;
    gl.GenVertexArrays(1, (&vao)[0..1]);
    defer gl.DeleteVertexArrays(1, (&vao)[0..1]);

    gl.BindVertexArray(vao);
    defer gl.BindVertexArray(0);

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

    const vertexShader: []const u8 =
        \\ #version 330 core
        \\
        \\ layout (location = 0) in vec4 position;
        \\
        \\ void main()
        \\ {
        \\  gl_Position = position;
        \\ }
    ;

    const fragmentShader: []const u8 =
        \\ #version 330 core
        \\
        \\ layout (location = 0) out vec4 color;
        \\
        \\ void main()
        \\ {
        \\  color = vec4(1.0, 0.0, 0.0, 1.0);
        \\ }
    ;

    const program = createProgram(vertexShader, fragmentShader);
    gl.UseProgram(program);

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
