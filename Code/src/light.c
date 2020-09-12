#include "light.h"



/**********	функция плавного увеличения яркости свечения	*************/
		void light_rising_trigger (void) {		
				for (uint8_t i = 75; i > 50; i--) {			
					TIM6 -> ARR = i;
					TIM6 -> EGR |= TIM_EGR_UG;															// обновление регистров (update event)
					
					delay_ms(100);
				}
	}
