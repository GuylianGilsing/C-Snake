#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_TransformMatrix;

void main()
{
   gl_Position = u_ProjectionMatrix * u_TransformMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}