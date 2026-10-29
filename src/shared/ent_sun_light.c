#include "entity.h"
#include "quark.h"

void entity_init_sun_light(quark_t* quark,
    const char* name,
    float x, float y, float z,
    vec3 dir, vec3 color, entity_t** out)
{
	if (!out) return;

	entity_t* entity = NULL;
	entity_init_common(quark, name, ENTITY_SUN_LIGHT, (vec3){x, y, z}, &entity);
	glm_vec3_copy(dir, entity->data.sun_light.direction);
	glm_normalize(entity->data.sun_light.direction);
	glm_vec3_copy(color, entity->data.sun_light.color);
	glm_mat4_identity(entity->data.sun_light.matrix);

	*out = entity;
}
