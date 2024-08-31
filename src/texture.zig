const std = @import("std");
const gl = @import("gl");
const stb = @import("stb-image");

const ArrayList = std.ArrayList;

pub const Texture = struct {
    openGlId: c_uint,
    filePath: []const u8,
    // buffer: []const u8,
    width: c_int,
    height: c_int,
    // bytes per pixel
    bpp: c_int,

    pub fn init(path: []const u8) !Texture {
        stb.set_flip_vertically_on_load(1);
        var img = try stb.load_image(path, 4);
        defer stb.free_image(&img);

        var openGlId: c_uint = undefined;
        gl.GenTextures(1, (&openGlId)[0..1]);
        gl.BindTexture(gl.TEXTURE_2D, openGlId);

        gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

        gl.TexImage2D(gl.TEXTURE_2D, 0, gl.RGBA8, img.width, img.height, 0, gl.RGBA, gl.UNSIGNED_BYTE, img.data);
        gl.BindTexture(gl.TEXTURE_2D, 0);

        return Texture{
            .openGlId = openGlId,
            .filePath = path,
            .width = img.width,
            .height = img.height,
            .bpp = img.nchan,
        };
    }

    pub fn deinit(self: Texture) void {
        // var img = stb.Image{ .data = self.data };
        // img.deinit();
        var openGlId = self.openGlId;
        gl.DeleteTextures(1, (&openGlId)[0..1]);
    }

    pub fn bind(self: Texture, slot: c_uint) void {
        gl.ActiveTexture(gl.TEXTURE0 + slot);
        gl.BindTexture(gl.TEXTURE_2D, self.openGlId);
    }

    fn unbind(_: Texture) void {
        gl.BindTexture(gl.TEXTURE_2D, 0);
    }
};
