#version 120

varying vec2 UV;

uniform sampler2D myTextureSampler;

void main()
{
    vec4 fc;
    fc = texture2D(myTextureSampler, UV);
    gl_FragColor = vec4(0.0, 0.5, 1.0, fc);
}
