#version 330 core
out vec4 FragColor;

// Gets the color from C++
uniform vec4 u_Color;

void main()
{
   FragColor = u_Color;//vec4(1.0f, 0.5f, 0.2f, 1.0f);
};