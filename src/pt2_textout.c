#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "pt2_header.h"
#include "pt2_helpers.h"
#include "pt2_tables.h"
#include "pt2_palette.h"
#include "pt2_visuals.h"

static const char hexTable[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void charOut(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, char ch, uint32_t color)
{
	const uint8_t *srcPtr;
	uint32_t *dstPtr;

	if (ch == '\0' && (ch <= ' ' || ch > '~'))
		return;

	srcPtr = &fontBMP[(uint8_t)ch * (FONT_CHAR_W * FONT_CHAR_H)];
	dstPtr = &frameBuffer[(yPos * SCREEN_W) + xPos];

	for (uint32_t y = 0; y < FONT_CHAR_H; y++)
	{
		for (uint32_t x = 0; x < FONT_CHAR_W; x++)
		{
			if (srcPtr[x])
				dstPtr[x] = color;
		}

		srcPtr += FONT_CHAR_W;
		dstPtr += SCREEN_W;
	}
}

void charOutBg(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, char ch, uint32_t fgColor, uint32_t bgColor)
{
	const uint8_t *srcPtr;
	uint32_t *dstPtr;

	if (ch == '\0')
		return;

	if (ch < ' ' || ch > '~')
		ch = ' ';

	srcPtr = &fontBMP[(uint8_t)ch * (FONT_CHAR_W * FONT_CHAR_H)];
	dstPtr = &frameBuffer[(yPos * SCREEN_W) + xPos];

	for (uint32_t y = 0; y < FONT_CHAR_H; y++)
	{
		for (uint32_t x = 0; x < FONT_CHAR_W; x++)
			dstPtr[x] = srcPtr[x] ? fgColor : bgColor;

		srcPtr += FONT_CHAR_W;
		dstPtr += SCREEN_W;
	}
}

void charOutBig(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, char ch, uint32_t color)
{
	const uint8_t *srcPtr;
	uint32_t *dstPtr;

	if (ch != '\0' && (ch <= ' ' || ch > '~'))
		return;

	srcPtr = &fontBMP[(uint8_t)ch * (FONT_CHAR_W * FONT_CHAR_H)];
	dstPtr = &frameBuffer[(yPos * SCREEN_W) + xPos];

	for (uint32_t y = 0; y < FONT_CHAR_H; y++)
	{
		for (uint32_t x = 0; x < FONT_CHAR_W; x++)
		{
			if (srcPtr[x])
			{
				dstPtr[x] = color;
				dstPtr[x+SCREEN_W] = color;
			}
		}

		srcPtr += FONT_CHAR_W;
		dstPtr += SCREEN_W * 2;
	}
}

void charOutBigBg(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, char ch, uint32_t fgColor, uint32_t bgColor)
{
	const uint8_t *srcPtr;
	uint32_t *dstPtr;

	if (ch == '\0')
		return;

	if (ch < ' ' || ch > '~')
		ch = ' ';

	srcPtr = &fontBMP[(uint8_t)ch * (FONT_CHAR_W * FONT_CHAR_H)];
	dstPtr = &frameBuffer[(yPos * SCREEN_W) + xPos];

	for (uint32_t y = 0; y < FONT_CHAR_H; y++)
	{
		for (uint32_t x = 0; x < FONT_CHAR_W; x++)
		{
			if (srcPtr[x])
			{
				dstPtr[x] = fgColor;
				dstPtr[x+SCREEN_W] = fgColor;
			}
			else
			{
				dstPtr[x] = bgColor;
				dstPtr[x+SCREEN_W] = bgColor;
			}
		}

		srcPtr += FONT_CHAR_W;
		dstPtr += SCREEN_W * 2;
	}
}

void textOut(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, const char *text, uint32_t color)
{
	uint32_t x = xPos;
	while (*text != '\0')
	{
		charOut(frameBuffer, x, yPos, *text++, color);
		x += FONT_CHAR_W;
	}
}

void textOutTight(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, const char *text, uint32_t color)
{
	uint32_t x = xPos;
	while (*text != '\0')
	{
		charOut(frameBuffer, x, yPos, *text++, color);
		x += (FONT_CHAR_W - 1);
	}
}

void textOutBg(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, const char *text, uint32_t fgColor, uint32_t bgColor)
{
	uint32_t x = xPos;
	while (*text != '\0')
	{
		charOutBg(frameBuffer, x, yPos, *text++, fgColor, bgColor);
		x += FONT_CHAR_W;
	}
}

void textOutBig(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, const char *text, uint32_t color)
{
	uint32_t x = xPos;
	while (*text != '\0')
	{
		charOutBig(frameBuffer, x, yPos, *text++, color);
		x += FONT_CHAR_W;
	}
}

void textOutBigBg(uint32_t *frameBuffer, uint32_t xPos, uint32_t yPos, const char *text, uint32_t fgColor, uint32_t bgColor)
{
	uint32_t x = xPos;
	while (*text != '\0')
	{
		charOutBigBg(frameBuffer, x, yPos, *text++, fgColor, bgColor);
		x += FONT_CHAR_W;
	}
}

void printTwoDecimals(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "00", fontColor);
	}
	else
	{
		if (value > 99)
			value = 99;

		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor);
	}
}

void printTwoDecimalsBig(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOutBig(frameBuffer, x, y, "00", fontColor);
	}
	else
	{
		if (value > 99)
			value = 99;

		charOutBig(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor); value /= 10;
		charOutBig(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor);
	}
}

void printThreeDecimals(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "000", fontColor);
	}
	else
	{
		if (value > 999)
			value = 999;

		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor);
	}
}

void printFourDecimals(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "0000", fontColor);
	}
	else
	{
		if (value > 9999)
			value = 9999;

		charOut(frameBuffer, x + (FONT_CHAR_W * 3), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor);
	}
}

void printFiveDecimals(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "00000", fontColor);
	}
	else
	{
		if (value > 99999)
			value = 99999;

		charOut(frameBuffer, x + (FONT_CHAR_W * 4), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 3), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor); value /= 10;
		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor);
	}
}

// this one is used for module size and sampler screen display length (zeroes are padded with space)
void printSixDecimals(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	char numberText[7];
	uint8_t i;

	if (value == 0)
	{
		textOut(frameBuffer, x, y, "     0", fontColor);
	}
	else
	{
		if (value > 999999)
			value = 999999;

		numberText[6] = 0;
		numberText[5] = '0' + (value % 10); value /= 10;
		numberText[4] = '0' + (value % 10); value /= 10;
		numberText[3] = '0' + (value % 10); value /= 10;
		numberText[2] = '0' + (value % 10); value /= 10;
		numberText[1] = '0' + (value % 10); value /= 10;
		numberText[0] = '0' + (value % 10);

		i = 0;
		while (numberText[i] == '0')
			numberText[i++] = ' ';

		textOut(frameBuffer, x, y, numberText, fontColor);
	}
}

void printOneHex(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	charOut(frameBuffer, x, y, hexTable[value & 15], fontColor);
}

void printOneHexBig(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	charOutBig(frameBuffer, x, y, hexTable[value & 15], fontColor);
}

void printTwoHex(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "00", fontColor);
	}
	else
	{
		value &= 0xFF;

		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 4], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[value & 15], fontColor);
	}
}

void printTwoHexBig(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOutBig(frameBuffer, x, y, "00", fontColor);
	}
	else
	{
		value &= 0xFF;

		charOutBig(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 4], fontColor);
		charOutBig(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[value & 15], fontColor);
	}
}

void printThreeHex(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "000", fontColor);
	}
	else
	{
		value &= 0xFFF;

		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 8], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 4)) >> 4], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[value & 15], fontColor);
	}
}

void printFourHex(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "0000", fontColor);
	}
	else
	{
		value &= 0xFFFF;

		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 12], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 8)) >> 8], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[(value & (15 << 4)) >> 4], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 3), y, hexTable[value & 15], fontColor);
	}
}

void printFiveHex(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor)
{
	if (value == 0)
	{
		textOut(frameBuffer, x, y, "00000", fontColor);
	}
	else
	{
		value &= 0xFFFFF;

		charOut(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value  >> 16], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 12)) >> 12], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[(value & (15 << 8)) >> 8], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 3), y, hexTable[(value & (15 << 4)) >> 4], fontColor);
		charOut(frameBuffer, x + (FONT_CHAR_W * 4), y, hexTable[value & 15], fontColor);
	}
}

void printTwoDecimalsBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "00", fontColor, backColor);
	}
	else
	{
		if (value > 99)
			value = 99;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor, backColor);
	}
}

void printTwoDecimalsBigBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBigBg(frameBuffer, x, y, "00", fontColor, backColor);
	}
	else
	{
		if (value > 99)
			value = 99;

		charOutBigBg(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBigBg(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor, backColor);
	}
}

void printThreeDecimalsBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "000", fontColor, backColor);
	}
	else
	{
		if (value > 999)
			value = 999;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 2), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor, backColor);
	}
}

void printFourDecimalsBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "0000", fontColor, backColor);
	}
	else
	{
		if (value > 9999)
			value = 9999;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 3), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 2), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, '0' + (value % 10), fontColor, backColor); value /= 10;
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, '0' + (value % 10), fontColor, backColor);
	}
}

// this one is used for "DISP:" in the sampler screen (zeroes are padded with space)
void printFiveDecimalsBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	char numberText[6];
	uint8_t i;

	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "    0", fontColor, backColor);
	}
	else
	{
		if (value > 99999)
			value = 99999;

		numberText[5] = 0;
		numberText[4] = '0' + (value % 10); value /= 10;
		numberText[3] = '0' + (value % 10); value /= 10;
		numberText[2] = '0' + (value % 10); value /= 10;
		numberText[1] = '0' + (value % 10); value /= 10;
		numberText[0] = '0' + (value % 10);

		i = 0;
		while (numberText[i] == '0')
			numberText[i++] = ' ';

		textOutBg(frameBuffer, x, y, numberText, fontColor, backColor);
	}
}

// this one is used for module size (zeroes are padded with space)
void printSixDecimalsBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	char numberText[7];
	uint8_t i;

	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "     0", fontColor, backColor);
	}
	else
	{
		if (value > 999999)
			value = 999999;

		numberText[6] = 0;
		numberText[5] = '0' + (value % 10); value /= 10;
		numberText[4] = '0' + (value % 10); value /= 10;
		numberText[3] = '0' + (value % 10); value /= 10;
		numberText[2] = '0' + (value % 10); value /= 10;
		numberText[1] = '0' + (value % 10); value /= 10;
		numberText[0] = '0' + (value % 10);

		i = 0;
		while (numberText[i] == '0')
			numberText[i++] = ' ';

		textOutBg(frameBuffer, x, y, numberText, fontColor, backColor);
	}
}

void printOneHexBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	charOutBg(frameBuffer, x, y, hexTable[value & 0xF], fontColor, backColor);
}

void printOneHexBigBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	charOutBigBg(frameBuffer, x, y, hexTable[value & 0xF], fontColor, backColor);
}

void printTwoHexBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "00", fontColor, backColor);
	}
	else
	{
		value &= 0xFF;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 4], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[value & 15], fontColor, backColor);
	}
}

void printTwoHexBigBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBigBg(frameBuffer, x, y, "00", fontColor, backColor);
	}
	else
	{
		value &= 0xFF;

		charOutBigBg(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 4], fontColor, backColor);
		charOutBigBg(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[value & 15], fontColor, backColor);
	}
}

void printThreeHexBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "000", fontColor, backColor);
	}
	else
	{
		value &= 0xFFF;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 8], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 4)) >> 4], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[value & 15], fontColor, backColor);
	}
}

void printFourHexBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "0000", fontColor, backColor);
	}
	else
	{
		value &= 0xFFFF;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 12], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 8)) >> 8], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[(value & (15 << 4)) >> 4], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 3), y, hexTable[value & 15], fontColor, backColor);
	}
}

void printFiveHexBg(uint32_t *frameBuffer, uint32_t x, uint32_t y, uint32_t value, uint32_t fontColor, uint32_t backColor)
{
	if (value == 0)
	{
		textOutBg(frameBuffer, x, y, "00000", fontColor, backColor);
	}
	else
	{
		value &= 0xFFFFF;

		charOutBg(frameBuffer, x + (FONT_CHAR_W * 0), y, hexTable[value >> 16], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 1), y, hexTable[(value & (15 << 12)) >> 12], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 2), y, hexTable[(value & (15 << 8)) >> 8], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 3), y, hexTable[(value & (15 << 4)) >> 4], fontColor, backColor);
		charOutBg(frameBuffer, x + (FONT_CHAR_W * 4), y, hexTable[value & 15], fontColor, backColor);
	}
}

void setPrevStatusMessage(void)
{
	strcpy(editor.ui.statusMessage, editor.ui.prevStatusMessage);
	editor.ui.updateStatusText = true;
}

void setStatusMessage(const char *msg, bool carry)
{
	assert(msg != NULL);

	if (carry)
		strcpy(editor.ui.prevStatusMessage, msg);

	strcpy(editor.ui.statusMessage, msg);
	editor.ui.updateStatusText = true;
}

void displayMsg(const char *msg)
{
	assert(msg != NULL);

	editor.errorMsgActive = true;
	editor.errorMsgBlock = false;
	editor.errorMsgCounter = 0;

	if (*msg != '\0')
		setStatusMessage(msg, NO_CARRY);
}

void displayErrorMsg(const char *msg)
{
	assert(msg != NULL);

	editor.errorMsgActive = true;
	editor.errorMsgBlock = true;
	editor.errorMsgCounter = 0;

	if (*msg != '\0')
		setStatusMessage(msg, NO_CARRY);

	setErrPointer();
}
