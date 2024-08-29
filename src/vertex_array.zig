const gl = @import("gl");

const root = @import("root.zig");
const VertexBuffer = root.VertexBuffer;
pub const VertexArray = struct {
    openGlId: c_uint,

    pub fn init() VertexArray {
        var openGlId: c_uint = undefined;
        gl.GenVertexArrays(1, (&openGlId)[0..1]);
        gl.BindVertexArray(openGlId);
        return VertexArray{ .openGlId = openGlId };
    }

    fn bind(self: *VertexArray) void {
        gl.BindVertexArray(self.openGlId);
    }

    fn unbind(_: *VertexArray) void {
        gl.BindVertexArray(0);
    }

    pub fn addBuffer(self: *VertexArray, buffer: VertexBuffer, layout: VertexBuffer.Layout) void {
        self.bind();
        buffer.bind();
        var offset: c_int = 0;
        for (0.., layout.elements.items) |i, element| {
            // zig fmt: off
            gl.VertexAttribPointer(
                @intCast(i), // position in shader
                element.count,
                element.type,
                element.normalized,
                layout.stride,
                @intCast(offset)
            );
            // zig fmt: on
            gl.EnableVertexAttribArray(@intCast(i));
            offset += element.count * VertexBuffer.Element.sizeOfType(element.type);
        }
    }

    pub fn deinit(self: *VertexArray) void {
        var vao = self.openGlId;
        gl.DeleteVertexArrays(1, (&vao)[0..1]);
    }
};
