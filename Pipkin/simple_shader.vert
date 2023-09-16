#version 450

layout(binding = 0 ) uniform UBO{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
} cameraData;
layout(location =0) in vec3 vertexPosition;
layout(location=1) in vec3 vertexColor;
layout(location = 0) out vec3 fragColor;
layout(push_constant) uniform constants{
	mat4 transform;
	vec3 color;
} ObjectData;
void main(){
	gl_Position = cameraData.projection*cameraData.view * ObjectData.transform * vec4((vertexPosition),1.0);
	//gl_Position = vec4(vertexPosition,1.0);
	fragColor = vertexColor;
}	