#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

const int NUM_LS =  5;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {     	// base alignment // aligned offset // total bytes
    vec3 position;              // 16             0                 16

    vec3 color;                 // 16             16                32

    float ambient;              // 4              32                36
    float diffuse;              // 4              36                40
    float specular;             // 4              40                44

    float constant;             // 4              44                48
    float linear;               // 4              48                52
    float quadratic;            // 4              52                56
};                              // 16                               64


layout (location = 0) in vec3 vFragPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexPosition;

layout (location = 3) uniform vec3 uViewPos;
layout (location = 4) uniform vec3 uBaseColor;
layout (location = 5) uniform sampler2D uTexture;
layout (location = 6) uniform float uShininess;
layout (location = 7) uniform samplerCube uCubeMap;
layout (location = 8) uniform Material uMaterial;

layout (std140) uniform lightBlock {
    PointLight lights[5];
};

layout (location = 0) out vec4 fragColor;

vec3 calcPhongLight(PointLight light, vec3 viewDirection, vec3 lightDirection) {
    vec3 reflectDirection = reflect(-lightDirection, vNormal);
    float shininess = uMaterial.shininess * uShininess * 128.0 + 17.0;
    float specular = pow(max(dot(vNormal, reflectDirection), 0.0), uMaterial.shininess * uShininess * 4.0);
    float cos_theta = cos(acos( dot(lightDirection, normalize(vNormal)) / length(lightDirection) * length(normalize(vNormal))));
    if (cos_theta < 0.0) {
        cos_theta = 0.0;
    }
    float distanceToLightSource = distance(light.position, vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLightSource + light.quadratic * pow(distanceToLightSource, 2.0));

    // ambient
    vec3 result = light.ambient * uMaterial.ambient;
    // diffuse
    result += light.diffuse * cos_theta * uMaterial.diffuse;
    // specular
    result += light.specular * specular * uMaterial.specular;

    return result * attenuation * light.color;

}

void main() {
    vec3 viewDirection = normalize(uViewPos - vFragPos);

    vec3 refr = vec3(0.0, 0.0, 0.0);
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < NUM_LS; i += 1) {

        vec3 lightDirection = normalize(lights[i].position - vFragPos);
        refr += refract(-lightDirection, normalize(vNormal), 1.33);
        result += calcPhongLight(lights[i], viewDirection, lightDirection);
    }
    vec3 refl = reflect(viewDirection, vNormal);
    vec4 texel = texture(uCubeMap, refr);

    //fragColor = texel * vec4(uBaseColor, 1.0) * vec4(result, 1.0);
    fragColor = texel * vec4(result, 1.0);
    //fragColor = vec4(result, 1.0);
    //fragColor = vec4(lights[0].ambient, lights[0].diffuse, lights[0].specular, 1.0);
    //fragColor = vec4(uMaterial.ambient + uMaterial.diffuse + uMaterial.specular, 1.0);
}

