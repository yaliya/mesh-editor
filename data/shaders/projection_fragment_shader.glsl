#version 330 core

out vec4 fragmentColor;

in vec3 fragmentPosition; // Position from vertex shader
in vec3 fragmentNormal;   // Normal from vertex shader
in vec2 fragmentTexCoords; // Texture coordinates

uniform vec3 lightDir;     // Direction of the sunlight (e.g., normalized direction vector)
uniform vec3 viewPos;      // Camera/viewer position
uniform vec3 lightColor;   // Color of the light (e.g., vec3(1.0, 1.0, 0.9) for warm sunlight)
uniform vec3 objectColor;  // Color of the object
uniform bool useLighting;   // Control whether to apply lighting calculations
uniform bool useMaterial;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
};

uniform Material material;

void main()
{
    vec3 textureColor = useMaterial ? texture(material.diffuse, fragmentTexCoords).rgb : objectColor;
    vec3 specularColor = useMaterial ? texture(material.specular, fragmentTexCoords).rgb : objectColor;

    if (useLighting) {
        // Ambient component
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * lightColor;

        // Normalize the fragment normal and use the light direction directly
        vec3 norm = normalize(fragmentNormal);

        // Diffuse lighting calculation
        float diff = max(dot(norm, -lightDir), 0.0); // `lightDir` points downward
        vec3 diffuse = diff * lightColor * 0.9f; // Adjust diffuse strength if needed

        // Specular component
        float specularStrength = useMaterial ? texture(material.specular, fragmentTexCoords).r : 1.5;
        vec3 viewDir = normalize(viewPos - fragmentPosition);
        vec3 reflectDir = reflect(-lightDir, norm);  // Reflecting from the light source
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 320);
        vec3 specular = specularStrength * spec * lightColor * specularColor;

        // Fresnel effect calculation
        vec3 fresnelBias = normalize(vec3(-1.6, 0.8, 0.2)); // Adjusted bias for top-left shift
        float fresnelFactor = pow(1.0 - max(dot(lightDir + fresnelBias, norm), 0.0), 5.0); // Adjust exponent for strength
        fresnelFactor *= smoothstep(0.0, 1.0, dot(lightDir, fresnelBias)); // Adds falloff from center
        vec3 fresnelReflection = vec3(0.1, 0.1, 0.1) * fresnelFactor; // Darker reflection color to avoid white-out

        // Combine ambient, diffuse, specular, and Fresnel effects
        vec3 result = (ambient + diffuse + specular + fresnelReflection) * textureColor;
        //            vec3 result = (ambient + diffuse) * objectColor;

        // Final color output
        fragmentColor = vec4(clamp(result, 0.0, 1.0), 1.0); // Clamp result to [0, 1]
    } else {
        fragmentColor = vec4(textureColor, 1.0); // No lighting effect
    }
}
