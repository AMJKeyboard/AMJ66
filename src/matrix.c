/*
Copyright 2014 Kai Ryu <kai1103@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void init_rows(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // disable JTAG
    MCUCR = _BV(JTD);
    MCUCR = _BV(JTD);

    // initialize row and col
    init_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
 * pin: F0  B3  B2  B1  B0  B7  D0  D1  D2  D3  D5  D6  D7  B4  B5  B6
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRF &= ~_BV(PF0);
    PORTF |= _BV(PF0);

    DDRD &= ~0b11101111;
    PORTD |= 0b11101111;
    DDRB &= ~0b11111111;
    PORTB |= 0b11111111;
}

static matrix_row_t read_cols(void)
{

    return (PINF&_BV(PF0) ? 0 : (1<<0)) |
           (PINB&_BV(PB3) ? 0 : (1<<1)) |
           (PINB&_BV(PB2) ? 0 : (1<<2)) |
           (PINB&_BV(PB1) ? 0 : (1<<3)) |
           (PINB&_BV(PB0) ? 0 : (1<<4)) |
           (PINB&_BV(PB7) ? 0 : (1<<5)) |
           (PIND&_BV(PD0) ? 0 : (1<<6)) |
           (PIND&_BV(PD1) ? 0 : (1<<7)) |
           (PIND&_BV(PD2) ? 0 : (1<<8)) |
           (PIND&_BV(PD3) ? 0 : (1<<9)) |
           (PIND&_BV(PD5) ? 0 : (1<<10)) |
           (PIND&_BV(PD6) ? 0 : (1<<11)) |
           (PIND&_BV(PD7) ? 0 : (1<<12)) |
           (PINB&_BV(PB4) ? 0 : (1<<13)) |
           (PINB&_BV(PB5) ? 0 : (1<<14)) |
           (PINB&_BV(PB6) ? 0 : (1<<15));
}

/* Row pin configuration
 * row: 0   1   2   3   4
 * pin: F1  F4  F5  F6  F7
 */
static void init_rows(void)
{
    unselect_rows();
}

static void unselect_rows(void)
{
    DDRF &= ~(_BV(PF1) | _BV(PF4) | _BV(PF5) | _BV(PF6) | _BV(PF7));
    PORTF &= ~(_BV(PF1) | _BV(PF4) | _BV(PF5) | _BV(PF6) | _BV(PF7));
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0: // row6  f...
            DDRF  |= _BV(PF7);
            PORTF &= ~_BV(PF7);
            break;
        case 1: // row1
            DDRF |= _BV(PF6);
            PORTF &= ~_BV(PF6);
            break;
        case 2: // row2
            DDRF |= _BV(PF5);
            PORTF &= ~_BV(PF5);
            break;
        case 3: // row3
            DDRF |= _BV(PF4);
            PORTF &= ~_BV(PF4);
            break;
        case 4: // row3
            DDRF |= _BV(PF1);
            PORTF &= ~_BV(PF1);
            break;
    }
}
