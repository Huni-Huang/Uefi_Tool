/** @file
  Narrow font Data definition for GraphicsConsole driver.

Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//#include "GraphicsConsole.h"

#include <Protocol/HiiFont.h>


EFI_NARROW_GLYPH  gUsStdNarrowGlyphData[] = {
  //
  // Unicode glyphs from 0x20 to 0x7e are the same as ASCII characters 0x20 to 0x7e
  //
  { 0x0020, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x0021, 0x00, {0x00,0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x0022, 0x00, {0x00,0x00,0x00,0x6C,0x6C,0x6C,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x0023, 0x00, {0x00,0x00,0x00,0x00,0x6C,0x6C,0x6C,0xFE,0x6C,0x6C,0x6C,0xFE,0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00}},
  { 0x0024, 0x00, {0x00,0x00,0x18,0x18,0x7C,0xC6,0xC6,0x60,0x38,0x0C,0x06,0xC6,0xC6,0x7C,0x18,0x18,0x00,0x00,0x00}},
  { 0x0025, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0x0C,0x0C,0x18,0x18,0x30,0x30,0x60,0x60,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x0026, 0x00, {0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0x78,0x76,0xDC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x0027, 0x00, {0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x0028, 0x00, {0x00,0x00,0x00,0x06,0x0C,0x0C,0x18,0x18,0x18,0x18,0x18,0x18,0x0C,0x0C,0x06,0x00,0x00,0x00,0x00}},
  { 0x0029, 0x00, {0x00,0x00,0x00,0xC0,0x60,0x60,0x30,0x30,0x30,0x30,0x30,0x30,0x60,0x60,0xC0,0x00,0x00,0x00,0x00}},
  { 0x002a, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6C,0x38,0xFE,0x38,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x002b, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x002c, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00,0x00}},
  { 0x002d, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x002e, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00}},
  { 0x002f, 0x00, {0x00,0x00,0x00,0x06,0x06,0x0C,0x0C,0x18,0x18,0x30,0x30,0x60,0x60,0xC0,0xC0,0x00,0x00,0x00,0x00}},
  { 0x0030, 0x00, {0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xD6,0xD6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,0x00,0x00}},
  { 0x0031, 0x00, {0x00,0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00}},
  { 0x0032, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0x06,0x06,0x06,0x0C,0x18,0x30,0x60,0xC0,0xC2,0xFE,0x00,0x00,0x00,0x00}},
  { 0x0033, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0x06,0x06,0x06,0x3C,0x06,0x06,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0034, 0x00, {0x00,0x00,0x00,0x1C,0x1C,0x3C,0x3C,0x6C,0x6C,0xCC,0xFE,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00}},
  { 0x0035, 0x00, {0x00,0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xC0,0xFC,0x06,0x06,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0036, 0x00, {0x00,0x00,0x00,0x3C,0x60,0xC0,0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0037, 0x00, {0x00,0x00,0x00,0xFE,0xC6,0x06,0x06,0x06,0x0C,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x0038, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0039, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x06,0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00}},
  { 0x003a, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00}},
  { 0x003b, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00}},
  { 0x003c, 0x00, {0x00,0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00}},
  { 0x003d, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x003e, 0x00, {0x00,0x00,0x00,0x00,0xC0,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0xC0,0x00,0x00,0x00,0x00}},
  { 0x003f, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0x0C,0x0C,0x18,0x18,0x18,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x0040, 0x00, {0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xDE,0xDE,0xDE,0xDC,0xC0,0xC0,0x7E,0x00,0x00,0x00,0x00}},

  { 0x0041, 0x00, {0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},

  { 0x0042, 0x00, {0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x66,0x7C,0x66,0x66,0x66,0x66,0x66,0xFC,0x00,0x00,0x00,0x00}},
  { 0x0043, 0x00, {0x00,0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC2,0x66,0x3C,0x00,0x00,0x00,0x00}},
  { 0x0044, 0x00, {0x00,0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,0x00}},
  { 0x0045, 0x00, {0x00,0x00,0x00,0xFE,0x66,0x62,0x60,0x68,0x78,0x68,0x60,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x0046, 0x00, {0x00,0x00,0x00,0xFE,0x66,0x62,0x60,0x64,0x7C,0x64,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00}},
  { 0x0047, 0x00, {0x00,0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xDE,0xC6,0xC6,0xC6,0x66,0x3C,0x00,0x00,0x00,0x00}},
  { 0x0048, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x0049, 0x00, {0x00,0x00,0x00,0xFC,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,0x00,0x00}},
  { 0x004a, 0x00, {0x00,0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00,0x00,0x00,0x00}},
  { 0x004b, 0x00, {0x00,0x00,0x00,0xE6,0x66,0x6C,0x6C,0x78,0x70,0x78,0x6C,0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,0x00}},
  { 0x004c, 0x00, {0x00,0x00,0x00,0xF0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x004d, 0x00, {0x00,0x00,0x00,0xC6,0xEE,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x004e, 0x00, {0x00,0x00,0x00,0xC6,0xE6,0xF6,0xF6,0xF6,0xDE,0xCE,0xCE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x004f, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0050, 0x00, {0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00}},
  { 0x0051, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0x7C,0x1C,0x0E,0x00,0x00}},
  { 0x0052, 0x00, {0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x66,0x7C,0x78,0x6C,0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,0x00}},
  { 0x0053, 0x00, {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x60,0x38,0x0C,0x06,0x06,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0054, 0x00, {0x00,0x00,0x00,0xFC,0xFC,0xB4,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00}},
  { 0x0055, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0056, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x10,0x00,0x00,0x00,0x00}},
  { 0x0057, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0xD6,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00}},
  { 0x0058, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0xC6,0x6C,0x6C,0x38,0x6C,0x6C,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x0059, 0x00, {0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00}},
  { 0x005a, 0x00, {0x00,0x00,0x00,0xFE,0xC6,0x86,0x0C,0x0C,0x18,0x30,0x60,0xC0,0xC2,0xC6,0xFE,0x00,0x00,0x00,0x00}},
  { 0x005b, 0x00, {0x00,0x00,0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1E,0x00,0x00,0x00,0x00}},
  { 0x005c, 0x00, {0x00,0x00,0x00,0xC0,0xC0,0x60,0x60,0x30,0x30,0x18,0x18,0x0C,0x0C,0x06,0x06,0x00,0x00,0x00,0x00}},
  { 0x005d, 0x00, {0x00,0x00,0x00,0xF0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xF0,0x00,0x00,0x00,0x00}},
  { 0x005e, 0x00, {0x00,0x00,0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x005f, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00}},
  { 0x0060, 0x00, {0x00,0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x0061, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x0062, 0x00, {0x00,0x00,0x00,0xE0,0x60,0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x66,0x66,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0063, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,0xC0,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0064, 0x00, {0x00,0x00,0x00,0x1C,0x0C,0x0C,0x0C,0x3C,0x6C,0xCC,0xCC,0xCC,0xCC,0xCC,0x7E,0x00,0x00,0x00,0x00}},
  { 0x0065, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0066, 0x00, {0x00,0x00,0x00,0x1E,0x33,0x30,0x30,0x30,0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00}},
  { 0x0067, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0xCC,0x78,0x00}},
  { 0x0068, 0x00, {0x00,0x00,0x00,0xE0,0x60,0x60,0x60,0x7C,0x76,0x66,0x66,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00}},
  { 0x0069, 0x00, {0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x006a, 0x00, {0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00}},
  { 0x006b, 0x00, {0x00,0x00,0x00,0xE0,0x60,0x60,0x66,0x6C,0x78,0x70,0x78,0x6C,0x6C,0x66,0xE6,0x00,0x00,0x00,0x00}},
  { 0x006c, 0x00, {0x00,0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x006d, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0xEE,0xFE,0xD6,0xD6,0xD6,0xD6,0xD6,0x00,0x00,0x00,0x00}},
  { 0x006e, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00}},
  { 0x006f, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0070, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00}},
  { 0x0071, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0x0C,0x1E,0x00}},
  { 0x0072, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x60,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00}},
  { 0x0073, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,0x7C,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x0074, 0x00, {0x00,0x00,0x00,0x10,0x30,0x30,0x30,0xFC,0x30,0x30,0x30,0x30,0x30,0x36,0x1C,0x00,0x00,0x00,0x00}},
  { 0x0075, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x0076, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00,0x00,0x00,0x00}},
  { 0x0077, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xD6,0xD6,0xFE,0xEE,0x6C,0x00,0x00,0x00,0x00}},
  { 0x0078, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x38,0x6C,0x6C,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x0079, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0xF8,0x00}},
  { 0x007a, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x86,0x0C,0x18,0x30,0x60,0xC0,0xFE,0x00,0x00,0x00,0x00}},
  { 0x007b, 0x00, {0x00,0x00,0x00,0x0E,0x18,0x18,0x18,0x18,0x30,0x18,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00}},
  { 0x007c, 0x00, {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x007d, 0x00, {0x00,0x00,0x00,0xE0,0x30,0x30,0x30,0x30,0x18,0x30,0x30,0x30,0x30,0x30,0xE0,0x00,0x00,0x00,0x00}},
  { 0x007e, 0x00, {0x00,0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},

  { 0x00a0, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00a1, 0x00, {0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x3C,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00}},
  { 0x00a2, 0x00, {0x00,0x00,0x00,0x00,0x18,0x18,0x7C,0xC6,0xC0,0xC0,0xC0,0xC6,0x7C,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x00a3, 0x00, {0x00,0x00,0x00,0x38,0x6C,0x64,0x60,0x60,0xF0,0x60,0x60,0x60,0x60,0xE6,0xFC,0x00,0x00,0x00,0x00}},
  { 0x00a4, 0x00, {0x00,0x00,0x18,0x00,0x00,0x00,0xC6,0x7C,0xC6,0xC6,0xC6,0xC6,0x7C,0xC6,0x00,0x00,0x00,0x00,0x00}},
  { 0x00a5, 0x00, {0x00,0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,0x7E,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x00a6, 0x00, {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00}},
  { 0x00a7, 0x00, {0x00,0x00,0x18,0x7C,0xC6,0x60,0x38,0x6C,0xC6,0xC6,0x6C,0x38,0x0C,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00a8, 0x00, {0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00a9, 0x00, {0x00,0x00,0x00,0x00,0x7C,0x82,0x9A,0xA2,0xA2,0xA2,0x9A,0x82,0x7C,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00aa, 0x00, {0x00,0x00,0x00,0x00,0x3C,0x6C,0x6C,0x6C,0x3E,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00ab, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x6C,0xD8,0x6C,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00ac, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00ad, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00ae, 0x00, {0x00,0x00,0x00,0x00,0x7C,0x82,0xB2,0xAA,0xAA,0xB2,0xAA,0xAA,0x82,0x7C,0x00,0x00,0x00,0x00,0x00}},
  { 0x00af, 0x00, {0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b0, 0x00, {0x00,0x00,0x00,0x38,0x6C,0x6C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b1, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b2, 0x00, {0x00,0x00,0x00,0x3C,0x66,0x0C,0x18,0x32,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b3, 0x00, {0x00,0x00,0x00,0x7C,0x06,0x3C,0x06,0x06,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b4, 0x00, {0x00,0x00,0x00,0x0C,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b5, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0xC0,0x00}},
  { 0x00b6, 0x00, {0x00,0x00,0x00,0x7F,0xDB,0xDB,0xDB,0xDB,0x7B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x00,0x00,0x00,0x00}},
  { 0x00b7, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00b8, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x0C,0x78,0x00,0x00,0x00}},
  { 0x00b9, 0x00, {0x00,0x00,0x00,0x18,0x38,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00ba, 0x00, {0x00,0x00,0x00,0x00,0x38,0x6C,0x6C,0x6C,0x38,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00bb, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x6C,0x36,0x6C,0xD8,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00bc, 0x00, {0x00,0x00,0x00,0x60,0xE0,0x62,0x66,0x6C,0x18,0x30,0x66,0xCE,0x9A,0x3F,0x06,0x06,0x00,0x00,0x00}},
  { 0x00bd, 0x00, {0x00,0x00,0x00,0x60,0xE0,0x62,0x66,0x6C,0x18,0x30,0x60,0xDC,0x86,0x0C,0x18,0x3E,0x00,0x00,0x00}},
  { 0x00be, 0x00, {0x00,0x00,0x00,0xE0,0x30,0x62,0x36,0xEC,0x18,0x30,0x66,0xCE,0x9A,0x3F,0x06,0x06,0x00,0x00,0x00}},
  { 0x00bf, 0x00, {0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x30,0x30,0x60,0x60,0xC0,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00c0, 0x00, {0x60,0x30,0x18,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c1, 0x00, {0x18,0x30,0x60,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c2, 0x00, {0x10,0x38,0x6C,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c3, 0x00, {0x76,0xDC,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c4, 0x00, {0xCC,0xCC,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c5, 0x00, {0x38,0x6C,0x38,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00c6, 0x00, {0x00,0x00,0x00,0x00,0x3E,0x6C,0xCC,0xCC,0xCC,0xFE,0xCC,0xCC,0xCC,0xCC,0xCE,0x00,0x00,0x00,0x00}},
  { 0x00c7, 0x00, {0x00,0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC2,0x66,0x3C,0x18,0x70,0x00,0x00}},
  { 0x00c8, 0x00, {0x60,0x30,0x18,0x00,0xFE,0x66,0x62,0x60,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x00c9, 0x00, {0x18,0x30,0x60,0x00,0xFE,0x66,0x62,0x60,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x00ca, 0x00, {0x10,0x38,0x6C,0x00,0xFE,0x66,0x62,0x60,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x00cb, 0x00, {0xCC,0xCC,0x00,0x00,0xFE,0x66,0x62,0x60,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00}},
  { 0x00cc, 0x00, {0x60,0x30,0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00cd, 0x00, {0x18,0x30,0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00ce, 0x00, {0x10,0x38,0x6C,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00cf, 0x00, {0xCC,0xCC,0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00d0, 0x00, {0x00,0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0xF6,0x66,0x66,0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,0x00}},
  { 0x00d1, 0x00, {0x76,0xDC,0x00,0x00,0xC6,0xE6,0xE6,0xF6,0xF6,0xDE,0xDE,0xCE,0xCE,0xC6,0xC6,0x00,0x00,0x00,0x00}},
  { 0x00d2, 0x00, {0x60,0x30,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00d3, 0x00, {0x18,0x30,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00d4, 0x00, {0x10,0x38,0x6C,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00d5, 0x00, {0x76,0xDC,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00d6, 0x00, {0xCC,0xCC,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00d7, 0x00, {0x10,0x28,0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x38,0x6C,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00}},
  { 0x00d8, 0x00, {0x00,0x00,0x00,0x7C,0xCE,0xCE,0xDE,0xD6,0xD6,0xD6,0xD6,0xF6,0xE6,0xE6,0x7C,0x40,0x00,0x00,0x00}},
  { 0x00d9, 0x00, {0x60,0x30,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00da, 0x00, {0x18,0x30,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00db, 0x00, {0x10,0x38,0x6C,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00dc, 0x00, {0xCC,0xCC,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00dd, 0x00, {0x18,0x30,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00de, 0x00, {0x00,0x00,0x10,0x00,0xF0,0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x7C,0x60,0xF0,0x00,0x00,0x00,0x00}},
  { 0x00df, 0x00, {0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0xCC,0xD8,0xCC,0xC6,0xC6,0xC6,0xC6,0xCC,0x00,0x00,0x00,0x00}},
  { 0x00e0, 0x00, {0x00,0x30,0x30,0x60,0x30,0x18,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e1, 0x00, {0x00,0x00,0x00,0x18,0x30,0x60,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e2, 0x00, {0x00,0x00,0x00,0x10,0x38,0x6C,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e3, 0x00, {0x00,0x00,0x00,0x00,0x76,0xDC,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e4, 0x00, {0x00,0x00,0x00,0xCC,0xCC,0x00,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e5, 0x00, {0x00,0x00,0x00,0x38,0x6C,0x38,0x00,0x78,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00e6, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x36,0x36,0x7E,0xD8,0xD8,0xD8,0x6E,0x00,0x00,0x00,0x00}},
  { 0x00e7, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,0xC0,0xC0,0xC0,0xC6,0x7C,0x18,0x70,0x00,0x00}},
  { 0x00e8, 0x00, {0x00,0x00,0x00,0x60,0x30,0x18,0x00,0x7C,0xC6,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00e9, 0x00, {0x00,0x00,0x00,0x0C,0x18,0x30,0x00,0x7C,0xC6,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00ea, 0x00, {0x00,0x00,0x00,0x10,0x38,0x6C,0x00,0x7C,0xC6,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00eb, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0x00,0x00,0x7C,0xC6,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00ec, 0x00, {0x00,0x00,0x00,0x60,0x30,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00ed, 0x00, {0x00,0x00,0x00,0x0C,0x18,0x30,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00ee, 0x00, {0x00,0x00,0x00,0x18,0x3C,0x66,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00ef, 0x00, {0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00f0, 0x00, {0x00,0x00,0x00,0x34,0x18,0x2C,0x0C,0x06,0x3E,0x66,0x66,0x66,0x66,0x66,0x3C,0x00,0x00,0x00,0x00}},
  { 0x00f1, 0x00, {0x00,0x00,0x00,0x00,0x76,0xDC,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00}},
  { 0x00f2, 0x00, {0x00,0x00,0x00,0x60,0x30,0x18,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f3, 0x00, {0x00,0x00,0x00,0x18,0x30,0x60,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f4, 0x00, {0x00,0x00,0x00,0x10,0x38,0x6C,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f5, 0x00, {0x00,0x00,0x00,0x00,0x76,0xDC,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f6, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f7, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x7E,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { 0x00f8, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xCE,0xDE,0xD6,0xF6,0xE6,0xC6,0x7C,0x00,0x00,0x00,0x00}},
  { 0x00f9, 0x00, {0x00,0x00,0x00,0x60,0x30,0x18,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00fa, 0x00, {0x00,0x00,0x00,0x18,0x30,0x60,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00fb, 0x00, {0x00,0x00,0x00,0x30,0x78,0xCC,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00fc, 0x00, {0x00,0x00,0x00,0xCC,0xCC,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00}},
  { 0x00fd, 0x00, {0x00,0x00,0x00,0x0C,0x18,0x30,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0xF8,0x00}},
  { 0x00fe, 0x00, {0x00,0x00,0x00,0xE0,0x60,0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00}},
  { 0x00ff, 0x00, {0x00,0x00,0x00,0xC6,0xC6,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0x78,0x00}},
/*  
  { (CHAR16)BOXDRAW_HORIZONTAL,                 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_VERTICAL,                   0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_RIGHT,                 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_LEFT,                  0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_UP_RIGHT,                   0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_UP_LEFT,                    0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_VERTICAL_RIGHT,             0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_VERTICAL_LEFT,              0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_HORIZONTAL,            0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_UP_HORIZONTAL,              0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_VERTICAL_HORIZONTAL,        0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOUBLE_HORIZONTAL,          0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_DOUBLE_VERTICAL,            0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOWN_RIGHT_DOUBLE,          0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x18,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_DOUBLE_RIGHT,          0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_DOWN_RIGHT,          0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x30,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOWN_LEFT_DOUBLE,           0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x18,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_DOUBLE_LEFT,           0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_DOWN_LEFT,           0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x06,0xF6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_UP_RIGHT_DOUBLE,            0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x18,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_UP_DOUBLE_RIGHT,            0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_DOUBLE_UP_RIGHT,            0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x30,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_UP_LEFT_DOUBLE,             0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x18,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_UP_DOUBLE_LEFT,             0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_DOUBLE_UP_LEFT,             0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xF6,0x06,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_VERTICAL_RIGHT_DOUBLE,      0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x18,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_VERTICAL_DOUBLE_RIGHT,      0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_VERTICAL_RIGHT,      0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x30,0x37,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_VERTICAL_LEFT_DOUBLE,       0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x18,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_VERTICAL_DOUBLE_LEFT,       0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xF6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_VERTICAL_LEFT,       0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xF6,0x06,0xF6,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOWN_HORIZONTAL_DOUBLE,     0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_DOWN_DOUBLE_HORIZONTAL,     0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_DOWN_HORIZONTAL,     0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xF7,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_UP_HORIZONTAL_DOUBLE,       0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_UP_DOUBLE_HORIZONTAL,       0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_DOUBLE_UP_HORIZONTAL,       0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xF7,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE, 0x00, {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0x18,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
  { (CHAR16)BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL, 0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xFF,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},
  { (CHAR16)BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL, 0x00, {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0xF7,0x00,0xF7,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}},

  { (CHAR16)BLOCKELEMENT_FULL_BLOCK,            0x00, {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},
  { (CHAR16)BLOCKELEMENT_LIGHT_SHADE,           0x00, {0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22}},

  { (CHAR16)GEOMETRICSHAPE_RIGHT_TRIANGLE,      0x00, {0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0xF8,0xFE,0xF8,0xF0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)GEOMETRICSHAPE_LEFT_TRIANGLE,       0x00, {0x00,0x00,0x00,0x00,0x00,0x06,0x0E,0x1E,0x3E,0xFE,0x3E,0x1E,0x0E,0x06,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)GEOMETRICSHAPE_UP_TRIANGLE,         0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x38,0x7C,0x7C,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)GEOMETRICSHAPE_DOWN_TRIANGLE,       0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0x7C,0x7C,0x38,0x38,0x10,0x00,0x00,0x00,0x00,0x00,0x00}},

  { (CHAR16)ARROW_UP,                           0x00, {0x00,0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)ARROW_DOWN,                         0x00, {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)ARROW_LEFT,                         0x00, {0x00,0x00,0x00,0x00,0x00,0x20,0x60,0x60,0xFE,0xFE,0x60,0x60,0x20,0x00,0x00,0x00,0x00,0x00,0x00}},
  { (CHAR16)ARROW_RIGHT,                        0x00, {0x00,0x00,0x00,0x00,0x00,0x08,0x0C,0x0C,0xFE,0xFE,0x0C,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00}},

  { 0x0000, 0x00, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} //EOL
  */
};

// Get available Unicode glyphs narrow fonts(8*19 pixels) size.
UINT32 mNarrowFontSize =  sizeof (gUsStdNarrowGlyphData);

