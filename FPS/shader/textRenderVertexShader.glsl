#version 120

attribute vec2 vertexPosition_screenspace;
attribute vec2 vertexUV;

varying vec2 UV;

void main()
{
    vec2 vertexPosition_homogeneousspace =
        vertexPosition_screenspace - vec2(1440, 900);
    vertexPosition_homogeneousspace /= vec2(1440, 900);
    gl_Position = vec4(vertexPosition_homogeneousspace, 0, 1);

    UV = vertexUV;
}
