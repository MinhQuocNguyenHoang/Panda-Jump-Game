#include "scr_menu_game.h"
#include <string.h>

/* Variable and Struct Declaration - Menu game */
/*****************************************************************************/
#define NUMBER_MENU_ITEMS (4)
#define SCREEN_MENU_H (64)

#define MENU_ITEMS_ICON_COLOR()                              \
  do                                                         \
  {                                                          \
    menu_items_icon_color[0] = !menu_chosse.items.is_item_1; \
    menu_items_icon_color[1] = !menu_chosse.items.is_item_2; \
    menu_items_icon_color[2] = !menu_chosse.items.is_item_3; \
    menu_items_icon_color[3] = !menu_chosse.items.is_item_4; \
  } while (0);

struct menu_items
{
  unsigned int is_item_1 : 1;
  unsigned int is_item_2 : 1;
  unsigned int is_item_3 : 1;
  unsigned int is_item_4 : 1;
};

// Menu items name
static const char *menu_items_name[NUMBER_MENU_ITEMS] = {
    "PANDA GAME", // item 1
    "SETTING",    // item 2
    "CHARTS",     // item 3
    "EXIT",       // item 4
};

// Menu items icon
static const uint8_t *menu_items_icon[NUMBER_MENU_ITEMS] = {
    panda_icon,   // item 1
    setting_icon, // item 2
    chart_icon,   // item 3
    exit_icon,    // item 4
};

// Menu items size W and H
static uint8_t menu_items_icon_size_w[NUMBER_MENU_ITEMS] = {15, 16, 17, 16};
static uint8_t menu_items_icon_size_h[NUMBER_MENU_ITEMS] = {15, 16, 16, 16};

// Menu items color
static uint8_t menu_items_icon_color[NUMBER_MENU_ITEMS];

typedef struct
{
  int screen;
  int location;
} screen_t;

union scr_menu_t
{
  uint32_t _id = 1;
  menu_items items;
};

static screen_t screen_menu;
static scr_menu_t menu_chosse;

static const uint8_t MENU_BOX_X[] = {7, 37, 67, 97};

/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_menu_game();

view_dynamic_t dyn_view_menu = {{
                                    .item_type = ITEM_TYPE_DYNAMIC,
                                },
                                view_scr_menu_game};

view_screen_t scr_menu_game = {
    &dyn_view_menu,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

static void view_scr_menu_game()
{
  view_render.clear();

  // Draw header Title "PANDA JUMP" in the center at Y=2
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  view_render.setCursor(34, 2);
  view_render.print("PANDA JUMP");
  
  // Draw divider line below header
  view_render.drawFastHLine(0, 12, 128, WHITE);

  // Draw the 4 cards horizontally
  for (uint8_t i = 0; i < NUMBER_MENU_ITEMS; i++)
  {
    uint8_t bx = MENU_BOX_X[i];
    
    if (i == screen_menu.location)
    {
      // Highlighted item: solid white background
      view_render.fillRoundRect(bx, 16, 24, 24, 3, WHITE);
    }
    else
    {
      // Unselected item: outline only
      view_render.drawRoundRect(bx, 16, 24, 24, 3, WHITE);
    }

    // Draw the icon centered inside the 24x24 box
    uint8_t iw = menu_items_icon_size_w[i];
    uint8_t ih = menu_items_icon_size_h[i];
    uint8_t ix = bx + 12 - (iw / 2);
    uint8_t iy = 16 + 12 - (ih / 2);

    view_render.drawBitmap(ix, iy, menu_items_icon[i], iw, ih, menu_items_icon_color[i]);
  }

  // Draw selected menu item name centered at Y=44
  view_render.setTextColor(WHITE);
  const char* name = menu_items_name[screen_menu.location];
  uint8_t name_len = strlen(name);
  uint8_t name_x = 64 - (name_len * 6) / 2;
  view_render.setCursor(name_x, 44);
  view_render.print(name);

  // Draw bottom help text centered at Y=54
  view_render.setCursor(4, 54);
  view_render.print("UP/DN:Move  MODE:Ok");
}

/*****************************************************************************/
/* Handle - Menu game */
/*****************************************************************************/
static void update_menu_screen_chosse()
{
  // update color menu
  menu_chosse._id = 1 << screen_menu.location;
  MENU_ITEMS_ICON_COLOR();
}

static void screen_tran_menu()
{
  switch (screen_menu.location)
  {
  case 0:
  { // Play Panda Game
    SCREEN_TRAN(scr_panda_game_handle, &scr_panda_game);
  }
  break;

  case 1:
  { // Setting
    SCREEN_TRAN(scr_setting_handle, &scr_setting);
  }
  break;

  case 2:
  { // Charts
    SCREEN_TRAN(scr_chart_handle, &scr_chart);
  }
  break;

  case 3:
  { // Exit to welcome/idle
    SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
  }
  break;

  default:
    break;
  }
}

void scr_menu_game_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("MENU SCREEN_ENTRY\n");
    screen_menu.screen = 0;
    screen_menu.location = 0;
    update_menu_screen_chosse();
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
  }
  break;

  case AC_DISPLAY_SHOW_IDLE:
  {
    APP_DBG_SIG("MENU AC_DISPLAY_SHOW_IDLE\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    SCREEN_TRAN(scr_idle_handle, &scr_idle);
  }
  break;

  case AC_DISPLAY_BUTON_MODE_PRESSED:
  { // note spelling: BUTON
    APP_DBG_SIG("MENU AC_DISPLAY_BUTON_MODE_PRESSED\n");
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    screen_tran_menu();
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  { // note spelling: BUTON
    APP_DBG_SIG("MENU AC_DISPLAY_BUTON_UP_PRESSED\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (screen_menu.location > 0)
    {
      screen_menu.location--;
    }

    update_menu_screen_chosse();
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  { // note spelling: BUTON
    APP_DBG_SIG("MENU AC_DISPLAY_BUTON_DOWN_PRESSED\n");
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
              AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);

    if (screen_menu.location < NUMBER_MENU_ITEMS - 1)
    {
      screen_menu.location++;
    }

    update_menu_screen_chosse();
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
  }
  break;

  default:
    break;
  }
}
