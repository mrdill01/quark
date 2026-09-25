#include "player.h"
#include "quark.h"

void bot_init(quark_t* quark, player_t* bot) {
    bot->bot.last_speech = quark->time;
    bot->bot.last_speech_word = quark->time;
}

static void move_to_target(quark_t* quark, player_t* bot, vec3 target, float stop_distance) {
    if (!target)
        return;
    
    float distance = glm_vec3_distance(bot->position, target);
    if (distance < stop_distance)
        return;

    glm_vec3_sub(target, bot->position, bot->target_dir);
    glm_vec3_normalize(bot->target_dir);
}

void bot_tick(quark_t* quark, player_t* bot) {
    glm_vec3_zero(bot->target_dir);
    bot->buttons = 0;

    if ((int)random(0, 150) == 0)
        bot->buttons |= PLAYER_BUTTON_JUMP;

    move_to_target(quark, bot, quark->player->position, 4.0f);
}
