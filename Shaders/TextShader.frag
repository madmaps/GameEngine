#version 410
uniform sampler2D text_texture;
uniform vec3 inColor;
in vec2 texture_coordinates;
out vec4 frag_color;


void main()
{
    vec4 texel = texture(text_texture, texture_coordinates);
    if(texel.r > 0.99 && texel.g < 0.01 && texel.b > 0.99)
    {
        discard;
    }
    frag_color = vec4(inColor, 1.0);
    //frag_color = vec4(1.0,1.0,1.0,1.0);

}
