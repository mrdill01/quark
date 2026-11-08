#include "entity.h"
#include "quark.h"

void entity_init_pickup(quark_t* quark,
    const char* name,
    vec3 position,
    mesh_t* mesh,
    pickup_type_t type,
    void* pickup_sound,
    entity_t** out)
{
    entity_t* entity = NULL;
	entity_init_common(quark, name, ENTITY_PICKUP, position, &entity);
    entity->data.pickup.mesh = mesh;
    for (int i = 0; i < MAX_MATERIALS; i++)
        entity->data.pickup.materials[i] = NULL;
    entity->data.pickup.type = type;
    entity->data.pickup.pickup_sound = pickup_sound;
    entity->data.pickup.can_pickup = true;
    entity->data.pickup.pickup_time = 0.0f;
    
    vec3 light_position = {position[0], position[1] + 0.5f, position[2]};

    vec3 color = {16.0f, 64.0f, 16.0f};
    entity_t* point_light = NULL;
    entity_init_point_light(quark, "point light", light_position, color, 0.0f, &point_light);
    entity->data.pickup.point_light_id = entlist_add(quark, &quark->map.entlist, point_light);

    *out = entity;
}

void entity_pickup_set_material(quark_t* quark, entity_t* entity, material_t* material, int slot) {
	if (!entity || entity->type != ENTITY_PICKUP) return;
	
	if (slot >= MAX_MATERIALS) {
		error(quark, "material limit per entity reached (%d)", MAX_MATERIALS);
		return;
	}

    entity->data.mesh.materials[slot] = material;
}

void entity_tick_pickup(quark_t* quark, entity_t* entity, entity_pickup_t* pickup) {
    glm_quat(entity->rotation, rad(quark->time * 90.0f), 0.0f, 1.0f, 0.0f);

    if (!pickup->can_pickup && quark->time - pickup->pickup_time >= 30.0f)
        pickup->can_pickup = true;

    entity_t* point_light = quark->map.entlist.ents[pickup->point_light_id];
    point_light->data.point_light.is_visible = pickup->can_pickup;

    if (quark->player) {
        float distance = glm_vec3_distance(entity->position, quark->player->position);

        if (pickup->can_pickup && distance < 1.5f) {
            a_play(quark, &quark->audio, (sound_t*)pickup->pickup_sound,
                entity->position, random(0.85f, 1.15f));
            pickup->can_pickup = false;
            pickup->pickup_time = quark->time;

            switch (pickup->type) {
            case PICKUP_ARMOR: {
                quark->player->shield += 100.0f;
                quark->player->shield = clamp(quark->player->shield, 0.0f, 100.0f);
                break;
            }
            default: unreachable(quark); break;
            }
        }
    }

    /*float distance = glm_vec3_distance(entity->position, quark->player->position);
    if (distance < PICKUP_COLLECT_DISTANCE) {
        a_play(quark, &quark->audio, (sound_t*)pickup->pickup_sound,
            entity->position, random(0.85f, 1.15f));
        
        if (pickup->type == PICKUP_COIN) {
            quark->player->inventory.coins++;
            r_add_partfx_pickup_coin(quark, &quark->renderer, entity->position);
        }
        
        entlist_remove(quark, &quark->map.entlist, entity);

    } else if (distance < PICKUP_SUCK_IN_DISTANCE) {
        vec3 direction;
        glm_vec3_sub(quark->player->position, entity->position, direction);
        glm_normalize(direction);

        vec3 velocity;
        glm_vec3_copy(direction, velocity);
        glm_vec3_scale(velocity, PICKUP_SUCK_IN_SPEED * quark->dt, velocity);

        glm_vec3_add(entity->velocity, velocity, entity->velocity);

    } else {
        glm_vec3_zero(entity->velocity);
    }

    vec3 move;
    glm_vec3_copy(entity->velocity, move);
    glm_vec3_scale(move, quark->dt, move);
    glm_vec3_add(entity->position, move, entity->position);*/
}
