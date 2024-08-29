const gl = @import("gl");

pub const VertexBuffer = struct {
    const Self = @This();

    openGlId: c_uint,

    pub fn init(data: *const anyopaque, size: u32) Self {
        var openGlId: c_uint = undefined;
        gl.GenBuffers(1, (&openGlId)[0..1]);
        gl.BindBuffer(gl.ARRAY_BUFFER, openGlId);
        gl.BufferData(gl.ARRAY_BUFFER, size, data, gl.STATIC_DRAW);
        return Self{ .openGlId = openGlId };
    }

    fn bind(self: Self) void {
        gl.BindBuffer(gl.ARRAY_BUFFER, self.openGlId);
    }

    fn unbind(_: Self) void {
        gl.BindBuffer(gl.ARRAY_BUFFER, 0);
    }

    pub fn deinit(self: Self) void {
        var openGlId = self.openGlId;
        gl.DeleteBuffers(1, (&openGlId)[0..1]);
    }
};
