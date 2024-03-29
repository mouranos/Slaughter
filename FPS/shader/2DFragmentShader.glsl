#version 120

varying vec2 UV;

uniform sampler2D myTextureSampler;

void main()
{
    vec4 fc;
    fc = texture2D(myTextureSampler, UV);
    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, fc);
}
