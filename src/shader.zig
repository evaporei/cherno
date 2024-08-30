const std = @import("std");
const gl = @import("gl");

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

pub const Shader = struct {
    openGlId: c_uint,

    pub fn init(vertexShaderSrc: []const u8, fragmentShaderSrc: []const u8) Shader {
        const program = gl.CreateProgram();

        const vs = compileShader(gl.VERTEX_SHADER, vertexShaderSrc);
        defer gl.DeleteShader(vs);
        const fs = compileShader(gl.FRAGMENT_SHADER, fragmentShaderSrc);
        defer gl.DeleteShader(fs);

        gl.AttachShader(program, vs);
        gl.AttachShader(program, fs);
        gl.LinkProgram(program);
        gl.ValidateProgram(program);

        return Shader{
            .openGlId = program,
        };
    }

    pub fn bind(self: Shader) void {
        gl.UseProgram(self.openGlId);
    }

    fn unbind(_: Shader) void {
        gl.UseProgram(0);
    }

    // TODO: use/create vector library
    pub fn setUniform4f(self: Shader, name: []const u8, v0: f32, v1: f32, v2: f32, v3: f32) void {
        const location = gl.GetUniformLocation(self.openGlId, @ptrCast(name.ptr));
        if (location == -1) {
            std.debug.print("ur not using the uniform bitch\n", .{});
        }
        gl.Uniform4f(location, v0, v1, v2, v3);
    }
};
