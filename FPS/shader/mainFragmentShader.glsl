#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
    gl_FragColor = texture2D(myTextureSampler, UV);
}
