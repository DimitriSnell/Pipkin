#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec3 fragColor;
layout(location = 1) flat in int  index;
layout(location = 2) in vec2 fragUv;
layout(binding = 1) uniform sampler samp;
layout(binding = 2) uniform texture2D textures[8];

void main(){
	if(index == 0){
		outColor = vec4(fragColor*(texture(sampler2D(textures[index],samp),fragUv)).rgb,1.0); 
	}
	else{
		outColor = vec4(fragColor,1.0);
	}
}