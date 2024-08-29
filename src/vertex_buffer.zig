const std = @import("std");
const gl = @import("gl");

const ArrayList = std.ArrayList;

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

    pub fn bind(self: Self) void {
        gl.BindBuffer(gl.ARRAY_BUFFER, self.openGlId);
    }

    pub fn unbind(_: Self) void {
        gl.BindBuffer(gl.ARRAY_BUFFER, 0);
    }

    pub fn deinit(self: Self) void {
        var openGlId = self.openGlId;
        gl.DeleteBuffers(1, (&openGlId)[0..1]);
    }

    pub const Element = struct {
        type: c_uint,
        count: c_int,
        normalized: u8,

        pub fn sizeOfType(typ: c_uint) c_int {
            return switch (typ) {
                gl.FLOAT => 4,
                gl.UNSIGNED_INT => 4,
                gl.UNSIGNED_BYTE => 1,
                else => unreachable,
            };
        }
    };

    pub const Layout = struct {
        allocator: std.mem.Allocator,
        elements: ArrayList(Element),
        stride: c_int,

        pub fn init(allocator: std.mem.Allocator) Layout {
            return Layout{ .allocator = allocator, .elements = ArrayList(Element).init(allocator), .stride = 0 };
        }

        pub fn push(self: *Layout, comptime T: type, count: c_int) !void {
            var normalized: u8 = gl.FALSE;
            const typ = switch (T) {
                f32 => gl.FLOAT,
                u32 => gl.UNSIGNED_INT,
                u8 => {
                    normalized = gl.TRUE;
                    return gl.UNSIGNED_BYTE;
                },
                else => @compileError("unsupported type"),
            };
            self.stride += Element.sizeOfType(typ) * count;
            try self.elements.append(Element{
                .count = count,
                .type = typ,
                .normalized = normalized,
            });
        }

        pub fn deinit(self: *Layout) void {
            self.elements.deinit();
        }
    };
};
