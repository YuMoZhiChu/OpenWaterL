#version 330 core
out vec4 FragColor;

in vec3 ourColor;

// scale = (sin(time)/2.0f)+0.5f [0,1] 之间波动
uniform float color_scale;

void main()
{
	// 随着时间波动
	vec3 rever_color = vec3(1.0f-ourColor.x, 1.0f-ourColor.y, 1.0f-ourColor.z);
    FragColor = vec4(ourColor*(1-color_scale)+rever_color*color_scale, 1.0f);
}