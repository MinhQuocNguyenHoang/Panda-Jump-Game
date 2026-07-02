#include "scr_menu_game.h"

/* Variable and Struct Declaration - Menu game */
/*****************************************************************************/
#define STEP_MENU_CHOSSE (22)
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
static char menu_items_name[NUMBER_MENU_ITEMS][20] = {
    "   Panda Game   ", // item 1
    "   Setting      ", // item 2
    "   Charts       ", // item 3
    "   Exit         ", // item 4
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

// Menu items axis Y for drawing the 3 visible frames
static uint8_t menu_items_icon_axis_y[3] = {
    2,  // icon frame 1
    24, // icon frame 2
    46  // icon frame 3
};

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

// Scroll bar
typedef struct
{
  uint8_t axis_x = 126;
  uint8_t axis_y = 0;
  uint8_t size_W = 3;
  uint8_t size_h = SCREEN_MENU_H / NUMBER_MENU_ITEMS;
} scr_menu_scroll_bar_t;

// Frames
typedef struct
{
  uint8_t axis_x = 0;
  uint8_t axis_y = 0;
  uint8_t size_w = 123;
  uint8_t size_h = 20;
  uint8_t size_r = 3;
} scr_menu_frames_t;

static screen_t screen_menu;
static scr_menu_t menu_chosse;
static scr_menu_scroll_bar_t scroll_bar;
static scr_menu_frames_t frame_white;
static scr_menu_frames_t frame[3];

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
#define PJ_GAME_MENU_ICON_AXIS_X (7)
#define PJ_GAME_MENU_TEXT_AXIS_X (20)

  // Scroll bar
  view_render.fillRect(scroll_bar.axis_x - 1, scroll_bar.axis_y,
                       scroll_bar.size_W, scroll_bar.size_h, WHITE);
  view_render.drawBitmap(scroll_bar.axis_x, 0, dot_icon, 1, SCREEN_MENU_H,
                         WHITE);

  // Frame White
  view_render.fillRoundRect(frame_white.axis_x, frame_white.axis_y,
                            frame_white.size_w, frame_white.size_h,
                            frame_white.size_r, WHITE);

  for (uint8_t i = 0; i < 3; i++)
  {
    // Frames
    view_render.drawRoundRect(frame[i].axis_x, frame[i].axis_y, frame[i].size_w,
                              frame[i].size_h, frame[i].size_r, WHITE);
    // Icon
    view_render.drawBitmap(PJ_GAME_MENU_ICON_AXIS_X, menu_items_icon_axis_y[i],
                           menu_items_icon[screen_menu.screen + i],
                           menu_items_icon_size_w[screen_menu.screen + i],
                           menu_items_icon_size_h[screen_menu.screen + i],
                           menu_items_icon_color[screen_menu.screen + i]);
  }

  // Text Menu
  view_render.setTextSize(1);
  for (uint8_t i = 0; i < 3; i++)
  {
    view_render.setTextColor(menu_items_icon_color[screen_menu.screen + i]);
    view_render.setCursor(PJ_GAME_MENU_TEXT_AXIS_X,
                          menu_items_icon_axis_y[i] + 5);
    view_render.print(menu_items_name[screen_menu.screen + i]);
  }
}

/*****************************************************************************/
/* Handle - Menu game */
/*****************************************************************************/
static void update_menu_screen_chosse()
{
  // Frames location
  frame_white.axis_y = frame[screen_menu.location - screen_menu.screen].axis_y;
  frame[0].axis_y = 0;
  frame[1].axis_y = 22;
  frame[2].axis_y = 44;

  // update color menu
  menu_chosse._id = 1 << screen_menu.location;
  MENU_ITEMS_ICON_COLOR();

  // update scroll bar
  scroll_bar.axis_y =
      (SCREEN_MENU_H * screen_menu.location / NUMBER_MENU_ITEMS);
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
  {                                       // Charts
    BUZZER_PlaySound(BUZZER_SOUND_3BEEP); // placeholder sound
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

    if (frame_white.axis_y == frame[0].axis_y)
    {
      if (screen_menu.screen > 0)
      {
        screen_menu.screen--;
      }
    }
    else if (frame_white.axis_y == frame[1].axis_y)
    {
      frame_white.axis_y = frame[0].axis_y;
    }
    else if (frame_white.axis_y == frame[2].axis_y)
    {
      frame_white.axis_y = frame[1].axis_y;
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

    if (frame_white.axis_y == frame[0].axis_y)
    {
      frame_white.axis_y = frame[1].axis_y;
    }
    else if (frame_white.axis_y == frame[1].axis_y)
    {
      frame_white.axis_y = frame[2].axis_y;
    }
    else if (frame_white.axis_y == frame[2].axis_y)
    {
      if (screen_menu.screen < NUMBER_MENU_ITEMS - 3)
      {
        screen_menu.screen++;
      }
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
