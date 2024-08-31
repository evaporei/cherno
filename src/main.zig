const std = @import("std");
const glfw = @import("mach-glfw");
const gl = @import("gl");

const root = @import("root.zig");
const VertexArray = root.VertexArray;
const VertexBuffer = root.VertexBuffer;
const IndexBuffer = root.IndexBuffer;
const Shader = root.Shader;
const Renderer = root.Renderer;
const Texture = root.Texture;

fn errorCallback(error_code: glfw.ErrorCode, description: [:0]const u8) void {
    std.log.err("glfw: {}: {s}\n", .{ error_code, description });
}

fn processInput(window: *const glfw.Window) void {
    if (window.getKey(glfw.Key.escape) == glfw.Action.press) {
        window.setShouldClose(true);
    }
}

// https://www.khronos.org/opengl/wiki/OpenGL_Error#Meaning_of_errors
/// GL_INVALID_ENUM, 0x0500
// Given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.
/// GL_INVALID_VALUE, 0x0501
// Given when a value parameter is not a legal value for that function. This is only given for local problems; if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.
/// GL_INVALID_OPERATION, 0x0502
// Given when the set of state for a command is not legal for the parameters given to that command. It is also given for commands where combinations of parameters define what the legal parameters are.
/// GL_STACK_OVERFLOW, 0x0503
// Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size.
/// GL_STACK_UNDERFLOW, 0x0504
// Given when a stack popping operation cannot be done because the stack is already at its lowest point.
/// GL_OUT_OF_MEMORY, 0x0505
// Given when performing an operation that can allocate memory, and the memory cannot be allocated. The results of OpenGL functions that return this error are undefined; it is allowable for partial execution of an operation to happen in this circumstance.
/// GL_INVALID_FRAMEBUFFER_OPERATION, 0x0506
// Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.
/// GL_CONTEXT_LOST, 0x0507 (with OpenGL 4.5 or ARB_KHR_robustness)
// Given if the OpenGL context has been lost, due to a graphics card reset.
/// GL_TABLE_TOO_LARGE1, 0x8031
// Part of the ARB_imaging extension.
fn glCheckError() void {
    var err = gl.GetError();
    while (err != gl.NO_ERROR) {
        std.debug.print("error code 0x{x}: ", .{err});
        switch (err) {
            gl.INVALID_ENUM => std.debug.print("enum param not legal\n", .{}),
            gl.INVALID_VALUE => std.debug.print("value param not legal\n", .{}),
            gl.INVALID_OPERATION => std.debug.print("set state is ilegal for given command\n", .{}),
            // gl.STACK_OVERFLOW => std.debug.print("stack pushing operation overflown (over stack size...)\n", .{}),
            // gl.STACK_UNDERFLOW => std.debug.print("stack popping operation underflown (already at lowest point)\n", .{}),
            gl.OUT_OF_MEMORY => std.debug.print("memory could not be allocated (command needed to allocate and failed)\n", .{}),
            gl.INVALID_FRAMEBUFFER_OPERATION => std.debug.print("failed to read/write to framebuffer (it's incomplete)\n", .{}),
            // gl.TABLE_TOO_LARGE => std.debug.print("this shouldn't happen (table too large)\n", .{}),
            else => std.debug.print("wtf did I just receive??\n", .{}),
        }
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

    const positions = [_]f32{
        -0.5, -0.5, 0.0, 0.0,
        0.5,  -0.5, 1.0, 0.0,
        0.5,  0.5,  1.0, 1.0,
        -0.5, 0.5,  0.0, 1.0,
    };

    const indices = [_]u32{
        0, 1, 2,
        2, 3, 0,
    };

    std.debug.print("{s}\n", .{gl.GetString(gl.VERSION).?});

    gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
    gl.Enable(gl.BLEND);

    var vertexArray = VertexArray.init();
    defer vertexArray.deinit();

    const vertexBuffer = VertexBuffer.init(@constCast(&positions), positions.len * @sizeOf(f32));
    defer vertexBuffer.deinit();

    const indexBuffer = IndexBuffer.init(@constCast(&indices), 6);
    defer indexBuffer.deinit();

    var layout = VertexBuffer.Layout.init(allocator);
    defer layout.deinit();
    try layout.push(f32, 2);
    try layout.push(f32, 2);

    vertexArray.addBuffer(vertexBuffer, layout);

    var shader = try Shader.init(allocator, "res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");
    defer shader.deinit();
    shader.bind();

    var texture = try Texture.init("res/textures/cherno_logo.png");
    defer texture.deinit();

    texture.bind(0);
    try shader.setUniform1i("u_Texture", 0);

    const renderer = Renderer.init();

    try shader.setUniform4f("u_Color", 0.8, 0.3, 0.8, 1.0);

    glCheckError();

    var r: f32 = 0;
    var increment: f32 = 0.05;

    while (!window.shouldClose()) {
        processInput(&window);

        renderer.clear();

        try shader.setUniform4f("u_Color", r, 0.3, 0.8, 1.0);
        renderer.draw(vertexArray, indexBuffer, shader);

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
