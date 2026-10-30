//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform float ambientStrength;

uniform vec3 dirLight_direction;
uniform vec3 dirLight_color;
uniform float dirLight_intensity;
uniform bool dirLight_enabled;

vec3 calc_dir_light(vec3 normal, vec3 view_dir, vec3 base_color)
{
    if (!dirLight_enabled) {
        return vec3(0.0);
    }
    vec3 light_dir = normalize(-dirLight_direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 halfway = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway), 0.0), 32.0);

    vec3 ambient = 0.1 * base_color;
    vec3 diffuse = diff * base_color;
    vec3 specular = spec * vec3(0.3);
    return (ambient + diffuse + specular) * dirLight_color * dirLight_intensity;
}

void main()
{
    vec3 base_color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 view_dir = normalize(viewPos - FragPos);

    vec3 ambient = ambientStrength * ambientColor * base_color;

    vec3 result = ambient + calc_dir_light(normal, view_dir, base_color);

    FragColor = vec4(result, 1.0);
}