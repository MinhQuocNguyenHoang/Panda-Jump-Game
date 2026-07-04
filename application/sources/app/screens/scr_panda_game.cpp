#include "scr_panda_game.h"

uint8_t pj_game_state = GAME_OFF;

static uint32_t tick_count = 0;

static void view_scr_panda_game();
static bool check_collision(int16_t px, int16_t py, int16_t bx, int16_t by);
void ranking(uint32_t score);

static void drawSparkleStar(int16_t x, int16_t y)
{
  view_render.drawPixel(x, y, WHITE);
  view_render.drawPixel(x - 1, y, WHITE);
  view_render.drawPixel(x + 1, y, WHITE);
  view_render.drawPixel(x, y - 1, WHITE);
  view_render.drawPixel(x, y + 1, WHITE);
}

static void drawVectorTrophy(int16_t x, int16_t y)
{
  // Bowl
  view_render.drawTriangle(x + 2, y, x + 18, y, x + 10, y + 12, WHITE);
  view_render.fillTriangle(x + 4, y + 1, x + 16, y + 1, x + 10, y + 10, WHITE);

  // Handles
  view_render.drawCircle(x + 2, y + 4, 3, WHITE);
  view_render.drawCircle(x + 18, y + 4, 3, WHITE);

  // Stem
  view_render.drawFastVLine(x + 10, y + 10, 6, WHITE);
  view_render.drawFastVLine(x + 9, y + 10, 6, WHITE);
  view_render.drawFastVLine(x + 11, y + 10, 6, WHITE);

  // Base
  view_render.fillRect(x + 5, y + 16, 11, 4, WHITE);
}

static void printCenteredNumber(int16_t center_x, int16_t y, uint32_t number)
{
  int16_t offset = (number >= 100) ? 9 : (number >= 10) ? 6
                                                        : 3;
  view_render.setCursor(center_x - offset, y);
  view_render.print(number);
}

static void check_game_time_limit()
{
  if (tick_count % 5 == 0)
  {
    panda.survival_time_ticks++;

    // Check victory time limit
    uint32_t target_ticks = 120; // Default 60s
    if (game_settings.time_limit == 0)
    {
      target_ticks = 60; // 30s
    }
    else if (game_settings.time_limit == 2)
    {
      target_ticks = 180; // 90s
    }

    if (panda.survival_time_ticks >= target_ticks)
    {
      ranking(panda.score);
      pj_game_state = GAME_WIN;
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_MERRY_CHRISTMAS);
      }
    }
  }
}

static void check_bug_collisions(uint8_t px, uint8_t py)
{
  for (int i = 0; i < MAX_BUGS; i++)
  {
    if (bugs[i].active && bugs[i].explosion_ticks == 0 && check_collision(px, py, bugs[i].x, bugs[i].y))
    {
      if (py < bugs[i].y) // Panda is physically higher: stomp the bug!
      {
        bugs[i].explosion_ticks = 4; // Start explosion
        if (game_settings.sound_en)
        {
          BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        uint8_t pts = 10;
        if (game_settings.difficulty == 1)
        {
          pts = 20;
        }
        else if (game_settings.difficulty == 2)
        {
          pts = 30;
        }
        panda.score += pts;
      }
      else // Collision from below or level: Game Over!
      {
        ranking(panda.score);
        pj_game_state = GAME_OVER;
        if (game_settings.sound_en)
        {
          BUZZER_PlaySound(BUZZER_SOUND_BANG);
        }
        break;
      }
    }
  }
}

static void check_arrow_collisions(uint8_t px, uint8_t py)
{
  for (int i = 0; i < MAX_ARROWS; i++)
  {
    if (arrows[i].active && check_collision(px, py, arrows[i].x, arrows[i].y))
    {
      pj_game_state = GAME_OVER;
      ranking(panda.score);
      if (game_settings.sound_en)
      {
        BUZZER_PlaySound(BUZZER_SOUND_BANG);
      }
      break;
    }
  }
}

view_dynamic_t dyn_view_panda_game = {{
                                          .item_type = ITEM_TYPE_DYNAMIC,
                                      },
                                      view_scr_panda_game};

view_screen_t scr_panda_game = {
    &dyn_view_panda_game,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

static const uint8_t BAMBOO_X[] = {16, 48, 80};

void pj_game_frame_display()
{
  // 1. Draw 3 bamboos
  for (int i = 0; i < 3; i++)
  {
    view_render.drawBitmap(BAMBOO_X[i], 0, bamboo, 8, 64, WHITE);
  }

  // Shifted divider line to 102 (was 81)
  view_render.drawFastVLine(102, 0, 64, WHITE);

  // Shifted score box to 104 (was 84) with width 22 (was 30)
  view_render.drawRoundRect(104, 2, 22, 27, 3, WHITE);

  // Draw a mini star icon shifted by +16px: center is 114 (was 98)
  view_render.drawPixel(114, 5, WHITE);
  view_render.drawLine(112, 6, 116, 6, WHITE);
  view_render.drawLine(111, 7, 117, 7, WHITE);
  view_render.drawLine(113, 8, 115, 8, WHITE);
  view_render.drawPixel(112, 9, WHITE);
  view_render.drawPixel(116, 9, WHITE);

  // Print score (centered in the box, center X = 115)
  view_render.setTextSize(1);
  view_render.setTextColor(WHITE);
  printCenteredNumber(115, 16, panda.score);

  // --- TIME BOX (BOTTOM RIGHT) ---
  // Draw rounded box for survival time shifted by +16px: 104 (was 84)
  view_render.drawRoundRect(104, 33, 22, 28, 3, WHITE);

  // Draw a mini clock icon centered at 114 (was 98)
  view_render.drawCircle(114, 40, 4, WHITE);
  view_render.drawLine(114, 40, 114, 38, WHITE); // Hour hand
  view_render.drawLine(114, 40, 116, 40, WHITE); // Minute hand

  // Calculate and print remaining survival time (counting down to 0)
  uint32_t target_ticks = 120;
  if (game_settings.time_limit == 0)
  {
    target_ticks = 60;
  }
  else if (game_settings.time_limit == 2)
  {
    target_ticks = 180;
  }
  uint32_t remaining_ticks = (panda.survival_time_ticks >= target_ticks) ? 0 : (target_ticks - panda.survival_time_ticks);
  uint32_t seconds = remaining_ticks / 2;

  // Print seconds (centered in the box, center X = 113)
  printCenteredNumber(113, 48, seconds);
  view_render.print("s");
}

void pj_panda_display()
{
  uint8_t bx = BAMBOO_X[panda.lane];
  uint8_t px = (panda.side == 0) ? (bx - 12) : (bx + 4);
  const unsigned char *bmp = (panda.side == 0) ? panda_left : panda_right;
  view_render.drawBitmap(px, panda.y, bmp, 16, 16, WHITE);
}

static bool check_collision(int16_t px, int16_t py, int16_t bx, int16_t by)
{
  return (px < bx + 8 && px + 16 > bx &&
          py < by + 8 && py + 16 > by);
}

void pj_bug_display(bug_t *bug)
{
  const unsigned char *bmp;
  uint8_t h = 8;

  if (bug->direction) // crawls down
  {
    bmp = bug->side ? bug_right_down : bug_left_down;
    h = 8;
  }
  else // crawls up
  {
    bmp = bug->side ? bug_right : bug_left;
    h = 10;
  }
  view_render.drawBitmap(bug->x, bug->y, bmp, 8, h, WHITE);
}

void pj_arrow_display(arrow_t *arrow_t)
{
  view_render.drawBitmap(arrow_t->x, arrow_t->y, arrow, 10, 5, WHITE);
}

// Hàm vẽ biểu tượng Ngôi sao tính điểm (đặc trưng của game)
static void drawScoreStar(int16_t x, int16_t y)
{
  view_render.drawPixel(x + 3, y, WHITE);
  view_render.drawLine(x + 1, y + 1, x + 5, y + 1, WHITE);
  view_render.drawLine(x, y + 2, x + 6, y + 2, WHITE);
  view_render.drawLine(x + 2, y + 3, x + 4, y + 3, WHITE);
  view_render.drawPixel(x + 1, y + 4, WHITE);
  view_render.drawPixel(x + 5, y + 4, WHITE);
}

// Hàm vẽ biểu tượng Đồng hồ bấm giờ
static void drawClockIcon(int16_t x, int16_t y)
{
  view_render.drawCircle(x + 4, y + 4, 4, WHITE);
  view_render.drawLine(x + 4, y + 4, x + 4, y + 2, WHITE); // Kim giờ
  view_render.drawLine(x + 4, y + 4, x + 6, y + 4, WHITE); // Kim phút
}

// Hàm vẽ màn hình kết thúc game dùng chung (Hỗ trợ cả Thua cuộc và Chiến thắng)
static void drawEndGameScreen(const char *title, bool is_victory)
{
  // 1. Vẽ khung viền bo góc ngoài cùng
  view_render.drawRoundRect(2, 4, 124, 56, 4, WHITE);

  // 2. Vẽ thanh tiêu đề trắng đặc
  view_render.fillRect(4, 6, 120, 11, WHITE);

  // 3. Vẽ Tiêu đề căn giữa chữ đen
  view_render.setTextSize(1);
  view_render.setTextColor(BLACK);
  int16_t title_offset = (title[0] == 'V') ? 40 : 37; // Căn giữa cho "VICTORY!" hoặc "GAME OVER"
  view_render.setCursor(title_offset, 8);
  view_render.print(title);
  view_render.setTextColor(WHITE); // Chuyển lại màu chữ trắng

  // 4. Vẽ vạch chia dọc phân tách cột thông tin
  // Màn hình win dùng vách ngăn lệch trái (x=52), màn hình thua dùng vách ngăn ở giữa (x=64)
  view_render.drawFastVLine(is_victory ? 52 : 64, 20, 22, WHITE);

  if (is_victory)
  {
    // --- Cột trái: Cúp và sao lấp lánh trang trí ---
    drawVectorTrophy(16, 23);
    drawSparkleStar(10, 28);
    drawSparkleStar(40, 28);

    // --- Cột phải: Thông số chiến thắng ---
    view_render.setCursor(58, 22);
    view_render.print("YOU WIN!");
    view_render.setCursor(58, 33);
    view_render.print("Score: ");
    view_render.print(panda.score);
  }
  else
  {
    // --- Cột trái: Điểm số ---
    uint8_t score_x_icon = (panda.score < 10) ? 24 : (panda.score < 100) ? 21
                                                                         : 18;
    uint8_t score_x_text = score_x_icon + 11;
    drawScoreStar(score_x_icon, 24);
    view_render.setCursor(score_x_text, 23);
    view_render.print(panda.score);

    // --- Cột phải: Thời gian sinh tồn thực tế ---
    uint32_t seconds = panda.survival_time_ticks / 2;
    uint8_t time_x_icon = (seconds < 10) ? 83 : (seconds < 100) ? 80
                                                                : 77;
    uint8_t time_x_text = time_x_icon + 12;
    drawClockIcon(time_x_icon, 23);
    view_render.setCursor(time_x_text, 23);
    view_render.print(seconds);
    view_render.print("s");
  }

  // 5. Đường kẻ ngang bên dưới và Hướng dẫn phím bấm
  view_render.drawFastHLine(2, 45, 124, WHITE);
  view_render.setCursor(7, 49);
  view_render.print(is_victory ? "UP:Play  MODE:Exit" : "UP:Retry  MODE:Exit");
}

static void view_scr_panda_game()
{
  view_render.clear();

  if (pj_game_state == GAME_PLAY)
  {
    // Hiệu ứng lá rơi ngẫu nhiên sinh động
    view_render.drawPixel((128 - (tick_count * 2)) % 138 - 10, 15, WHITE);
    view_render.drawPixel((128 - (tick_count * 3)) % 138 - 10, 35, WHITE);
    view_render.drawPixel((128 - (tick_count * 1)) % 138 - 10, 48, WHITE);

    pj_game_frame_display();
    pj_panda_display();

    for (int i = 0; i < MAX_BUGS; i++)
    {
      if (bugs[i].active)
      {
        if (bugs[i].explosion_ticks > 0)
        {
          view_render.drawBitmap(bugs[i].x - 4, bugs[i].y - 1, boom, 16, 10, WHITE);
        }
        else
        {
          pj_bug_display(&bugs[i]);
        }
      }
    }

    for (int i = 0; i < MAX_ARROWS; i++)
    {
      if (arrows[i].active)
      {
        pj_arrow_display(&arrows[i]);
      }
    }
  }
  else if (pj_game_state == GAME_OVER)
  {
    drawEndGameScreen("GAME OVER", false);
  }
  else if (pj_game_state == GAME_WIN)
  {
    drawEndGameScreen("VICTORY!", true);
  }
}

static bool score_recorded = false;

void ranking(uint32_t score)
{
  if (score_recorded)
    return;
  score_recorded = true;

  // Check if the new score qualifies for the top 3
  if (score > top_scores[2])
  {
    // Insert the new score into the correct position
    if (score > top_scores[0])
    {
      top_scores[2] = top_scores[1];
      top_scores[1] = top_scores[0];
      top_scores[0] = score;
    }
    else if (score > top_scores[1])
    {
      top_scores[2] = top_scores[1];
      top_scores[1] = score;
    }
    else
    {
      top_scores[2] = score;
    }
  }
}

void scr_panda_game_handle(ak_msg_t *msg)
{
  switch (msg->sig)
  {
  case SCREEN_ENTRY:
  {
    APP_DBG_SIG("PANDA SCREEN_ENTRY\n");
    pj_game_state = GAME_PLAY;
    score_recorded = false;
    if (game_settings.sound_en)
    {
      BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
    }
    tick_count = 0;
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
    task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
    task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_SETUP);
    task_post_pure_msg(PJ_ARROW_GAME_ID, PJ_ARROW_GAME_SETUP);
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE, 100,
              TIMER_PERIODIC);
  }
  break;

  case AC_DISPLAY_PANDA_GAME_UPDATE:
  {
    tick_count++;

    // Perform collision check on display ticks (for higher responsiveness)
    if (pj_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_UPDATE);
      task_post_pure_msg(PJ_ARROW_GAME_ID, PJ_ARROW_GAME_UPDATE);

      // Check game time limit and increment ticks
      check_game_time_limit();

      // Check collisions (only if still in play)
      if (pj_game_state == GAME_PLAY)
      {
        uint8_t bx = BAMBOO_X[panda.lane];
        uint8_t px = (panda.side == 0) ? (bx - 12) : (bx + 4);

        check_bug_collisions(px, panda.y);

        if (pj_game_state == GAME_PLAY)
        {
          check_arrow_collisions(px, panda.y);
        }
      }
    }
  }
  break;

  case AC_DISPLAY_BUTON_UP_PRESSED:
  {
    APP_DBG_SIG("PANDA LEFT\n");
    if (pj_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_RIGHT);
    }
    else if (pj_game_state == GAME_OVER || pj_game_state == GAME_WIN)
    {
      score_recorded = false;
      pj_game_state = GAME_PLAY;
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_SETUP);
      task_post_pure_msg(PJ_BUG_GAME_ID, PJ_BUG_GAME_SETUP);
      task_post_pure_msg(PJ_ARROW_GAME_ID, PJ_ARROW_GAME_SETUP);
    }
  }
  break;

  case AC_DISPLAY_BUTON_DOWN_PRESSED:
  {
    APP_DBG_SIG("PANDA RIGHT\n");
    if (pj_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_JUMP_LEFT);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_RELEASED:
  {
    APP_DBG_SIG("PANDA UP\n");
    if (pj_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_UP);
    }
    else if (pj_game_state == GAME_OVER || pj_game_state == GAME_WIN)
    {
      timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE);
      SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
    }
  }
  break;

  case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
  {
    APP_DBG_SIG("PANDA DOWN\n");
    if (pj_game_state == GAME_PLAY)
    {
      task_post_pure_msg(PJ_PANDA_GAME_ID, PJ_PANDA_GAME_DOWN);
    }
  }
  break;

  default:
    break;
  }
}
