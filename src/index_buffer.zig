const gl = @import("gl");

pub const IndexBuffer = struct {
    const Self = @This();

    openGlId: c_uint,
    count: u32,

    pub fn init(data: []u32, count: u32) Self {
        var openGlId: c_uint = undefined;
        gl.GenBuffers(1, @ptrCast(&openGlId));
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, openGlId);
        gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, count * @sizeOf(u32), @ptrCast(data), gl.STATIC_DRAW);
        return Self{ .openGlId = openGlId, .count = count };
    }

    pub fn bind(self: Self) void {
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, self.openGlId);
    }

    fn unbind(_: Self) void {
        gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, 0);
    }

    pub fn deinit(self: Self) void {
        var openGlId = self.openGlId;
        gl.DeleteBuffers(1, (&openGlId)[0..1]);
    }
};
