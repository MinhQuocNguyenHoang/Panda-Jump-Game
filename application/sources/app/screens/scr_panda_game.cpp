#include "scr_panda_game.h"

static void view_scr_panda_game();

view_dynamic_t dyn_view_panda_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_panda_game
};

view_screen_t scr_panda_game = {
	&dyn_view_panda_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

static void view_scr_panda_game() {
	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(10, 20);
	view_render.print("Panda Game Start!");
	
	view_render.setCursor(10, 40);
	view_render.print("Press MODE to exit");
}

void scr_panda_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("PANDA SCREEN_ENTRY\n");
		BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
	} break;

	case AC_DISPLAY_BUTON_MODE_PRESSED: { // note spelling: BUTON
		APP_DBG_SIG("PANDA EXIT\n");
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	} break;

	default:
		break;
	}
}
