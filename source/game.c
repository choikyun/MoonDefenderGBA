/**
 * @file game.c
 * @brief ゲーム本体
 * @date  2017/02/01 作成
 * @date  2017/03/03 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/

#define GLOBAL_VALUE_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gba.h>

#include "game.h"
#include "bg.h"
#include "sprite.h"
#include "score.h"
#include "music.h"
#include "graph.h"

static void
disp_title ();
static void
disp_over ();
static void
disp_clear ();
static void
restart ();
static void
init_stage ();
static void
pause ();
static void
add_score (int);
static void
sub_score (int);
static void
update_lv ();
static void
update_score ();
static void
update_remining_missiles ();
static void
disp_num (int, int, u16);
static void
demo_end ();
static void
select_mode ();
static void
init_aim ();
static void
disp_aim ();
static void
move_aim ();
static void
init_amb ();
static void
check_boundary ();
static void
disp_cities ();
static void
init_cities ();
static void
init_sprite_setting ();
static void
disp_amb ();
static void
fire_enemy_missile (int, int, bool);
static void
init_missile ();
static bool
is_attacked_target (MissileType *);
static void
draw_line (MissileType *);
static void
move_enemy_missile ();
static void
disp_warhead (int, MissileType *);
static void
manage_stage_level ();
static void
set_marker ();
static void
disp_marker ();
static void
aim_missile ();
static void
fire_amb_missile ();
static void
move_amb_missile ();
static void
set_bomb (int, int);
static void
disp_bomb ();
static bool
is_hit_to_bombs (int, int, int, int, int);
static void
reset_message (BlinkMessageType *);
static void
reset_message_fast (BlinkMessageType *);
static void
disp_warning ();
static void
draw_bg ();
static void
drop_items ();
static void
drop_supply ();
static void
drop_superbomb ();
static void
move_items ();
static void
move_superbomb ();
static void
init_items ();
static void
level_up ();
static void
add_enemy_score (int);
static void
move_ufo ();
static void
init_ufo ();
static void
fire_ufo_missile (int, int);
static void
disp_num_title (int, int, u16);
static void
explosion ();
static void
flash ();
static void
select_mode ();
static void
disp_pause ();
static void
trophy ();
static void
disp_trophy_mes ();
static void
disp_trophy ();


//debug
void
vbaPrint (char *s);


/**
 * @brief 都市座標
 */
static int city_coord[MAX_CITIES] = { 16, 48, 80, 144, 176, 208 };


/**********************************************//**
 * @brief メインループ
 ***********************************************/
void
game ()
{
  game_state.key = keysDown ();
  game_state.keyr = keysDownRepeat ();

  seed++;

  switch (game_state.scene)
  {
    case GAME_TITLE:
      disp_title ();
      select_mode ();
      break;

    case GAME_DEMO:
      break;

    case GAME_MAIN:
      draw_bg ();
      move_amb_missile ();
      move_enemy_missile ();
      move_items ();
      move_superbomb ();
      move_ufo ();
      move_aim ();
      update_score ();
      update_lv ();
      update_remining_missiles ();
      trophy ();
      level_up ();
      disp_trophy_mes ();
      manage_stage_level ();
      disp_amb ();
      disp_marker ();
      disp_bomb ();
      disp_cities ();
      disp_aim ();
      pause ();
      break;

    case GAME_READY:
      draw_bg ();
      update_score ();
      update_lv ();
      update_remining_missiles ();
      disp_warning ();
      disp_amb ();
      disp_cities ();
      break;

    case GAME_PAUSE:
      draw_bg ();
      update_score ();
      update_lv ();
      update_remining_missiles ();
      disp_amb ();
      disp_pause ();
      pause ();
      break;

    case GAME_OVER:
      draw_bg ();
      move_enemy_missile ();
      move_items ();
      move_superbomb ();
      move_ufo ();
      update_score ();
      update_lv ();
      update_remining_missiles ();
      disp_bomb ();
      disp_over ();
      break;
  }
}



/**********************************************//**
 * @brief 自機ミサイル発射
 ***********************************************/
static void
aim_missile ()
{
  if (game_state.scene != GAME_MAIN)
    return;

  if ((game_state.key & KEY_A))
  {
    aim.fire_interval = AIM_FIRE_INTERVAL;
  }
}



/**********************************************//**
 * @brief 照準移動
 ***********************************************/
static void
move_aim ()
{
  u16 key = game_state.keyr;

  // 4方向
  if (key & KEY_UP)
  {
    aim.acc.y -= AIM_ACC;
    if (aim.acc.y < -AIM_ACC_MAX)
      aim.acc.y = -AIM_ACC_MAX;
  }
  if (key & KEY_DOWN)
  {
    aim.acc.y += AIM_ACC;
    if (aim.acc.y > AIM_ACC_MAX)
      aim.acc.y = AIM_ACC_MAX;
  }
  if (key & KEY_LEFT)
  {
    aim.acc.x -= AIM_ACC;
    if (aim.acc.x < -AIM_ACC_MAX)
      aim.acc.x = -AIM_ACC_MAX;
  }
  if (key & KEY_RIGHT)
  {
    aim.acc.x += AIM_ACC;
    if (aim.acc.x > AIM_ACC_MAX)
      aim.acc.x = AIM_ACC_MAX;
  }

  // 摩擦（減速）
  if (!(key & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)))
  {
    aim.acc.x += -aim.acc.x * AIM_FRIC;
    aim.acc.y += -aim.acc.y * AIM_FRIC;
  }

  aim.sprite.coord.x += aim.acc.x;
  aim.sprite.coord.y += aim.acc.y;

  // 境界チェック
  check_boundary ();

  // 自機ミサイル発射
  aim_missile ();
}



/**********************************************//**
 * @brief 境界チェック
 ***********************************************/
static void
check_boundary ()
{
  int x = aim.sprite.coord.x >> FIX;
  int y = aim.sprite.coord.y >> FIX;

  if (x < STAGE_W_MIN)
  {
    aim.sprite.coord.x = STAGE_W_MIN << FIX;
    aim.acc.x = -aim.acc.x / 2;
  }
  else if (x > STAGE_W_MAX)
  {
    aim.sprite.coord.x = STAGE_W_MAX << FIX;
    aim.acc.x = -aim.acc.x / 2;
  }

  if (y < STAGE_H_MIN)
  {
    aim.sprite.coord.y = STAGE_H_MIN << FIX;
    aim.acc.y = -aim.acc.y / 2;
  }
  else if (y > STAGE_H_MAX)
  {
    aim.sprite.coord.y = STAGE_H_MAX << FIX;
    aim.acc.y = -aim.acc.y / 2;
  }
}



/**********************************************//**
 * @brief 照準表示
 ***********************************************/
static void
disp_aim ()
{
  if (aim.fire_interval)
  {
    // 発射までのインターバル
    if (!--aim.fire_interval)
      fire_amb_missile ();
  }

  // 弾数警告
  if (am.max_missiles <= AMB_MISSILE_CAUTION)
  {
    // タイル変更
    set_sprite_tile (SPRITE_AIM, TILE_AIM1 + aim.sprite.show * TILE_SIZE_16);

    if (!--aim.flash.interval)
    {
      aim.flash.interval = AIM_CAUTION_INTERVAL;
      aim.sprite.show ^= 1;
    }
  }
  else
  {
    // タイル変更
    set_sprite_tile (SPRITE_AIM, TILE_AIM1);
    aim.flash.interval = AIM_CAUTION_INTERVAL;
  }

  move_sprite (aim.sprite.chr, aim.sprite.coord.x >> FIX, aim.sprite.coord.y >> FIX);
}



/**********************************************//**
 * @brief マーカーセット
 ***********************************************/
static void
set_marker ()
{
  // スプライト
  am.marker_buf[am.marker_index].sprite.chr = SPRITE_MARKER;
  am.marker_buf[am.marker_index].sprite.coord.x = aim.sprite.coord.x >> FIX;
  am.marker_buf[am.marker_index].sprite.coord.y = aim.sprite.coord.y >> FIX;

  // アニメ
  am.marker_buf[am.marker_index].anime.frame = 0;
  am.marker_buf[am.marker_index].anime.max_frame = WARHEAD_ANIME_MAX_FRAME;
  am.marker_buf[am.marker_index].anime.interval =
  am.marker_buf[am.marker_index].anime.interval_rel = WARHEAD_ANIME_INTERVAL;

  am.marker_index++;

  PlaySound(SOUND_ROCKON);
}



/**********************************************//**
 * @brief マーカー表示
 ***********************************************/
static void
disp_marker ()
{
  int max = am.marker_index;
  if (!max)
    return;

  for (int i = 0; i < max; i++)
  {
    // アニメ
    if (--am.marker_buf[i].anime.interval == 0)
    {
      am.marker_buf[i].anime.interval = am.marker_buf[i].anime.interval_rel;
      am.marker_buf[i].anime.frame = (am.marker_buf[i].anime.frame + 1) & am.marker_buf[i].anime.max_frame;
      // タイル変更
      set_sprite_tile (SPRITE_MARKER + i, TILE_MARKER1 + am.marker_buf[i].anime.frame * TILE_SIZE_16);
    }
    move_sprite (am.marker_buf[i].sprite.chr + i, am.marker_buf[i].sprite.coord.x, am.marker_buf[i].sprite.coord.y);
  }
}



/**********************************************//**
 * @brief AMBミサイル発射
 ***********************************************/
static void
fire_amb_missile ()
{
  if (am.max_missiles > 0 && am.index < MAX_AMB_MISSILES)
  {
    // マーカー
    set_marker ();

    // 発射座標
    int sx = AMB_C;
    am.buf[am.index].start.x = sx;
    am.buf[am.index].start.y = AMB_Y;

    // 目標座標
    int ex = (aim.sprite.coord.x >> FIX) + AIM_ADJUST_X;
    am.buf[am.index].end.x = ex;
    am.buf[am.index].end.y = (aim.sprite.coord.y >> FIX) + AIM_ADJUST_Y;

    // XY方向
    am.buf[am.index].direc.x = sx < ex ? 1 : -1;
    am.buf[am.index].direc.y = -1;

    // XY傾き
    int dx = abs (ex - sx);
    int dy = abs (am.buf[am.index].end.y - AMB_Y);
    am.buf[am.index].delta.x = dx;
    am.buf[am.index].delta.y = dy;

    // ステップ
    am.buf[am.index].step = 3;
    am.buf[am.index].max_step = dx >= dy ? dx + 1 : dy + 1;

    // スピード
    am.buf[am.index].speed = am.buf[am.index].speed_rel = AMB_MISSILE_SPEED;

    // 多弾頭か
    am.buf[am.index].is_multiple = false;

    // 弾頭スプライト
    am.buf[am.index].sprite.chr = SPRITE_WARHEAD_AMB;
    am.buf[am.index].sprite.tile = TILE_WARHEAD1;
    am.buf[am.index].sprite.coord.x = sx + WARHEAD_ADJUST_X;
    am.buf[am.index].sprite.coord.y = AMB_Y + WARHEAD_ADJUST_Y;

    // 弾頭アニメ
    am.buf[am.index].anime.frame = 0;
    am.buf[am.index].anime.interval =
    am.buf[am.index].anime.interval_rel = WARHEAD_ANIME_INTERVAL;
    am.buf[am.index].anime.max_frame = WARHEAD_ANIME_MAX_FRAME;

    am.index++;
    am.max_missiles--;
  }
}



/**********************************************//**
 * @brief 対空砲表示
 ***********************************************/
static void
disp_amb ()
{
  // オーバー判定
  if (!game_state.intermission && is_hit_to_bombs (AMB_X, AMB_Y, BOMB_HIT_WIDTH, BOMB_HIT_HEIGHT, BOMB_ANIME_MAX_FRAME))
  {
    PlaySound(SOUND_CRASH_CITY);

    flash ();

    game_state.intermission = INTERMISSION;
    game_state.next_scene = GAME_OVER;
    // タイル変更
    set_sprite_tile (SPRITE_AMB, TILE_AMB1 + TILE_SIZE_16);

    // 爆風発生
    set_bomb (amb.sprite.coord.x - BOMB_ADJUST_X, amb.sprite.coord.y);
  }
  else if (!game_state.intermission && !am.max_missiles && !am.index && !bomb.index)
  {
    flash ();

    game_state.intermission = INTERMISSION;
    game_state.next_scene = GAME_OVER;


  }

  move_sprite (amb.sprite.chr, amb.sprite.coord.x, amb.sprite.coord.y);
}



/**********************************************//**
 * @brief 都市表示
 ***********************************************/
static void
disp_cities ()
{
  for (int i = 0; i < MAX_CITIES; i++)
  {
    if (is_hit_to_bombs (cities[i].sprite.coord.x, cities[i].sprite.coord.y, HIT_CITY_W, HIT_CITY_H, 0))
    {
      PlaySound(SOUND_CRASH_CITY);

      flash();

      if (cities[i].damage < CITY_MAX_DAMAGE)
      {
        cities[i].damage++;
        // タイル書き換え
        set_sprite_tile (cities[i].sprite.chr, TILE_CITY1 + cities[i].damage * TILE_SIZE_16);
      }
      if (cities[i].damage == CITY_MAX_DAMAGE)
      {
        cities[i].damage++;

        // 破壊された
        stage.cities--;
        if (!stage.cities)
        {
         // ゲームオーバー
          game_state.intermission = INTERMISSION;
          game_state.next_scene = GAME_OVER;
        }
      }
      else if (cities[i].damage > CITY_MAX_DAMAGE)
      {
        // 跡地爆破のペナルティ
        sub_score (CITY_PENALTY * (MAX_CITIES - stage.cities));
      }

      // 振動発生
      if (!cities[i].shock.duration)
      {
        cities[i].shock.duration = SHOCK_DURATION;
        cities[i].shock.interval = SHOCK_INTERVAL;
      }
    }

    // 振動
    if (cities[i].shock.duration)
    {
      cities[i].shock.duration--;

      if (! --cities[i].shock.interval)
      {
        cities[i].shock.direc *= -1;
        cities[i].shock.interval = SHOCK_INTERVAL;
      }
      
      int r = cities[i].shock.direc * cities[i].shock.range;
      move_sprite (cities[i].sprite.chr, cities[i].sprite.coord.x + r, cities[i].sprite.coord.y);
    }
    else
      move_sprite (cities[i].sprite.chr, cities[i].sprite.coord.x, cities[i].sprite.coord.y);
  }
}



/**********************************************//**
 * @brief ステージ管理
 ***********************************************/
static void
manage_stage_level ()
{
  // UFO
  if (!--enemy.ufo_interval)
  {
    enemy.ufo_interval = enemy.ufo_interval_rel;

    if (enemy.ufo_prob && !RND(0, enemy.ufo_prob) && !ufo.is_appeared)
      init_ufo ();
  }

  // ITEM
  drop_items ();

  // ENEMY MISSILE
  int max = RND(1, enemy.max_launch);
  if (!--enemy.interval)
  {
    enemy.interval = enemy.interval_rel;
    for (int i = 0; i < max; i++)
    {
      if (enemy.max_missiles > 0)
      {
        fire_enemy_missile (RND(0, SCREEN_WIDTH-1), MISSILE_START_Y, true);
        enemy.max_missiles--;
      }
    }
  }

  // 次のレベル
  if (!enemy.max_missiles)
  {
    // 次のレベル
    if (stage.lv < MAX_LEVEL)
    {
      stage.lv++;
      reset_message (&lv_mes);
      lv_mes.is_start = true;

      //PlaySound(SOUND_LEVELUP);
    }

    init_missile ();
  }

  // シーン切り替え
  if (game_state.intermission > 0)
  {
    if (!--game_state.intermission)
      game_state.scene = game_state.next_scene;

    if (game_state.scene == GAME_OVER)
    {
      StopMusic();
      PlaySound(SOUND_OVER);
    }
  }
}



/**********************************************//**
 * @brief アイテム移動
 ***********************************************/
static void
move_items ()
{
  if (supply.is_used)
  {
    if (is_hit_to_bombs (supply.sprite.coord.x, supply.sprite.coord.y, SUPPLY_HIT_WIDTH, SUPPLY_HIT_HEIGHT, BOMB_ANIME_MAX_FRAME))
    {
      am.max_missiles += SUPPLY_CHARGE_MISSILE;
      supply.is_used = false;
      erase_sprite (supply.sprite.chr);

      PlaySound(SOUND_ITEM);

      return;
    }

    if (!--supply.speed)
    {
      supply.speed = supply.speed_rel;

      supply.sprite.coord.y++;

      if (supply.sprite.coord.y > ITEM_LIMIT_Y)
      {
        supply.is_used = false;
        erase_sprite (supply.sprite.chr);
        return;
      }
    }

    // アニメ
    if (--supply.anime.interval == 0)
    {
      supply.anime.interval = supply.anime.interval_rel;
      supply.anime.frame = (supply.anime.frame + 1) & supply.anime.max_frame;
      // タイル変更
      set_sprite_tile (SPRITE_ITEM1, TILE_ITEM1 + supply.anime.frame * TILE_SIZE_16);
    }

    move_sprite (supply.sprite.chr, supply.sprite.coord.x, supply.sprite.coord.y);
  }
}


/**********************************************//**
 * @brief 爆弾移動
 ***********************************************/
static void
move_superbomb ()
{
  if (superbomb.is_used)
  {
    if (is_hit_to_bombs (superbomb.sprite.coord.x, superbomb.sprite.coord.y, SUPERBOMB_HIT_WIDTH, SUPERBOMB_HIT_HEIGHT, BOMB_ANIME_MAX_FRAME))
    {
      flash ();
      explosion();

      superbomb.is_used = false;
      erase_sprite (superbomb.sprite.chr);
      return;
    }

    if (!--superbomb.speed)
    {
      superbomb.speed = superbomb.speed_rel;

      superbomb.sprite.coord.y++;

      if (superbomb.sprite.coord.y > ITEM_LIMIT_Y)
      {
        superbomb.is_used = false;
        erase_sprite (superbomb.sprite.chr);
        return;
      }
    }

    // アニメ
    if (--superbomb.anime.interval == 0)
    {
      superbomb.anime.interval = superbomb.anime.interval_rel;
      superbomb.anime.frame = (superbomb.anime.frame + 1) & superbomb.anime.max_frame;
      // タイル変更
      set_sprite_tile (SPRITE_ITEM2, TILE_SUPERBOMB1 + superbomb.anime.frame * TILE_SIZE_16);
    }

    move_sprite (superbomb.sprite.chr, superbomb.sprite.coord.x, superbomb.sprite.coord.y);
  }
}



/**********************************************//**
 * @brief アイテム落下
 ***********************************************/
static void
drop_items ()
{
  // 補給アイテム
  if (am.max_missiles <= SUPPLY_DROP_LIMIT && am.max_missiles >= 1 && !supply.is_used && !--supply.drop_interval)
    drop_supply ();

  // 広範囲爆弾
  if (enemy.index >= SUPERBOMB_ENEMY_UPPERLIMIT && !superbomb.is_used && !--superbomb.drop_interval)
    drop_superbomb ();

}



/**********************************************//**
 * @brief 補給投下
 ***********************************************/
static void
drop_supply ()
{
  supply.is_used = true;

  supply.drop_interval = ITEM_DROP_INTERVAL;

  supply.speed = supply.speed_rel = ITEM_SPEED;

  supply.sprite.chr = SPRITE_ITEM1;
  supply.sprite.coord.x = RND(0, SCREEN_WIDTH-SUPPLY_WIDTH);
  supply.sprite.coord.y = MISSILE_START_Y;

  supply.anime.frame = 0;
  supply.anime.interval =
  supply.anime.interval_rel = ITEM_ANIME_INTERVAL;

  supply.anime.max_frame = ITEM_ANIME_MAX_FRAME;
}



/**********************************************//**
 * @brief 爆弾投下
 ***********************************************/
static void
drop_superbomb ()
{
  superbomb.is_used = true;

  superbomb.drop_interval = SUPERBOMB_DROP_INTERVAL;

  superbomb.speed = superbomb.speed_rel = SUPERBOMB_SPEED;

  superbomb.sprite.chr = SPRITE_ITEM2;
  superbomb.sprite.coord.x = RND(0, SCREEN_WIDTH-SUPERBOMB_WIDTH);
  superbomb.sprite.coord.y = MISSILE_START_Y;

  superbomb.anime.frame = 0;
  superbomb.anime.interval =
  superbomb.anime.interval_rel = ITEM_ANIME_INTERVAL;

  superbomb.anime.max_frame = ITEM_ANIME_MAX_FRAME;
}




/**********************************************//**
 * @brief UFO 初期化
 ***********************************************/
static void
init_ufo ()
{
  ufo.is_appeared = true;

  int x = RND(0, SCREEN_WIDTH - 16);
  ufo.direc_x = x < SCREEN_WIDTH / 2 - 8 ? 2 : -2;

  ufo.sprite.chr = SPRITE_UFO;
  ufo.sprite.coord.x = x;
  ufo.sprite.coord.y = MISSILE_START_Y;

  ufo.max_step = 5 + RND(0, 10);

  ufo.stop_time = 30;
}



/**********************************************//**
 * @brief UFOミサイル発射
 * @param sx X座標
 * @param sy Y座標
 ***********************************************/
static void
fire_ufo_missile (int sx, int sy)
{
  if (enemy.index < MAX_ENEMY_MISSILES)
  {
    // 発射座標
    enemy.buf[enemy.index].start.x = sx;
    enemy.buf[enemy.index].start.y = sy;

    // 目標座標
    int ex = AMB_X;
    enemy.buf[enemy.index].end.x = ex;
    enemy.buf[enemy.index].end.y = AMB_Y;

    // XY方向
    enemy.buf[enemy.index].direc.x = sx < ex ? 1 : -1;
    enemy.buf[enemy.index].direc.y = 1;

    // XY傾き
    int dx = abs (ex - sx);
    int dy = MISSILE_END_Y - sy;
    enemy.buf[enemy.index].delta.x = dx;
    enemy.buf[enemy.index].delta.y = dy;

    // ステップ
    enemy.buf[enemy.index].step = 0;
    enemy.buf[enemy.index].max_step = dx >= dy ? dx + 1 : dy + 1;

    // スピード
    enemy.buf[enemy.index].speed =
    enemy.buf[enemy.index].speed_rel = UFO_MISSILE_SPEED;

    enemy.buf[enemy.index].is_multiple = false;

    // 弾頭スプライト
    enemy.buf[enemy.index].sprite.chr = SPRITE_WARHEAD;
    enemy.buf[enemy.index].sprite.tile = TILE_WARHEAD_UFO1;
    enemy.buf[enemy.index].sprite.coord.x = sx + WARHEAD_ADJUST_X;
    enemy.buf[enemy.index].sprite.coord.y = sy + WARHEAD_ADJUST_X;

    // 弾頭アニメ
    enemy.buf[enemy.index].anime.frame = 0;
    enemy.buf[enemy.index].anime.interval =
    enemy.buf[enemy.index].anime.interval_rel = WARHEAD_ANIME_INTERVAL / 2;
    enemy.buf[enemy.index].anime.max_frame = WARHEAD_ANIME_MAX_FRAME;

    enemy.index++;
  }
}



/**********************************************//**
 * @brief 敵ミサイル発射
 * @param sx X座標
 * @param sy Y座標
 * @param is_multiple 多弾頭か
 ***********************************************/
static void
fire_enemy_missile (int sx, int sy, bool is_multiple)
{
  if (enemy.index < MAX_ENEMY_MISSILES)
  {
    // 発射座標
    enemy.buf[enemy.index].start.x = sx;
    enemy.buf[enemy.index].start.y = sy;

    // 目標座標
    int city = RND(0, (MAX_CITIES-1));
    int ex = RND(city_coord[city], (city_coord[city]+CITY_W));
    enemy.buf[enemy.index].end.x = ex;
    enemy.buf[enemy.index].end.y = MISSILE_END_Y;

    // XY方向
    enemy.buf[enemy.index].direc.x = sx < ex ? 1 : -1;
    enemy.buf[enemy.index].direc.y = 1;

    // XY傾き
    int dx = abs (ex - sx);
    int dy = MISSILE_END_Y - sy;
    enemy.buf[enemy.index].delta.x = dx;
    enemy.buf[enemy.index].delta.y = dy;

    // ステップ
    enemy.buf[enemy.index].step = 0;
    enemy.buf[enemy.index].max_step = dx >= dy ? dx + 1 : dy + 1;

    // スピード
    enemy.buf[enemy.index].speed = enemy.buf[enemy.index].speed_rel = RND(enemy.speed, enemy.speed * 2);

    // 多弾頭
    if (is_multiple)
    {
      // スピード
      enemy.buf[enemy.index].speed = enemy.buf[enemy.index].speed_rel = RND(enemy.speed, enemy.speed * 2);
      enemy.buf[enemy.index].is_multiple = !RND(0, MULTIPLE_PROB) ? true : false;
    }
    else
    {
      // 分裂した
      enemy.buf[enemy.index].speed_rel /= 2;
      enemy.buf[enemy.index].speed = enemy.buf[enemy.index].speed_rel;
      enemy.buf[enemy.index].is_multiple = is_multiple;
    }
    enemy.buf[enemy.index].multiple_step = MULTIPLE_START + RND(0, MULTIPLE_START_MARGIN);

    // 弾頭スプライト
    enemy.buf[enemy.index].sprite.chr = SPRITE_WARHEAD;
    enemy.buf[enemy.index].sprite.tile = TILE_WARHEAD1;
    enemy.buf[enemy.index].sprite.coord.x = sx + WARHEAD_ADJUST_X;
    enemy.buf[enemy.index].sprite.coord.y = sy + WARHEAD_ADJUST_Y;

    // 弾頭アニメ
    enemy.buf[enemy.index].anime.frame = 0;
    enemy.buf[enemy.index].anime.interval =
    enemy.buf[enemy.index].anime.interval_rel = WARHEAD_ANIME_INTERVAL;
    enemy.buf[enemy.index].anime.max_frame = WARHEAD_ANIME_MAX_FRAME;

    enemy.index++;
  }
}



/**********************************************//**
 * @brief 敵ミサイル移動
 ***********************************************/
static void
move_enemy_missile ()
{
  int cur, end;
  OBJATTR *sp = OAM + SPRITE_WARHEAD;

  if (!enemy.index)
    return;

  // 後ろからバッファを操作
  cur = end = enemy.index - 1;
  for (int i = enemy.index; i > 0; i--, cur--)
  {
    // ミサイルの弾道
    draw_line (&enemy.buf[cur]);

    // ミサイルの弾頭
    disp_warhead (cur, &enemy.buf[cur]);

    if (is_attacked_target (&enemy.buf[cur]) || is_hit_to_bombs (enemy.buf[cur].sprite.coord.x, enemy.buf[cur].sprite.coord.y, WARHEAD_HIT_WIDTH, WARHEAD_HIT_HEIGHT, BOMB_ANIME_MAX_FRAME))
    {
      // スコア
      if (!is_attacked_target (&enemy.buf[cur]))
        add_enemy_score (enemy.buf[cur].sprite.coord.y);

      // 爆風発生
      set_bomb (enemy.buf[cur].sprite.coord.x, enemy.buf[cur].sprite.coord.y);

      CpuSet (&enemy.buf[end], &enemy.buf[cur], (COPY32 | (sizeof(MissileType) / 4)));
      CpuSet (sp + end, sp + cur, (COPY32 | (sizeof(OBJATTR) / 4)));

      erase_sprite (SPRITE_WARHEAD + end);

      enemy.index--;
      end--;
    }
  }

  // 弾頭分離
  for (int i = enemy.index - 1; i >= 0; i--)
  {
    if (enemy.buf[i].is_multiple && enemy.buf[i].step == enemy.buf[i].multiple_step&& enemy.buf[i].speed < MAX_MULTIPLE)
      fire_enemy_missile (enemy.buf[i].sprite.coord.x - WARHEAD_ADJUST_X, enemy.buf[i].sprite.coord.y - WARHEAD_ADJUST_Y, false);
  }
}



/**********************************************//**
 * @brief 広範囲爆弾の大爆発
 ***********************************************/
static void
explosion ()
{
  int cur, end;
  OBJATTR *sp = OAM + SPRITE_WARHEAD;

  if (!enemy.index)
    return;

  // 爆破するミサイル数
  int max = enemy.index - SUPERBOMB_POWER;
  if (max < 0)
    max = 0;
  
  // 後ろからバッファを操作
  cur = end = enemy.index - 1;
  for (int i = enemy.index; i > max; i--, cur--)
  {
    // 爆風発生
    set_bomb (enemy.buf[cur].sprite.coord.x, enemy.buf[cur].sprite.coord.y);

    CpuSet (&enemy.buf[end], &enemy.buf[cur], (COPY32 | (sizeof(MissileType) / 4)));
    CpuSet (sp + end, sp + cur, (COPY32 | (sizeof(OBJATTR) / 4)));

    erase_sprite (SPRITE_WARHEAD + end);

    enemy.index--;
    end--;
  }
}



/**********************************************//**
 * @brief AMBミサイル移動
 ***********************************************/
static void
move_amb_missile ()
{
  int cur, end;
  OBJATTR *sp_amb = OAM + SPRITE_WARHEAD_AMB;
  OBJATTR *sp_marker = OAM + SPRITE_MARKER;

  if (!am.index)
    return;

  // 後ろからバッファを操作
  cur = end = am.index - 1;
  for (int i = am.index; i > 0; i--, cur--)
  {
    // ミサイルの弾道
    draw_line (&am.buf[cur]);

    // ミサイルの弾頭
    disp_warhead (cur, &am.buf[cur]);

    if (is_attacked_target (&am.buf[cur]))
    {
      // 爆風発生
      set_bomb (am.buf[cur].sprite.coord.x, am.buf[cur].sprite.coord.y);

      // amb
      CpuSet (&am.buf[end], &am.buf[cur], (COPY32 | (sizeof(MissileType) / 4)));
      CpuSet (sp_amb + end, sp_amb + cur, (COPY32 | (sizeof(OBJATTR) / 4)));

      // marker
      CpuSet (&am.marker_buf[end], &am.marker_buf[cur], (COPY32 | (sizeof(MarkerType) / 4)));
      CpuSet (sp_marker + end, sp_marker + cur, (COPY32 | (sizeof(OBJATTR) / 4)));

      erase_sprite (SPRITE_WARHEAD_AMB + end);
      erase_sprite (SPRITE_MARKER + end);

      am.index--;
      am.marker_index--;
      end--;
    }
  }
}



/**********************************************//**
 * @brief ミサイルの軌跡描画
 * @param *m ミサイルポインタ
 ***********************************************/
static void
draw_line (MissileType *m)
{
  int x = m->start.x;
  int y = m->start.y;

  if (!--m->speed)
  {
    m->speed = m->speed_rel;
    if (m->step < m->max_step)
      m->step++;
  }

  // 垂直 水平は無いので省略
  if (m->delta.x == 0)
  {
    for (int i = 0; i < m->step; i++)
    {
      m->sprite.coord.x = x + WARHEAD_ADJUST_X;
      m->sprite.coord.y = y + WARHEAD_ADJUST_Y;
      pset8_frame (x, y, PAL_ENEMY_MISSILE);

      y += m->direc.y;
    }
  }

  // 傾きが1以下 X基準
  else if (m->delta.x >= m->delta.y)
  {
    int e = 2 * m->delta.y - m->delta.x;

    for (int i = 0; i < m->step; i++)
    {
      m->sprite.coord.x = x + WARHEAD_ADJUST_X;
      m->sprite.coord.y = y + WARHEAD_ADJUST_Y;
      pset8_frame (x, y, PAL_ENEMY_MISSILE);

      if (e >= 0)
      {
        e -= 2 * m->delta.x;
        y += m->direc.y;
      }
      e += 2 * m->delta.y;
      x += m->direc.x;
    }
  }

  // 傾きが1より大きい Y基準
  else
  {
    int e = 2 * m->delta.x - m->delta.y;

    for (int i = 0; i < m->step; i++)
    {
      m->sprite.coord.x = x + WARHEAD_ADJUST_X;
      m->sprite.coord.y = y + WARHEAD_ADJUST_Y;
      pset8_frame (x, y, PAL_ENEMY_MISSILE);

      if (e >= 0)
      {
        e -= 2 * m->delta.y;
        x += m->direc.x;
      }
      e += 2 * m->delta.x;
      y += m->direc.y;
    }
  }
}



/**********************************************//**
 * @brief ミサイル衝突判定
 * @param *m ミサイルポインタ
 ***********************************************/
static bool
is_attacked_target (MissileType *m)
{
  if (m->step == m->max_step)
  {
    return true;
  }

  return false;
}



/**********************************************//**
 * @brief 爆風との衝突判定
 * @param x X座標
 * @param y Y座標
 * @param width X幅
 * @param height Y幅
 * @param timing 当たり判定のタイミング（フレーム）
 * @return
 ***********************************************/
static bool
is_hit_to_bombs (int x, int y, int width, int height, int timing)
{
  for (int i = 0; i < bomb.index; i++)
  {
    // 当たり判定　m - e + a < a + b + 1
    if (x - (u32) bomb.buf[i].sprite.coord.x + (u32) width < (u32) (width + BOMB_HIT_WIDTH + 1)
        && y - (u32) bomb.buf[i].sprite.coord.y + (u32) height < (u32) (height + BOMB_HIT_HEIGHT + 1)
        && bomb.buf[i].anime.frame == timing
        && bomb.buf[i].anime.interval == 1)
    {
      return true;
    }
  }
  return false;
}



/**********************************************//**
 * @brief 爆風表示
 ***********************************************/
static void
disp_bomb ()
{
  int cur, end;
  OBJATTR *sp = OAM + SPRITE_BOMB;

  if (!bomb.index)
    return;

  // 後ろからバッファを操作
  cur = end = bomb.index - 1;
  for (int i = bomb.index; i > 0; i--, cur--)
  {
    move_sprite (bomb.buf[cur].sprite.chr + cur, bomb.buf[cur].sprite.coord.x, bomb.buf[cur].sprite.coord.y);

    // アニメ
    if (!--bomb.buf[cur].anime.interval)
    {
      bomb.buf[cur].anime.interval = bomb.buf[cur].anime.interval_rel;
      bomb.buf[cur].anime.frame = (bomb.buf[cur].anime.frame + 1) & bomb.buf[cur].anime.max_frame;
      // タイル変更
      set_sprite_tile (bomb.buf[cur].sprite.chr + cur, TILE_BOMB1 + bomb.buf[cur].anime.frame * TILE_SIZE_16);

      if (!bomb.buf[cur].anime.frame)
      {
        CpuSet (&bomb.buf[end], &bomb.buf[cur], (COPY32 | (sizeof(BombType) / 4)));
        CpuSet (sp + end, sp + cur, (COPY32 | sizeof(OBJATTR) / 4));

        erase_sprite (SPRITE_BOMB + end);
        bomb.index--;
        end--;
      }
    }
  }
}



/**********************************************//**
 * @brief 爆風発生
 * @param x X座標
 * @param y Y座標
 ***********************************************/
static void
set_bomb (int x, int y)
{
  if (bomb.index < MAX_BOMBS)
  {
    // スプライト
    bomb.buf[bomb.index].sprite.chr = SPRITE_BOMB;
    bomb.buf[bomb.index].sprite.coord.x = x + BOMB_ADJUST_X;
    bomb.buf[bomb.index].sprite.coord.y = y + BOMB_ADJUST_Y;

    // アニメ
    bomb.buf[bomb.index].anime.frame = 0;
    bomb.buf[bomb.index].anime.interval =
    bomb.buf[bomb.index].anime.interval_rel = BOMB_ANIME_INTERVAL;
    bomb.buf[bomb.index].anime.max_frame = BOMB_ANIME_MAX_FRAME;

    bomb.index++;

    PlaySound(SOUND_BOMB);
  }
}



/**********************************************//**
 * @brief ミサイルの弾頭アニメーション
 * @param sprite_no スプライト番号
 * @param *m ミサイルポインタ
 ***********************************************/
static void
disp_warhead (int sprite_no, MissileType *m)
{
  // アニメ
  if (--m->anime.interval == 0)
  {
    m->anime.interval = m->anime.interval_rel;
    m->anime.frame = (m->anime.frame + 1) & m->anime.max_frame;
    // タイル変更
    set_sprite_tile (m->sprite.chr + sprite_no, m->sprite.tile + m->anime.frame * TILE_SIZE_16);
  }

  move_sprite (m->sprite.chr + sprite_no, m->sprite.coord.x, m->sprite.coord.y);
}



/**********************************************//**
 * @brief UFO移動
 ***********************************************/
static void
move_ufo ()
{
  if (!ufo.is_appeared)
    return;

  if (ufo.max_step)
  {
    // 降下
    ufo.sprite.coord.x += ufo.direc_x;
    ufo.sprite.coord.y += 2;
    ufo.max_step--;
  }
  else if (!ufo.stop_time)
  {
    // 上昇
    ufo.sprite.coord.x += ufo.direc_x;
    ufo.sprite.coord.y -= 2;

    if (ufo.sprite.coord.y < MISSILE_START_Y)
    {
      ufo.is_appeared = false;
      erase_sprite (ufo.sprite.chr);
      return;
    }
  }
  else if (!--ufo.stop_time)
  {
    // UFミサイル
    fire_ufo_missile (ufo.sprite.coord.x + UFO_MISSILE_ADJUST_X, ufo.sprite.coord.y + UFO_MISSILE_ADJUST_Y);
  }

  move_sprite (ufo.sprite.chr, ufo.sprite.coord.x, ufo.sprite.coord.y);
}



/**********************************************//**
 * @brief タイトル
 ***********************************************/
static void
disp_title ()
{
  u16 key = game_state.key;

  if (key & KEY_START)
  {
    restart ();
    stage.demo = false;
    StopMusic ();
  }
  else if ((key & KEY_R) && (key & KEY_B))
  {
    clear_hiscore ();
    init_trophy ();
    update_hiscore ();
  }
}



/**********************************************//**
 * @brief モード設定
 ***********************************************/
static void
select_mode ()
{
  if (game_state.scene != GAME_TITLE)
  {
    erase_sprite(SPRITE_MODE_ARROW);
    return;
  }


  u16 key = game_state.key;
  if (key & KEY_SELECT)
  {
    PlaySound(SOUND_LEVELUP);

    stage.mode ^= 1;
    SRAMWrite32(SRAM_MODE, stage.mode);
    hiscore = load_hiscore ();
  }

  update_hiscore ();
  move_sprite(SPRITE_MODE_ARROW, MODE_ARROW_X + MODE_ARROW_X2 * stage.mode, MODE_ARROW_Y);
}



/**********************************************//**
 * @brief デモ終了
 ***********************************************/
static void
demo_end ()
{
}



/**********************************************//**
 * @brief ゲームオーバー
 ***********************************************/
static void
disp_over ()
{
  u16 key = game_state.key;

  draw_bitmap_frame (MES_X, MES_Y, MES_W, MES_H, bmp_overBitmap);

  if (key & KEY_START)
  {
    save_hiscore(score);

    reset_frame ();
    init_sprite ();
    init_game ();
    load_title ();
    StopMusic();

    PlayMusic (MUSIC_TITLE, PLAY_LOOP_ON);
  }
}



/**********************************************//**
 * @brief ゲームクリア
 ***********************************************/
static void
disp_clear ()
{
  /*
  u16 key = game_state.key;

  draw_bitmap_frame (MES_X, MES_Y, MES_W, MES_H, bmp_completeBitmap);

  if (key & KEY_START)
  {
    // レベルアップ
    if (++stage.lv > MAX_LEVEL)
      stage.lv = MAX_LEVEL;
    // リスタート
    restart ();
    //StopMusic();
  }
***/
}



/**********************************************//**
 * @brief ゲーム初期化
 ***********************************************/
void
init_game ()
{
  // ゲームステート初期化
  game_state.scene = GAME_TITLE;
  stage.mode = 0;

  // FLASH初期化
  for(int i = 0; i < 32; i++)
  stage.flash.color[i] = FLASH_COLOR;

  // モード
  stage.mode = SRAMRead32(SRAM_MODE);
  if ((u32)stage.mode > 1 )
  {
    stage.mode = 0;// 数値がおかしかったらリセット
  }

  // パラメータ初期化
  score = 0;
  stage.lv = 1;

  // トロフィー
  init_trophy ();

  // ハイスコアのロード
  hiscore = load_hiscore ();

  // スプライト初期化
  init_sprite_setting ();

  // 都市初期化
  init_cities ();
}



/**********************************************//**
 * @brief トロフィー初期化
 ***********************************************/
void
init_trophy ()
{
  for (int i = 0; i < MAX_TROPHY; i++)
  {
    trophy_unlocked[i] = false;
  }
}


/**********************************************//**
 * @brief スプライト初期化
 ***********************************************/
static void
init_sprite_setting ()
{
  // mode arrow
  set_sprite_form (SPRITE_MODE_ARROW, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_MODE_ARROW, TILE_MODE_ARROW);


  // aim
  set_sprite_form (SPRITE_AIM, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_AIM, TILE_AIM1);

  // city
  for (int i = 0; i < MAX_CITIES; i++)
  {
    set_sprite_form (SPRITE_CITY1 + i, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile (SPRITE_CITY1 + i, TILE_CITY1);
  }

  //amb
  set_sprite_form (SPRITE_AMB, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_AMB, TILE_AMB1);

  //warhead amb
  for (int i = 0; i < MAX_AMB_MISSILES; i++)
  {
    set_sprite_form (SPRITE_WARHEAD_AMB + i, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile (SPRITE_WARHEAD_AMB + i, TILE_WARHEAD1);
  }

  //warhead
  for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
  {
    set_sprite_form (SPRITE_WARHEAD + i, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile (SPRITE_WARHEAD + i, TILE_WARHEAD1);
  }

  //marker
  for (int i = 0; i < MAX_AMB_MISSILES; i++)
  {
    set_sprite_form (SPRITE_MARKER + i, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile (SPRITE_MARKER + i, TILE_MARKER1);
  }

  //bomb
  for (int i = 0; i < MAX_BOMBS; i++)
  {
    set_sprite_form (SPRITE_BOMB + i, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
    set_sprite_tile (SPRITE_BOMB + i, TILE_BOMB1);
  }

  // ufo
  set_sprite_form (SPRITE_UFO, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_UFO, TILE_UFO);

  // supply
  set_sprite_form (SPRITE_ITEM1, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_ITEM1, TILE_ITEM1);

  // superbomb
  set_sprite_form (SPRITE_ITEM2, OBJ_SIZE(1), OBJ_SQUARE, OBJ_256_COLOR);
  set_sprite_tile (SPRITE_ITEM2, TILE_SUPERBOMB1);
}



/**********************************************//**
 * @brief リスタート
 ***********************************************/
static void
restart ()
{
  game_state.scene = GAME_READY;

  // ランダマイズ
  srand (REG_TM0CNT + seed);
  // seedをSRAMに保存
  SRAMWrite32 (SRAM_SEED, seed);

  // ステージ初期化
  init_stage ();
  // score
  update_score ();

  // メッセージ初期化
  reset_message (&mes);
  reset_message (&lv_mes);

  reset_message_fast (&trophy_mes);



  // 次のシーンまで
  game_state.intermission = 0;
}



/**********************************************//**
 * @brief ステージ初期化
 ***********************************************/
static void
init_stage ()
{
  // バッファのインデックス
  enemy.index =
  bomb.index =
  am.marker_index =
  am.index = 0;

  // aim初期化
  init_aim ();

  // AMB初期化
  init_amb ();

  // ミサイル初期化
  init_missile ();

  // アイテム初期化
  init_items ();

  // 初期ミサイル配置
  fire_enemy_missile (RND(0, SCREEN_WIDTH-1), MISSILE_START_Y, true);
  fire_enemy_missile (RND(0, SCREEN_WIDTH-1), MISSILE_START_Y, true);
}



/**********************************************//**
 * @brief AIM 初期化
 ***********************************************/
static void
init_aim ()
{
  // 加速度
  aim.acc.x = aim.acc.y = 0;

  // 発射アニメ
  aim.fire_interval = 0;

  // スプライト
  aim.sprite.chr = SPRITE_AIM;
  aim.sprite.coord.x = AIM_X << FIX;
  aim.sprite.coord.y = AIM_Y << FIX;

  // 弾数警告点滅
  aim.sprite.show = true;
  aim.flash.interval = AIM_CAUTION_INTERVAL;
}



/**********************************************//**
 * @brief Anti-Missile-Battery 初期化
 ***********************************************/
static void
init_amb ()
{
  am.index = 0;
  am.remaining = 0;

  am.max_missiles = INIT_AMB_MISSILES;

  //amb.anime.frame = 0;
  //amb.anime.interval =
  //amb.anime.interval_rel = AMB_ANIME_INTERVAL;
  //amb.anime.max_frame = AMB_MAX_FRAME;

  amb.sprite.chr = SPRITE_AMB;
  amb.sprite.coord.x = AMB_X;
  amb.sprite.coord.y = AMB_Y;
}



/**********************************************//**
 * @brief 都市初期化
 ***********************************************/
static void
init_cities ()
{
  for (int i = 0; i < MAX_CITIES; i++)
  {
    cities[i].damage = 0;

    cities[i].sprite.chr = SPRITE_CITY1 + i;
    cities[i].sprite.coord.x = city_coord[i];
    cities[i].sprite.coord.y = CITY_Y;
    
    // 振動
    cities[i].shock.range = SHOCK_RANGE;
    cities[i].shock.direc = 1;
    cities[i].shock.interval = SHOCK_INTERVAL;
    cities[i].shock.duration = 0;
  }

  stage.cities = MAX_CITIES;
}



/**********************************************//**
 * @brief ミサイル初期化
 ***********************************************/
static void
init_missile ()
{
  // レベルごとのパラメータ
  // max_missiles(amb), interval, max_launch, max_missiles(enemy), speed, ufo
  static int level[][MAX_LEVEL][LEVEL_PARAM] = {
      // NOMAL
      {
        { 10, 240, 2, 20, 7, 0 },
        { 10, 220, 2, 20, 7, 0 },
        { 10, 200, 2, 20, 7, 32 },
        { 10, 180, 3, 20, 6, 32 },
        { 10, 160, 3, 20, 6, 32 },
        { 10, 140, 3, 20, 6, 16 },
        { 10, 120, 4, 20, 5, 16 },
        { 10, 100, 4, 20, 5, 16 },
        { 10, 80,  4, 20, 5, 8 },
        { 10, 60,  5, 20, 4, 8 },
      },

      // EXTREME
      {
        { 10, 220, 2, 20, 6, 32 },
        { 10, 200, 2, 20, 6, 32 },
        { 10, 180, 2, 20, 6, 32 },
        { 10, 160, 3, 20, 5, 16 },
        { 10, 140, 3, 20, 5, 16 },
        { 10, 120, 3, 20, 5, 16 },
        { 10, 100, 4, 20, 4, 8 },
        { 10, 80, 4, 20, 4, 8 },
        { 10, 60,  4, 20, 4, 8 },
        { 10, 40,  5, 20, 3, 4 },
      },
  };

  int lv = stage.lv - 1;
  if (lv >= MAX_LEVEL)
    lv = MAX_LEVEL - 1;


  // enemy
  enemy.interval = INIT_ENEMY_FIRE_INTERVAL;
  enemy.interval_rel = level[stage.mode][lv][1];
  enemy.max_launch = level[stage.mode][lv][2];
  enemy.max_missiles = level[stage.mode][lv][3];
  enemy.speed = level[stage.mode][lv][4];

  // ufo
  enemy.ufo_interval = enemy.ufo_interval_rel = UFO_APPEAR_INTERVAL;
  enemy.ufo_prob = level[stage.mode][lv][5];
  ufo.is_appeared = false;
}



/**********************************************//**
 * @brief アイテム初期化
 ***********************************************/
static void
init_items ()
{
  // 補給
  supply.drop_interval = 1;
  supply.is_used = false;
  erase_sprite (SPRITE_ITEM1);

  // 爆弾
  superbomb.drop_interval = 1;
  superbomb.is_used = false;
  erase_sprite (SPRITE_ITEM2);

}



/**********************************************//**
 * @brief ポーズ
 ***********************************************/
static void
pause ()
{
  u16 key = game_state.key;

  if (key & KEY_SELECT)
  {
    game_state.scene ^= GAME_PAUSE;
    if (game_state.scene & GAME_PAUSE)
    {
      StopMusic ();
      PlaySound(SOUND_ITEM);
    }
    else
    {
      PlayMusic(MUSIC_STAGE + stage_bgm, true);
    }
  }
}



/**********************************************//**
 * @brief 敵ミサイル撃墜のスコア加算
 * @param y スコア加算判定のための高度（Y座標）
 ***********************************************/
static void
add_enemy_score (int y)
{
  static int scores[] = { 0, 100, 200, 400, 800 };

  add_score (scores[(int) ((SCREEN_HEIGHT - y) / SCORE_HEIGHT_RANGE)] * stage.cities);
}



/**********************************************//**
 * @brief スコア加算
 * @param num 加点
 ***********************************************/
static void
add_score (int num)
{
  score += num;
  update_score ();
}



/**********************************************//**
 * @brief スコア減点
 * @param num 減点
 ***********************************************/
static void
sub_score (int num)
{
  score -= num;
  if (score < 0)
    score = 0;
  update_score ();
}



/**********************************************//**
 * @brief LV表示
 ***********************************************/
static void
update_lv ()
{
  int i;
  int pos = LV_DIGIT * NUM_W - NUM_W;
  u32 lv = stage.lv;

  for (i = 0; i < LV_DIGIT; i++)
  {
    disp_num ( LV_X + pos, LV_Y, lv % 10);
    lv /= 10;
    pos -= (NUM_W);
  }
}



/**********************************************//**
 * @brief スコア表示
 ***********************************************/
static void
update_score ()
{
  int i;
  int pos = SCORE_DIGIT * NUM_W - NUM_W;
  int sc = score;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num (SCORE_X + pos, SCORE_Y, sc % 10);
    sc /= 10;
    pos -= (NUM_W);
  }
}



/**********************************************//**
 * @brief ハイスコア表示
 ***********************************************/
void
update_hiscore ()
{
  int i;
  int pos = SCORE_DIGIT * NUM_W - NUM_W;
  int sc = hiscore;

  for (i = 0; i < SCORE_DIGIT; i++)
  {
    disp_num_title (HISCORE_X + pos, HISCORE_Y, sc % 10);
    sc /= 10;
    pos -= (NUM_W);
  }

  disp_trophy ();
}



/**********************************************//**
 * @brief トロフィーマーク表示
 ***********************************************/
void
disp_trophy ()
{
  int x = TROPHY_X;
  // トロフィー表示
  for (int i = 0; i < MAX_TROPHY; i++)
  {
    if (trophy_unlocked[i])
      draw_bitmap8 (x, TROPHY_Y, TROPHY_W, TROPHY_H, bmp_trophyBitmap);
    else
      draw_bitmap8 (x, TROPHY_Y, TROPHY_W, TROPHY_H, bmp_notrophyBitmap);

    x += TROPHY_W;
  }
}



/**********************************************//**
 * @brief 残りミサイル表示
 ***********************************************/
static void
update_remining_missiles ()
{
  // 敵
  int pos = REMINING_DIGIT * NUM_W - NUM_W;
  u32 remining = enemy.max_missiles;

  for (int i = 0; i < REMINING_DIGIT; i++)
  {
    disp_num (REMINING_ENEMY_X + pos, REMINING_ENEMY_Y, remining % 10);
    remining /= 10;
    pos -= (NUM_W);
  }

  //AMB
  pos = REMINING_DIGIT * NUM_W - NUM_W;
  remining = am.max_missiles;

  for (int i = 0; i < REMINING_DIGIT; i++)
  {
    disp_num (REMINING_AMB_X + pos, REMINING_AMB_Y, remining % 10);
    remining /= 10;
    pos -= (NUM_W);
  }
}



/**********************************************//**
 * @brief ワーニングメッセージ表示
 ***********************************************/
static void
disp_warning ()
{
  if (!--mes.wait)
  {
    mes.wait = mes.wait_rel;
    mes.chr ^= 1;

    if (!--mes.count)
    {
      game_state.scene = GAME_MAIN;
      reset_message (&mes);
      stage_bgm = 0;
      PlayMusic (MUSIC_STAGE, PLAY_LOOP_ON);
    }
  }

  if (mes.chr)
    draw_bitmap_frame (MES_X, MES_Y, MES_W, MES_H, bmp_warningBitmap);
}



/**********************************************//**
 * @brief ポーズメッセージ表示
 ***********************************************/
static void
disp_pause ()
{
  draw_bitmap_frame (MES_X, MES_Y, MES_W, MES_H, bmp_pauseBitmap);
}



/**********************************************//**
 * @brief レベルアップの点滅
 ***********************************************/
static void
level_up ()
{
  if (!lv_mes.is_start)
    return;

  // 点滅
  if (!--lv_mes.wait)
  {
    lv_mes.wait = lv_mes.wait_rel;
    lv_mes.chr ^= 1;

    if (!--lv_mes.count)
    {
      reset_message (&lv_mes);

      // BGM切り替え
      StopMusic();
      stage_bgm = (stage_bgm + 1) & MAX_STAGE_BGM;
      PlayMusic(MUSIC_STAGE + stage_bgm, true);
    }
  }

  if (lv_mes.chr)
    draw_bitmap_frame (LV_MES_X, LV_MES_Y, LV_MES_W, LV_MES_H, bmp_lv_blackBitmap);
}



/**********************************************//**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message (BlinkMessageType *m)
{
  m->is_start = false;
  m->count = MES_COUNT;
  m->chr = 1;
  m->wait = m->wait_rel = MES_WAIT;
}



/**********************************************//**
 * @brief 点滅メッセージ用パラメータのリセット
 * @param *m メッセージパラメータポインタ
 ***********************************************/
static void
reset_message_fast (BlinkMessageType *m)
{
  m->is_start = false;
  m->count = MES_COUNT * 2;
  m->chr = 1;
  m->wait = m->wait_rel = MES_WAIT / 2;
}



/**********************************************//**
 * @brief BGに数字表示
 * @param x X座標
 * @param y Y座標
 * @param num 表示する数値
 ***********************************************/
static void
disp_num (int x, int y, u16 num)
{
  draw_bitmap_frame (x, y, NUM_W, NUM_H, bmp_numBitmap + 32 * num);
}



/**********************************************//**
 * @brief タイトル(FRAME1)に数字表示
 * @param x X座標
 * @param y Y座標
 * @param num 表示する数値
 ***********************************************/
static void
disp_num_title (int x, int y, u16 num)
{
  draw_bitmap8 (x, y, NUM_W, NUM_H, bmp_numBitmap + 32 * num);
}



static void
flash ()
{
  stage.flash.interval = FLASH_INTERVAL;
}



/**********************************************//**
 * @brief ステージのBG描画
 ***********************************************/
static void
draw_bg ()
{
  flip_frame ();

  if (stage.flash.interval)
  {
    stage.flash.interval--;
    fill_frame_bitmap (stage.flash.color);
  }
  else
  {
    // 通常のBG
    load_frame_bitmap (DEF_BG_BITMAP);
  }
}



/**********************************************//**
 * トロフィー（実績）解除
 ***********************************************/
static void
trophy ()
{
  /*
   * 実績２：レベル10に到達
   */
  if (stage.lv == MAX_LEVEL
      && !trophy_unlocked[1])
  {
    trophy_unlocked[1] = true;
    trophy_mes.is_start = true;
    return;
  }

  /*
   * 実績１：レベル5まで都市を守った
   */
  if (!cities[0].damage
      && !cities[1].damage
      && !cities[2].damage
      && !cities[3].damage
      && !cities[4].damage
      && !cities[5].damage
      && stage.lv == 5
      && !trophy_unlocked[0])
  {
    trophy_unlocked[0] = true;
    trophy_mes.is_start = true;
    return;
  }

  /*
   * 実績４：弾を20発以上残した
   */
  if (lv_mes.is_start && am.max_missiles >= 20 && !trophy_unlocked[3])
  {
    trophy_unlocked[3] = true;
    trophy_mes.is_start = true;
    return;
  }

  /*
   * 実績３：弾を10発以上残した
   */
  if (lv_mes.is_start && am.max_missiles >= 10 && !trophy_unlocked[2])
  {
    trophy_unlocked[2] = true;
    trophy_mes.is_start = true;
    return;
  }

  /*
   * 実績５：スコアが100万突破
   */
  if (score >= 1000000 && !trophy_unlocked[5])
  {
    trophy_unlocked[5] = true;
    trophy_mes.is_start = true;
    return;
  }

  /*
   * 実績５：スコアが50万突破
   */
  if (score >= 500000 && !trophy_unlocked[4])
  {
    trophy_unlocked[4] = true;
    trophy_mes.is_start = true;
    return;
  }
}



/**********************************************//**
 *  トロフィー獲得メッセージ
 ***********************************************/
static void
disp_trophy_mes ()
{
  if (!trophy_mes.is_start)
    return;

  // 点滅
  if (!--trophy_mes.wait)
  {
    trophy_mes.wait = trophy_mes.wait_rel;
    trophy_mes.chr ^= 1;

    if (!--trophy_mes.count)
    {
      reset_message_fast (&trophy_mes);
    }
  }

  if (trophy_mes.chr)
    draw_bitmap_frame (UNLOCK_MES_X, UNLOCK_MES_Y, UNLOCK_MES_W, UNLOCK_MES_H, bmp_unlockedBitmap);
}



/**********************************************//**
 *  タイトル ビットマップ転送
 ***********************************************/
void
load_title ()
{
  load_bg_bitmap_lz77(DEF_TITLE_BITMAP);
  update_hiscore ();
}



//// デバグ用
//THUMB code
void
vbaPrint (char *s)
{
  asm volatile("mov r0, %0;"
      "swi 0xff;"
      : // no ouput
      : "r" (s)
      : "r0");
}
