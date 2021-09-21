#include <stdlib.h>
#include "randomName.h"

const uint8_t randomCharStart[] = {'A', 'a', '0'};
const int randomCharRange[] = { 26, 26, 10 };

void setRandomName(uint8_t *begin, uint8_t *end) {
	while (begin != end) {
		int type = rand() % 3;
		*begin = randomCharStart[type] + rand() % randomCharRange[type];
		begin += 1;
	}
}