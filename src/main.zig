const std = @import("std");
const glfw = @import("mach-glfw");
const gl = @import("gl");

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

const positions: [8]f32 = [_]f32{
    -0.5, -0.5,
    0.5,  0.5,
    0.5,  -0.5,
    -0.5, 0.5,
};
const indices: [6]u32 = [_]u32{
    0, 1, 2,
    0, 1, 3,
};

const Square = struct {
    const Self = @This();
    vao: c_uint,
    buffer: c_uint,
    ibo: c_uint,
    color: struct { r: f32, g: f32, b: f32 },

    fn init() Self {
        var vao: c_uint = undefined;
        gl.GenVertexArrays(1, (&vao)[0..1]);
        gl.BindVertexArray(vao);

        var buffer: c_uint = undefined;
        gl.GenBuffers(1, (&buffer)[0..1]);
        gl.BindBuffer(gl.ARRAY_BUFFER, buffer);
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
        gl.EnableVertexAttribArray(0);

        var ibo: c_uint = undefined;
        gl.GenBuffers(1, (&ibo)[0..1]);
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);
        gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, indices.len * @sizeOf(u32), &indices, gl.STATIC_DRAW);
        std.debug.print("VAO: {d}, VBO: {d}, IBO: {d}\n", .{ vao, buffer, ibo });

        return Square{ .vao = vao, .buffer = buffer, .ibo = ibo, .color = .{ .r = 1, .g = 1, .b = 1 } };
    }

    fn setColor(self: *Self, r: f32, g: f32, b: f32) void {
        self.color = .{ .r = r, .g = g, .b = b };
    }

    fn draw(self: Self, program: c_uint) void {
        // std.debug.print("VAO: {d}, VBO: {d}, IBO: {d}\n", .{ self.vao, self.buffer, self.ibo });
        gl.BindVertexArray(self.vao);
        gl.BindBuffer(gl.ARRAY_BUFFER, self.buffer);
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, self.ibo);
        const location = gl.GetUniformLocation(program, "u_Color");
        std.debug.assert(location != -1);
        gl.Uniform4f(location, self.color.r, self.color.g, self.color.b, 1.0);
        gl.DrawElements(gl.TRIANGLES, 6, gl.UNSIGNED_INT, 0);
    }

    fn deinit(self: Self) void {
        var vao: c_uint = self.vao;
        var buffer: c_uint = self.buffer;
        var ibo: c_uint = self.ibo;
        gl.DeleteVertexArrays(1, (&vao)[0..1]);
        gl.BindVertexArray(0);
        gl.DeleteBuffers(1, (&buffer)[0..1]);
        gl.BindBuffer(gl.ARRAY_BUFFER, 0);
        gl.DeleteBuffers(1, (&ibo)[0..1]);
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, 0);
    }
};

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

    var square = Square.init();
    defer square.deinit();
    square.setColor(0.8, 0.3, 0.8);

    // var vao: c_uint = undefined;
    // gl.GenVertexArrays(1, (&vao)[0..1]);
    // gl.BindVertexArray(vao);
    //
    // var buffer: c_uint = undefined;
    // gl.GenBuffers(1, (&buffer)[0..1]);
    // gl.BindBuffer(gl.ARRAY_BUFFER, buffer);
    // gl.BufferData(gl.ARRAY_BUFFER, positions.len * @sizeOf(f32), &positions, gl.STATIC_DRAW);
    // // zig fmt: off
    // gl.VertexAttribPointer(
    //     0, // position in shader
    //     2, // each has 2
    //     gl.FLOAT, // type
    //     gl.FALSE, // no normals translation
    //     2 * @sizeOf(f32), // each vertex has 2 floats
    //     undefined
    // );
    // // zig fmt: on
    // gl.EnableVertexAttribArray(0);
    //
    // var ibo: c_uint = undefined;
    // gl.GenBuffers(1, (&ibo)[0..1]);
    // gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ibo);
    // gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, indices.len * @sizeOf(u32), &indices, gl.STATIC_DRAW);

    const vertexShader = try readFile(allocator, "res/shaders/basic.vertex.shader");
    defer vertexShader.deinit();

    const fragmentShader = try readFile(allocator, "res/shaders/basic.fragment.shader");
    defer fragmentShader.deinit();

    const program = createProgram(vertexShader.items, fragmentShader.items);
    gl.UseProgram(program);

    glCheckError();

    while (!window.shouldClose()) {
        processInput(&window);

        gl.ClearColor(0.0, 0.0, 0.0, 1.0);
        gl.Clear(gl.COLOR_BUFFER_BIT);

        square.draw(program);
        // const location = gl.GetUniformLocation(program, "u_Color");
        // std.debug.assert(location != -1);
        // gl.Uniform4f(location, 0.8, 0.3, 0.8, 1.0);
        // gl.DrawElements(gl.TRIANGLES, 6, gl.UNSIGNED_INT, 0);

        window.swapBuffers();
        glfw.pollEvents();
    }
}
