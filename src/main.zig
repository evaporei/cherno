const std = @import("std");
const c = @cImport({
    @cInclude("glad/glad.h");
    @cInclude("GLFW/glfw3.h");
});

const ArrayList = std.ArrayList;

fn errorCallback(err: c_int, desc: [*c]const u8) callconv(.C) void {
    std.debug.print("err: {d} {s}", .{ err, desc });
}

fn processInput(window: *c.GLFWwindow) void {
    const state: c_int = c.glfwGetKey(window, c.GLFW_KEY_ESCAPE);
    if (state == c.GLFW_PRESS) {
        c.glfwSetWindowShouldClose(window, 1);
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
    const shader = c.glCreateShader(kind);
    c.glShaderSource(shader, 1, (&src.ptr)[0..1], (&@as(c_int, @intCast(src.len)))[0..1]);
    c.glCompileShader(shader);

    var result: c_int = undefined;
    c.glGetShaderiv(shader, c.GL_COMPILE_STATUS, &result);
    if (result == c.GL_FALSE) {
        var message: [512]u8 = undefined;
        c.glGetShaderInfoLog(shader, message.len, null, &message);
        std.log.err("failed to compile vertex shader: {s}", .{std.mem.sliceTo(&message, 0)});
    }

    return shader;
}

fn createProgram(vertexShader: []const u8, fragmentShader: []const u8) c_uint {
    const program = c.glCreateProgram();

    const vs = compileShader(c.GL_VERTEX_SHADER, vertexShader);
    defer c.glDeleteShader(vs);
    const fs = compileShader(c.GL_FRAGMENT_SHADER, fragmentShader);
    defer c.glDeleteShader(fs);

    c.glAttachShader(program, vs);
    c.glAttachShader(program, fs);
    c.glLinkProgram(program);
    c.glValidateProgram(program);

    return program;
}

fn glCheckError() void {
    var err = c.glGetError();
    while (err != c.GL_NO_ERROR) {
        // cringe, just a number
        std.debug.print("{}", .{err});
        err = c.glGetError();
    }
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    _ = c.glfwSetErrorCallback(errorCallback);

    if (c.glfwInit() != c.GLFW_TRUE) {
        return error.GlfwInit;
    }
    defer c.glfwTerminate();

    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MAJOR, 3);
    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MINOR, 3);
    c.glfwWindowHint(c.GLFW_OPENGL_FORWARD_COMPAT, c.GL_TRUE);
    c.glfwWindowHint(c.GLFW_OPENGL_PROFILE, c.GLFW_OPENGL_CORE_PROFILE);

    const window = c.glfwCreateWindow(640, 480, "cherno", null, null) orelse return error.NoWindow;
    defer c.glfwDestroyWindow(window);

    c.glfwMakeContextCurrent(window);

    _ = c.gladLoadGL();

    c.glfwSwapInterval(1);

    var vao: u32 = undefined;
    c.glGenVertexArrays(1, &vao);
    defer c.glDeleteVertexArrays(1, &vao);

    c.glBindVertexArray(vao);
    defer c.glBindVertexArray(0);

    const positions = [_]f32{
        -0.5, -0.5,
        0.5,  0.5,
        0.5,  -0.5,
        -0.5, 0.5,
    };

    var buffer: u32 = undefined;
    c.glGenBuffers(1, &buffer);
    defer c.glDeleteBuffers(1, &buffer);

    c.glBindBuffer(c.GL_ARRAY_BUFFER, buffer);
    defer c.glBindBuffer(c.GL_ARRAY_BUFFER, 0);
    c.glBufferData(c.GL_ARRAY_BUFFER, positions.len * @sizeOf(f32), &positions, c.GL_STATIC_DRAW);

    // zig fmt: off
    c.glVertexAttribPointer(
        0, // position in shader
        2, // each has 2
        c.GL_FLOAT, // type
        c.GL_FALSE, // no normals translation
        2 * @sizeOf(f32), // each vertex has 2 floats
        @ptrFromInt(@as(usize, @intCast(0)))
    );
    // zig fmt: on
    c.glEnableVertexAttribArray(0);

    const indices = [_]u32{
        0, 1, 2,
        0, 1, 3,
    };

    var ibo: u32 = undefined;
    c.glGenBuffers(1, &ibo);
    c.glBindBuffer(c.GL_ELEMENT_ARRAY_BUFFER, ibo);
    c.glBufferData(c.GL_ELEMENT_ARRAY_BUFFER, indices.len * @sizeOf(u32), &indices, c.GL_STATIC_DRAW);
    defer c.glDeleteBuffers(1, &ibo);
    defer c.glBindBuffer(c.GL_ELEMENT_ARRAY_BUFFER, 0);

    const vertexShader = try readFile(allocator, "res/shaders/basic.vertex.shader");
    defer vertexShader.deinit();

    const fragmentShader = try readFile(allocator, "res/shaders/basic.fragment.shader");
    defer fragmentShader.deinit();

    const program = createProgram(vertexShader.items, fragmentShader.items);
    c.glUseProgram(program);

    const location = c.glGetUniformLocation(program, "u_Color");
    std.debug.assert(location != -1);
    c.glUniform4f(location, 0.8, 0.3, 0.8, 1.0);

    glCheckError();

    var r: f32 = 0;
    var increment: f32 = 0.05;

    while (c.glfwWindowShouldClose(window) == 0) {
        processInput(window);

        c.glClearColor(0.0, 0.0, 0.0, 1.0);
        c.glClear(c.GL_COLOR_BUFFER_BIT | c.GL_DEPTH_BUFFER_BIT);

        c.glUniform4f(location, r, 0.3, 0.8, 1.0);
        c.glDrawElements(c.GL_TRIANGLES, 6, c.GL_UNSIGNED_INT, @ptrFromInt(@as(usize, @intCast(0))));

        if (r > 1.0) {
            increment = -0.05;
        } else if (r < 0.0) {
            increment = 0.05;
        }

        r += increment;

        c.glfwSwapBuffers(window);
        c.glfwPollEvents();
    }
}
