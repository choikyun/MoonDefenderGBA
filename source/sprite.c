/**
 * @file sprite.c
 * @brief スプライト
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <gba.h>

#include "sprite.h"
#include "bg.h"

/***************************************************
 スプライト初期化
 ***************************************************/
void
init_sprite (void)
{
  int i;

  //すべて画面外へ
  for (i = 0; i < MAX_SPRITE; i++)
  {
    move_sprite (i, SCREEN_WIDTH, SCREEN_HEIGHT);
  }
}

/***************************************************
 スプライト非表示
 ***************************************************/
void
erase_sprite (u16 num)
{
  move_sprite (num, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/***************************************************
 スプライト移動
 ***************************************************/
void
move_sprite (u16 num, s16 x, s16 y)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr1 &= 0xfe00;
  sp->attr0 &= 0xff00;
  sp->attr1 |= OBJ_X(x);
  sp->attr0 |= OBJ_Y(y);
}

/***************************************************
 スプライト形状　カラー サイズ
 ***************************************************/
void
set_sprite_form (u16 num, u16 size, u16 form, u16 col)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr0 &= 0x1fff;
  sp->attr1 &= 0x3fff;
  sp->attr0 |= col | form;
  sp->attr1 |= size;
}

/***************************************************
 タイルキャラクタ設定
 **************************************************/
void
set_sprite_tile (u16 num, u16 tile)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  sp->attr2 &= 0xfc00;
  sp->attr2 |= tile;
}

/***************************************************
 スプライトX座標読み取り
 ***************************************************/
s16
read_sprite_x (u16 num)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  return sp->attr1 & ~0xfe00;
}

/***************************************************
 スプライトY座標読み取り
 ***************************************************/
s16
read_sprite_y (u16 num)
{
  OBJATTR *sp = (OBJATTR*) OAM + num;

  return sp->attr0 & ~0xff00;
}

/***************************************************
 キャラクタ初期化
 ***************************************************/
void
init_sprite_chr (void)
{
  // スプライトデータ
  u16 *oam = DEF_MODE < MODE_3 ? OBJ_BASE_ADR : BITMAP_OBJ_BASE_ADR;

  // スプライトパレット
  u16 *pal = OBJ_COLORS;

  // 共通パレット転送
  CpuSet (sprite_aim1Pal, pal, (COPY32 | sprite_aim1PalLen / 4));

  // キャラクタデータ転送

  //aim
  CpuSet (sprite_aim1Tiles, oam, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_aim2Tiles, oam + 32, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_aim3Tiles, oam + 32 * 2, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_aim4Tiles, oam + 32 * 3, (COPY32 | sprite_aim1TilesLen / 4));

  //warhead
  CpuSet (sprite_warhead1Tiles, oam + 32 * 4, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_warhead2Tiles, oam + 32 * 5, (COPY32 | sprite_aim1TilesLen / 4));

  //ufo warhead
  CpuSet (sprite_ufo_warhead1Tiles, oam + 32 * 6, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_ufo_warhead2Tiles, oam + 32 * 7, (COPY32 | sprite_aim1TilesLen / 4));

  // marker
  CpuSet (sprite_marker1Tiles, oam + 32 * 8, (COPY32 | sprite_aim1TilesLen / 4));
  CpuSet (sprite_marker2Tiles, oam + 32 * 9, (COPY32 | sprite_aim1TilesLen / 4));

  //city
  CpuSet (sprite_city1Tiles, oam + 32 * 10, (COPY32 | sprite_city1TilesLen / 4));
  CpuSet (sprite_city2Tiles, oam + 32 * 10 + 128, (COPY32 | sprite_city1TilesLen / 4));
  CpuSet (sprite_city3Tiles, oam + 32 * 10 + 128 * 2, (COPY32 | sprite_city1TilesLen / 4));
  CpuSet (sprite_city4Tiles, oam + 32 * 10 + 128 * 3, (COPY32 | sprite_city1TilesLen / 4));

  //amb
  CpuSet (sprite_amb1Tiles, oam + 32 * 10 + 128 * 4, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_amb2Tiles, oam + 32 * 10 + 128 * 5, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_amb3Tiles, oam + 32 * 10 + 128 * 6, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_amb4Tiles, oam + 32 * 10 + 128 * 7, (COPY32 | sprite_amb1TilesLen / 4));

  // bomb
  CpuSet (sprite_bomb1Tiles, oam + 32 * 10 + 128 * 8, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_bomb2Tiles, oam + 32 * 10 + 128 * 9, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_bomb3Tiles, oam + 32 * 10 + 128 * 10, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_bomb4Tiles, oam + 32 * 10 + 128 * 11, (COPY32 | sprite_amb1TilesLen / 4));

  // ufo
  CpuSet (sprite_ufoTiles, oam + 32 * 10 + 128 * 12, (COPY32 | sprite_amb1TilesLen / 4));

  // item1
  CpuSet (sprite_item1Tiles, oam + 32 * 10 + 128 * 13, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_item1_2Tiles, oam + 32 * 10 + 128 * 14, (COPY32 | sprite_amb1TilesLen / 4));

  // superbomb
  CpuSet (sprite_superbomb1Tiles, oam + 32 * 10 + 128 * 15, (COPY32 | sprite_amb1TilesLen / 4));
  CpuSet (sprite_superbomb2Tiles, oam + 32 * 10 + 128 * 16, (COPY32 | sprite_amb1TilesLen / 4));

  // mode arrow
  CpuSet (sprite_arrowTiles, oam + 32 * 10 + 128 * 17, (COPY32 | sprite_amb1TilesLen / 4));

}
