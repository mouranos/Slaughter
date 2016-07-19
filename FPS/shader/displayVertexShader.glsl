
attribute vec3 vertexPositionModelSpace;

varying vec4 ambient;
varying vec4 diffuse;
varying vec4 transformedMatrix;

uniform mat4 MVP;

void main(){

    gl_Position = transformedMatrix * vec4(vertexPositionModelSpace,1);

}
