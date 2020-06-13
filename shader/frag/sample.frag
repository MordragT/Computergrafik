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



float angleBetween(vec3 vector1, vec3 vector2) {
    float dotProduct = dot(vector1, vector2);
    float lengthProduct = length(vector1) * length(vector2);
    return acos( dotProduct / lengthProduct );
}

vec3 calcPhongLight(PointLight light, vec3 viewDirection) {

    // Calculate viewDirection & lightDirection -> normalise
    vec3 lightDirection = normalize(light.position - vFragPos);

    /* Intensity vectors */

    // Calculate the refraction angle for glass
    vec3 R = refract(-lightDirection, normalize(vNormal), 1.52);

    // Calculate the reflect direction for the fragment
    vec3 reflectDirection = reflect(-lightDirection, vNormal);

    // Material shininess
    float shininess = uMaterial.shininess * uShininess * 128.0 + 17.0;

    // Calculate the specular factor
    //float spec = pow(max(dot(vNormal, reflectDirection), 0.0), uMaterial.shininess * uShininess * 128.0);
    float spec = pow(max(dot(vNormal, reflectDirection), 0.0), 4.0);
    //fragColor = vec4(vec3(spec), 1.0);

    // Calculate the cosine of the angle theta
    float cos_theta = cos(angleBetween(lightDirection, normalize(vNormal)));

    // If tho cosine of theta is negative, set it to zero
    cos_theta = cos_theta < 0.0 ? 0.0 : cos_theta;

    // Calculate the intensity values ambient, diffuse and specular
    vec3 iAmbient  = light.ambient *             uMaterial.ambient;
    vec3 iDiffuse  = light.diffuse * cos_theta * uMaterial.diffuse;
    vec3 iSpecular = light.specular * spec *     uMaterial.specular;


    /* Attenuation */

    // Calculate the distance between this fragment and the light source
    float dist_ls = distance(light.position, vFragPos);

    // Calculate the attenuation value for the fragment
    float attenuation = 1.0 / (light.constant + light.linear * dist_ls + light.quadratic * pow(dist_ls, 2.0));

    /* The fragment's light intensity vector */

    // Return the light intensity vector
    return (iAmbient + iDiffuse + iSpecular) * attenuation * light.color;

}

void main() {
    vec3 viewDirection = normalize(uViewPos - vFragPos);

    vec3 R = reflect(viewDirection, vNormal);
    vec4 texel = texture(uCubeMap, R);
    //vec4 texel = texture(uTexture, vTexPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < NUM_LS; i += 1) {
        // Calculate Phong-Illumination: vec3 result
        result += calcPhongLight(lights[i], viewDirection);
    }

    // Calculate the fragment color
    //fragColor = texel * vec4(uBaseColor, 1.0) * vec4(result, 1.0);
    fragColor = vec4(result, 1.0);
    //fragColor = vec4(lights[0].ambient, lights[0].diffuse, lights[0].specular, 1.0);
    //fragColor = vec4(uMaterial.ambient + uMaterial.diffuse + uMaterial.specular, 1.0);
}

