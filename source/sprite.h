/**
 * @file sprite.h
 * @brief スプライトのヘッダ
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/


#ifndef _sprite_h_
#define _sprite_h_

#include <gba.h>
#include "sprite_aim1.h"
#include "sprite_aim2.h"
#include "sprite_aim3.h"
#include "sprite_aim4.h"

#include "sprite_warhead1.h"
#include "sprite_warhead2.h"

#include "sprite_marker1.h"
#include "sprite_marker2.h"

#include "sprite_bomb1.h"
#include "sprite_bomb2.h"
#include "sprite_bomb3.h"
#include "sprite_bomb4.h"

#include "sprite_city1.h"
#include "sprite_city2.h"
#include "sprite_city3.h"
#include "sprite_city4.h"

#include "sprite_amb1.h"
#include "sprite_amb2.h"
#include "sprite_amb3.h"
#include "sprite_amb4.h"

#include "sprite_ufo.h"
#include "sprite_ufo_warhead1.h"
#include "sprite_ufo_warhead2.h"

#include "sprite_item1.h"
#include "sprite_item1_2.h"

#include "sprite_superbomb1.h"
#include "sprite_superbomb2.h"

#include "sprite_arrow.h"



#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義

// スプライト最大数
#define MAX_SPRITE 128


///////////////////////////////////////////////////////////////////// グローバル変数

//////////////////////////////////////////////////////////////////// プロトタイプ
void
init_sprite (void);
void
move_sprite (u16 num, s16 x, s16 y);
void
set_sprite_form (u16 num, u16 size, u16 form, u16 col);
void
set_sprite_tile (u16 num, u16 tile);
void
init_sprite_chr (void);
void
erase_sprite (u16 num);
s16
read_sprite_x (u16 num);
s16
read_sprite_y (u16 num);

#endif	/* _sprite_h_ */
