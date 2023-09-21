#version 450


layout(binding = 0 ) uniform UBO{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
} cameraData;
layout(location =0) in vec3 vertexPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location=1) in vec3 vertexColor;
layout(location=4) in int textureIndex;
layout(location = 0) out vec3 fragColor;
layout(location = 1) out flat int index;
layout(location = 2) out vec2 fragUV;

layout(binding = 1) uniform sampler samp;
layout(binding = 2) uniform texture2D textures[8];

layout(push_constant) uniform constants{
	mat4 transform;
	vec3 color;
} ObjectData;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0,10.0,5.0));
const float AMBIENT = 0.03f;
void main(){
	gl_Position = cameraData.viewProjection * ObjectData.transform * vec4((vertexPosition),1.0);

	vec3 normalWorldSpace = normalize(mat3(ObjectData.transform)*normal);

	float lightIntensity = AMBIENT + max(dot(normalWorldSpace,DIRECTION_TO_LIGHT),0);
	//gl_Position = vec4(vertexPosition,1.0);

	index = textureIndex;
	fragUV = uv;
	fragColor = lightIntensity*vertexColor;
}	