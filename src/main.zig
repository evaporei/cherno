const std = @import("std");
const glfw = @import("mach-glfw");
const gl = @import("gl");

const root = @import("root.zig");
const VertexBuffer = root.VertexBuffer;
const IndexBuffer = root.IndexBuffer;

const ArrayList = std.ArrayList;

fn errorCallback(error_code: glfw.ErrorCode, description: [:0]const u8) void {
    std.log.err("glfw: {}: {s}\n", .{ error_code, description });
}

fn processInput(window: *const glfw.Window) void {
    if (window.getKey(glfw.Key.escape) == glfw.Action.press) {
        window.setShouldClose(true);
    }
}

fn readFile(allocator: std.mem.Allocator, path: []const u8) !ArrayList(u8) {
    var contents = ArrayList(u8).init(allocator);

    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var reader = file.reader();

    while (true) {
        var buf: [1024]u8 = undefined;
        const len = try reader.read(&buf);
        if (len == 0) break;
        try contents.appendSlice(buf[0..len]);
    }

    return contents;
}

fn compileShader(kind: c_uint, src: []const u8) c_uint {
    const shader = gl.CreateShader(kind);
    gl.ShaderSource(shader, 1, (&src.ptr)[0..1], (&@as(c_int, @intCast(src.len)))[0..1]);
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

fn glCheckError() void {
    var err = gl.GetError();
    while (err != gl.NO_ERROR) {
        // cringe, just a number
        std.debug.print("{}", .{err});
        err = gl.GetError();
    }
}

var gl_procs: gl.ProcTable = undefined;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

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
        0.5,  0.5,
        0.5,  -0.5,
        -0.5, 0.5,
    };

    const indices = [_]u32{
        0, 1, 2,
        0, 1, 3,
    };

    const vertexBuffer = VertexBuffer.init(@constCast(&positions), positions.len * @sizeOf(f32));
    defer vertexBuffer.deinit();

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

    const indexBuffer = IndexBuffer.init(@constCast(&indices), 6);
    defer indexBuffer.deinit();

    const vertexShader = try readFile(allocator, "res/shaders/basic.vertex.shader");
    defer vertexShader.deinit();

    const fragmentShader = try readFile(allocator, "res/shaders/basic.fragment.shader");
    defer fragmentShader.deinit();

    const program = createProgram(vertexShader.items, fragmentShader.items);
    gl.UseProgram(program);

    const location = gl.GetUniformLocation(program, "u_Color");
    std.debug.assert(location != -1);
    gl.Uniform4f(location, 0.8, 0.3, 0.8, 1.0);

    glCheckError();

    var r: f32 = 0;
    var increment: f32 = 0.05;

    while (!window.shouldClose()) {
        processInput(&window);

        gl.ClearColor(0.0, 0.0, 0.0, 1.0);
        gl.Clear(gl.COLOR_BUFFER_BIT);

        gl.Uniform4f(location, r, 0.3, 0.8, 1.0);
        gl.DrawElements(gl.TRIANGLES, 6, gl.UNSIGNED_INT, 0);

        if (r > 1.0) {
            increment = -0.05;
        } else if (r < 0.0) {
            increment = 0.05;
        }

        r += increment;

        window.swapBuffers();
        glfw.pollEvents();
    }
}
