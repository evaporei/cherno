const gl = @import("gl");

const root = @import("root.zig");
const VertexArray = root.VertexArray;
const IndexBuffer = root.IndexBuffer;
const Shader = root.Shader;

pub const Renderer = struct {
    pub fn init() Renderer {
        return Renderer{};
    }

    pub fn clear(_: Renderer) void {
        // gl.ClearColor(0.0, 0.0, 0.0, 1.0);
        gl.Clear(gl.COLOR_BUFFER_BIT);
    }

    pub fn draw(_: Renderer, vao: VertexArray, ibo: IndexBuffer, shader: Shader) void {
        shader.bind();
        vao.bind();
        ibo.bind();
        gl.DrawElements(gl.TRIANGLES, @intCast(ibo.count), gl.UNSIGNED_INT, 0);
    }
};
