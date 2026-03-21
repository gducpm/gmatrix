#include "def.h"

void process_gmchar(const int r, const int c) {
	if (frame % col_update[c] != 0) return;

	if (buffer[r][c].is_head) {
		if (r < rows - 1) {
			unsigned char charval;
			if (binary_charset) charval = (unsigned char) (rand() % 2) + 48;
			else charval = (unsigned char) (rand() % 94) + 33;
			gmchar next = (gmchar) {
				charval,
				true,
				buffer[r][c].expiry_in,
				true
			};
			buffer[r+1][c] = next;
		}
		buffer[r][c].expiry_in--;
		buffer[r][c].is_head = false;
	} else if (buffer[r][c].is_active) {
		if (--buffer[r][c].expiry_in < 1) {
			buffer[r][c].val = ' ';
			buffer[r][c].is_active = false;
		}
	}
}

void process_column(const int c) {
	if (frame % col_update[c] != 0) return;

	if (col_cooldown[c] > 0) {
		col_cooldown[c]--;
		return;
	}

	if (!buffer[0][c].is_active) {
		unsigned char charval;
		if (binary_charset) charval = (unsigned char) (rand() % 2) + 48;
		else charval = (unsigned char) (rand() % 94) + 33;
		gmchar head = (gmchar) {
			charval,
			true,
			(rand() % 20) + 10,
			true
		};
		buffer[0][c] = head;
		col_cooldown[c] = head.expiry_in + (rand() % 25) + 10;
	}
}

