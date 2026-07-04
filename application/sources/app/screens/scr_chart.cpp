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

// Hàm vẽ chiếc cúp vô địch tinh tế (12x8 pixel)
// Hàm vẽ vương miện hoàng gia tinh tế (11x6 pixel)
static void drawCrown(int16_t x, int16_t y)
{
    view_render.drawLine(x, y + 5, x + 10, y + 5, WHITE);      // Đường đế nằm ngang
    view_render.drawLine(x, y + 2, x, y + 5, WHITE);           // Cạnh dọc bên trái
    view_render.drawLine(x + 10, y + 2, x + 10, y + 5, WHITE); // Cạnh dọc bên phải

    // Các đường răng cưa của vương miện
    view_render.drawLine(x, y + 2, x + 2, y + 4, WHITE);
    view_render.drawLine(x + 2, y + 4, x + 5, y + 1, WHITE);
    view_render.drawLine(x + 5, y + 1, x + 8, y + 4, WHITE);
    view_render.drawLine(x + 8, y + 4, x + 10, y + 2, WHITE);

    // Các hạt ngọc ở 3 đỉnh vương miện
    view_render.drawPixel(x, y + 1, WHITE);
    view_render.drawPixel(x + 5, y, WHITE);
    view_render.drawPixel(x + 10, y + 1, WHITE);
}

// Hàm vẽ ngôi sao lấp lánh trang trí (5 pixel hình chữ thập)
static void drawSparkleStar(int16_t x, int16_t y)
{
    view_render.drawPixel(x, y, WHITE);
    view_render.drawPixel(x - 1, y, WHITE);
    view_render.drawPixel(x + 1, y, WHITE);
    view_render.drawPixel(x, y - 1, WHITE);
    view_render.drawPixel(x, y + 1, WHITE);
}

// Hàm vẽ bục vinh quang tổng quát (hỗ trợ tự động căn giữa chữ/số điểm)
static void drawPodiumColumn(int16_t x, int16_t y, int16_t w, int16_t h, const char* rank_label, uint32_t score, bool is_first_place)
{
    int16_t center_x = x + w / 2;
    
    // Vẽ bục (Hạng 1 tô đặc trắng, các hạng khác vẽ viền rỗng)
    if (is_first_place)
    {
        view_render.fillRoundRect(x, y, w, h, 2, WHITE);
        view_render.setTextColor(BLACK); // Chữ đen trên nền trắng
    }
    else
    {
        view_render.drawRoundRect(x, y, w, h, 2, WHITE);
        view_render.setTextColor(WHITE); // Chữ trắng trên nền đen
    }
    
    // In nhãn thứ hạng (1st, 2nd, 3rd) ở trung tâm bục
    view_render.setCursor(center_x - 8, y + (h - 7) / 2);
    view_render.print(rank_label);
    
    // In số điểm căn giữa phía trên đỉnh bục
    view_render.setTextColor(WHITE);
    int16_t offset = (score >= 100) ? 9 : (score >= 10) ? 6 : 3;
    view_render.setCursor(center_x - offset, y - 10);
    view_render.print(score);
}

void view_scr_chart()
{
    view_render.clear();
    
    // 1. Vẽ khung viền đơn mảnh bo góc ngoài cùng bao trọn màn hình
    view_render.drawRoundRect(0, 0, 128, 64, 3, WHITE);
    
    // 2. Vẽ Header kết hợp Icon Game có sẵn trong project
    view_render.drawBitmap(4, 3, panda_icon, 15, 15, WHITE);
    view_render.drawBitmap(107, 2, chart_icon, 17, 16, WHITE);
    
    // In tiêu đề chữ căn giữa
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(31, 7);
    view_render.print("LEADERBOARD");
    
    // Đường kẻ ngang ngăn cách tiêu đề tinh tế ở y = 19
    view_render.drawFastHLine(3, 19, 122, WHITE);
    
    // 3. Vẽ 3 bục vinh quang sử dụng hàm dùng chung cực kỳ gọn gàng
    drawPodiumColumn(14, 48, 28, 13, "2nd", top_scores[1], false); // Hạng 2
    drawPodiumColumn(86, 52, 28, 9,  "3rd", top_scores[2], false); // Hạng 3
    drawPodiumColumn(50, 42, 28, 19, "1st", top_scores[0], true);  // Hạng 1 (đặc biệt)
    
    // 4. Vẽ các chi tiết trang trí cho Hạng 1 (Vương miện và Sao lấp lánh)
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