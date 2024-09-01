const std = @import("std");
const c = @cImport({
    @cInclude("glad/glad.h");
    @cInclude("GLFW/glfw3.h");
});

fn errorCallback(err: c_int, desc: [*c]const u8) callconv(.C) void {
    std.debug.print("err: {d} {s}", .{ err, desc });
}

fn processInput(window: *c.GLFWwindow) void {
    const state: c_int = c.glfwGetKey(window, c.GLFW_KEY_ESCAPE);
    if (state == c.GLFW_PRESS) {
        c.glfwSetWindowShouldClose(window, 1);
    }
}

pub fn main() !void {
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

    while (c.glfwWindowShouldClose(window) == 0) {
        processInput(window);

        c.glClearColor(0.0, 0.0, 0.0, 1.0);
        c.glClear(c.GL_COLOR_BUFFER_BIT | c.GL_DEPTH_BUFFER_BIT);

        c.glfwSwapBuffers(window);
        c.glfwPollEvents();
    }
}
