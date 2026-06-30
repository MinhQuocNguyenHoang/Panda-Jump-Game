#include "button.h"

uint8_t button_init(button_t* button, uint32_t u, uint8_t id, pf_button_ctrl init, pf_button_read read, pf_button_callback callback) {
	button->enable		=	BUTTON_DISABLE;
	button->id			=	id;
	button->counter		=	0;
	button->unit		=	u;
	button->state		=	BUTTON_SW_STATE_RELEASED;
	button->last_hw_state =	BUTTON_HW_STATE_RELEASED;
	button->debounce_cnt =	0;

	button->init		=	init;
	button->read		=	read;
	button->callback	=	callback;

	if (button->init) {
		button->init();
	}
	else {
		return BUTTON_DRIVER_NG;
	}

	if (!button->read) {
		return BUTTON_DRIVER_NG;
	}

	if (!button->callback) {
		return BUTTON_DRIVER_NG;
	}

	return BUTTON_DRIVER_OK;
}

void button_enable(button_t* button) {
	button->enable = BUTTON_ENABLE;
}

void button_disable(button_t* button) {
	button->enable = BUTTON_DISABLE;
}

void button_timer_polling(button_t* button) {
	if (button->enable == BUTTON_ENABLE) {
		uint8_t raw_state = button->read();

		/* Debounce filter */
		if (raw_state != button->last_hw_state) {
			button->debounce_cnt++;
			if (button->debounce_cnt >= 3) { /* 3 consecutive 10ms ticks to confirm state change */
				button->last_hw_state = raw_state;
				button->debounce_cnt = 0;
			}
		} else {
			button->debounce_cnt = 0;
		}

		uint8_t hw_button_state = button->last_hw_state;

		/* hard button pressed */
		if (hw_button_state == BUTTON_HW_STATE_PRESSED) {

			if (button->counter_enable == BUTTON_ENABLE) {
				/* increase button counter */
				button->counter += button->unit;

				/* check long press */
				if (button->counter >= BUTTON_LONG_PRESS_TIME) {
					if (button->state != BUTTON_SW_STATE_LONG_PRESSED) {
						button->enable			= BUTTON_DISABLE;
						button->state			= BUTTON_SW_STATE_LONG_PRESSED;
						button->callback(button);
						button->enable			= BUTTON_ENABLE;
					}
				}
				/* check short press */
				else if (button->counter >= BUTTON_SHORT_PRESS_MIN_TIME) {
					if (button->state != BUTTON_SW_STATE_PRESSED) {
						button->enable			= BUTTON_DISABLE;
						button->state			= BUTTON_SW_STATE_PRESSED;
						button->callback(button);
						button->enable			= BUTTON_ENABLE;
					}
				}
			}
		}
		/* hard button released */
		else {
			/* check released */
			if (button->counter >= BUTTON_SHORT_PRESS_MIN_TIME) {
				button->enable			= BUTTON_DISABLE;
				
				if (button->state == BUTTON_SW_STATE_LONG_PRESSED) {
					button->state = BUTTON_SW_STATE_LONG_RELEASED;
					button->callback(button);
				} else {
					button->state = BUTTON_SW_STATE_RELEASED;
					button->callback(button);
				}
			}

			/* reset button */
			button->state			= BUTTON_SW_STATE_RELEASED;
			button->counter			= 0;
			button->counter_enable	= BUTTON_ENABLE;
			button->enable			= BUTTON_ENABLE;
		}
	}
}
