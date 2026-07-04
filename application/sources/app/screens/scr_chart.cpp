#include "scr_chart.h"

uint32_t top_scores[3] = {0};

static void view_scr_chart();
view_dynamic_t dyn_view_chart = {{
                                     .item_type = ITEM_TYPE_DYNAMIC,
                                 },
                                 view_scr_chart};

view_screen_t scr_chart = {
    &dyn_view_chart,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

// Function to draw a delicate royal crown (11x6 pixels)
static void drawCrown(int16_t x, int16_t y)
{
    view_render.drawLine(x, y + 5, x + 10, y + 5, WHITE);      // Horizontal base line
    view_render.drawLine(x, y + 2, x, y + 5, WHITE);           // Left vertical edge
    view_render.drawLine(x + 10, y + 2, x + 10, y + 5, WHITE); // Right vertical edge

    // Crown serrated/zig-zag lines
    view_render.drawLine(x, y + 2, x + 2, y + 4, WHITE);
    view_render.drawLine(x + 2, y + 4, x + 5, y + 1, WHITE);
    view_render.drawLine(x + 5, y + 1, x + 8, y + 4, WHITE);
    view_render.drawLine(x + 8, y + 4, x + 10, y + 2, WHITE);

    // Jewels at the 3 peaks of the crown
    view_render.drawPixel(x, y + 1, WHITE);
    view_render.drawPixel(x + 5, y, WHITE);
    view_render.drawPixel(x + 10, y + 1, WHITE);
}

// Function to draw a decorative sparkling star (5-pixel cross)
static void drawSparkleStar(int16_t x, int16_t y)
{
    view_render.drawPixel(x, y, WHITE);
    view_render.drawPixel(x - 1, y, WHITE);
    view_render.drawPixel(x + 1, y, WHITE);
    view_render.drawPixel(x, y - 1, WHITE);
    view_render.drawPixel(x, y + 1, WHITE);
}

// General function to draw a podium column (supports auto-centering text and score)
static void drawPodiumColumn(int16_t x, int16_t y, int16_t w, int16_t h, const char* rank_label, uint32_t score, bool is_first_place)
{
    int16_t center_x = x + w / 2;
    
    // Draw podium (1st place is solid white, other ranks are outline only)
    if (is_first_place)
    {
        view_render.fillRoundRect(x, y, w, h, 2, WHITE);
        view_render.setTextColor(BLACK); // Black text on white background
    }
    else
    {
        view_render.drawRoundRect(x, y, w, h, 2, WHITE);
        view_render.setTextColor(WHITE); // White text on black background
    }
    
    // Print rank label (1st, 2nd, 3rd) in the center of the column
    view_render.setCursor(center_x - 8, y + (h - 7) / 2);
    view_render.print(rank_label);
    
    // Print centered score above the top of the column
    view_render.setTextColor(WHITE);
    int16_t offset = (score >= 100) ? 9 : (score >= 10) ? 6 : 3;
    view_render.setCursor(center_x - offset, y - 10);
    view_render.print(score);
}

void view_scr_chart()
{
    view_render.clear();
    
    // 1. Draw outer single-line rounded border enclosing the screen
    view_render.drawRoundRect(0, 0, 128, 64, 3, WHITE);
    
    // 2. Draw Header combined with the existing game icons
    view_render.drawBitmap(4, 3, panda_icon, 15, 15, WHITE);
    view_render.drawBitmap(107, 2, chart_icon, 17, 16, WHITE);
    
    // Print centered title text
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(31, 7);
    view_render.print("LEADERBOARD");
    
    // Horizontal divider line below the header at y = 19
    view_render.drawFastHLine(3, 19, 122, WHITE);
    
    // 3. Draw the 3 podium columns using the generic helper function
    drawPodiumColumn(14, 48, 28, 13, "2nd", top_scores[1], false); // 2nd Place
    drawPodiumColumn(86, 52, 28, 9,  "3rd", top_scores[2], false); // 3rd Place
    drawPodiumColumn(50, 42, 28, 19, "1st", top_scores[0], true);  // 1st Place (Special)
    
    // 4. Draw decorative details for 1st Place (Crown and sparkling stars)
    drawCrown(59, 23);
    drawSparkleStar(42, 25);
    drawSparkleStar(85, 25);
}

void scr_chart_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("CHART SCREEN_ENTRY\n");
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE,
                  AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
    }
    break;

    case AC_DISPLAY_SHOW_IDLE:
    {
        APP_DBG_SIG("CHART AC_DISPLAY_SHOW_IDLE\n");
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        SCREEN_TRAN(scr_idle_handle, &scr_idle);
    }
    break;

    case AC_DISPLAY_BUTON_UP_PRESSED:
    case AC_DISPLAY_BUTON_DOWN_PRESSED:
    case AC_DISPLAY_BUTON_MODE_PRESSED:
    {
        APP_DBG_SIG("CHART BUTTON_PRESSED -> Exit to menu\n");
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
    }
    break;

    default:
        break;
    }
}