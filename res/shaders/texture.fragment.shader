#version 330 core

in vec2 v_TexCoord;

layout (location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    //color = u_Color;
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}
