const std = @import("std");
const gl = @import("gl");

const ArrayList = std.ArrayList;

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

const StringHashMap = std.StringHashMap;

pub const Shader = struct {
    openGlId: c_uint,
    uniformLocationCache: StringHashMap(c_int),

    pub fn init(allocator: std.mem.Allocator, vertexShaderPath: []const u8, fragmentShaderPath: []const u8) !Shader {
        const vertexShaderSrc = try readFile(allocator, vertexShaderPath);
        defer vertexShaderSrc.deinit();

        const fragmentShaderSrc = try readFile(allocator, fragmentShaderPath);
        defer fragmentShaderSrc.deinit();

        const program = gl.CreateProgram();

        const vs = compileShader(gl.VERTEX_SHADER, vertexShaderSrc.items);
        defer gl.DeleteShader(vs);
        const fs = compileShader(gl.FRAGMENT_SHADER, fragmentShaderSrc.items);
        defer gl.DeleteShader(fs);

        gl.AttachShader(program, vs);
        gl.AttachShader(program, fs);
        gl.LinkProgram(program);
        gl.ValidateProgram(program);

        return Shader{
            .openGlId = program,
            // we'll barely allocate here,
            // it's fine to use this allocator
            //
            // I actually wanted a hashmap on the stack
            .uniformLocationCache = StringHashMap(c_int).init(std.heap.page_allocator),
        };
    }

    pub fn deinit(self: *Shader) void {
        self.uniformLocationCache.deinit();
    }

    pub fn bind(self: Shader) void {
        gl.UseProgram(self.openGlId);
    }

    fn unbind(_: Shader) void {
        gl.UseProgram(0);
    }

    // TODO: use/create vector library
    pub fn setUniform4f(self: *Shader, name: []const u8, v0: f32, v1: f32, v2: f32, v3: f32) !void {
        const location = try self.getUniformLocation(name);
        gl.Uniform4f(location, v0, v1, v2, v3);
    }

    fn getUniformLocation(self: *Shader, name: []const u8) !c_int {
        if (self.uniformLocationCache.get(name) != null) {
            return self.uniformLocationCache.get(name).?;
        }
        const location = gl.GetUniformLocation(self.openGlId, @ptrCast(name.ptr));
        if (location == -1) {
            std.debug.print("ur not using the uniform bitch\n", .{});
        }
        try self.uniformLocationCache.put(name, location);
        return self.uniformLocationCache.get(name).?;
    }
};
