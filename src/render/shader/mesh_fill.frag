#version 330 core
out vec4 FragColor;

in vec3 normal;  
in vec3 pos; 
in vec3 color;
  
uniform vec3 light_pos; 
uniform vec3 view_pos; 
uniform vec3 light_color;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;
  	
    // diffuse 
    float diffuseStrength = 0.8;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light_pos - pos);
    float diff = abs(dot(norm, lightDir));
    vec3 diffuse = diffuseStrength * diff * light_color;
    
    // specular
    float specularStrength = 0.1;
    vec3 viewDir = normalize(view_pos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * light_color;  
        
    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.f);
}