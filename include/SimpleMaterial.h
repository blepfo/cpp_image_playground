#ifndef _SIMPLE_MATERIAL_H
#define _SIMPLE_MATERIAL_H

namespace SimpleMaterial {

class Material {
	public:
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 ambient;
		glm::vec3 emission;
        float shiny;

		Material(
            glm::vec3 diffuse, 
            glm::vec3 specular, 
            glm::vec3 ambient, 
            glm::vec3 emission, 
            float shiny
        )
			: diffuse(diffuse), specular(specular), ambient(ambient), emission(emission), shiny(shiny) {}
};

} // namespace SimpleMaterial

#endif
