/**
 * @file game.h
 * @brief ゲーム本体のヘッダ
 * @date  2017/02/01 更新
 * @author Choe Gyun(choikyun)
 */

/***************************************************
 * Moon Defender GBA
 * ver 1.0.0
 * 2017.02.01
 * Choe Gyun (choikyun)
 *****************************************************/


#ifndef _game_h_
#define _game_h_

#include <gba.h>
#include "bg.h"

#ifdef GLOBAL_VALUE_DEFINE
#define GLOBAL
#define GLOBAL_VAL(v) = (v)
#else
#define GLOBAL extern
#define GLOBAL_VAL(v)
#endif

///////////////////////////////////////////////////////////////////// 各種定義


/***************************************************
 * 汎用的な定義
 ***************************************************/

/**
 * 固定小数点部分 16bit
 */
#define FIX (16)



/**
 * ステージ幅　最小
 */
#define STAGE_W_MIN (0)
/**
 * ステージ幅　最大
 */
#define STAGE_W_MAX (SCREEN_WIDTH-16)
/**
 * ステージ高さ　最小
 */
#define STAGE_H_MIN (8)
/**
 * ステージ高さ　最大
 */
#define STAGE_H_MAX (SCREEN_HEIGHT - 32 - 16)



/**
 * 数字　幅
 */
#define NUM_W (8)
/**
 * 数字　高さ
 */
#define NUM_H (8)



/**
 * 数字の実際の幅
 *
 * MODE4では16bit単位で書き込むため
 */
#define REAL_NUM_W (NUM_W / 2)



/**
 * キー入力　長さ
 */
#define DEF_KEY_DELAY (1)
/**
 * キー入力 リピート
 */
#define DEF_KEY_REPEAT (1)



/**
 * メッセージ表示ウェイト
 */
#define MES_WAIT (20)



/**
 * メッセージ点滅カウント
 */
#define MES_COUNT (10)


/**
 * 一般オブジェクト　幅
 */
#define OBJ_W (16)
/**
 * 一般オブジェクト　高さ
 */
#define OBJ_H (16)




/***************************************************
 * ゲーム個別
 ***************************************************/

/***************************************************
 * モード
 ***************************************************/

/**
 * ゲームモード数
 */
#define MAX_MODE (2)




/***************************************************
 * 照準 スプライト
 ***************************************************/

/**
 * AIM　幅
 */
#define AIM_W (16)
/**
 * AIM　高さ
 */
#define AIM_H (16)


/**
 * AIM X座標
 */
#define AIM_X (SCREEN_WIDTH / 2 - AIM_W / 2)
/**
 * AIM　Y座標
 */
#define AIM_Y ((SCREEN_HEIGHT-AIM_H) / 2 - AIM_H / 2)


/**
 * AIM速度
 */
#define AIM_SPEED (1)


/**
 * AIM座標補正　X座標
 */
#define AIM_ADJUST_X (7)
/**
 * AIM座標補正　Y座標
 */
#define AIM_ADJUST_Y (7)


/**
 * AIM加速度
 */
#define AIM_ACC (4096*3)
/**
 * AIM加速度　最大値
 */
#define AIM_ACC_MAX (2 << FIX)


/**
 * AIM摩擦係数
 */
#define AIM_FRIC (0.08)


/**
 * AIMアニメーション　インターバル
 */
#define AIM_ANIME_INTERVAL (3)
/**
 * AIMアニメーション　フレーム数
 */
#define AIM_ANIME_MAX_FRAME (3)


/**
 * AIM警告点滅間隔
 */
#define AIM_CAUTION_INTERVAL (3)


/**
 * AIM発射までのインターバル
 */
#define AIM_FIRE_INTERVAL (1)




/***************************************************
 * Anti-Missile-Battery スプライト
 ***************************************************/

/**
 * AMB　X座標
 */
#define AMB_X (112)
/**
 * AMB　Y座標
 */
#define AMB_Y (142)


/**
 * AMB　センター
 */
#define AMB_C (AMB_X + 8)




/***************************************************
 * レベル
 ***************************************************/

/**
 * レベル表示桁
 */
#define LV_DIGIT (2)


/**
 * レベル　X座標
 */
#define LV_X (36)
/**
 * レベル　Y座標
 */
#define LV_Y (0)


/**
 * レベル最大値
 */
#define MAX_LEVEL (10)


/**
 * レベルのパラメータ数
 */
#define LEVEL_PARAM (6)



/***************************************************
 * 都市
 ***************************************************/

/**
 * 都市数
 */
#define MAX_CITIES (6)


/**
 * 都市ダメージ
 */
#define CITY_MAX_DAMAGE (3)


/**
 * 都市　Y座標
 */
#define CITY_Y (SCREEN_HEIGHT - CITY_H - 4)


/**
 * 都市　幅
 */
#define CITY_W (16)
/**
 * 都市　高さ
 */
#define CITY_H (10)

/**
 * 都市当たり判定　幅
 */
#define HIT_CITY_W (CITY_W - 1)
/**
 * 都市当たり判定　高さ
 */
#define HIT_CITY_H (CITY_H - 1)

/**
 * 都市ペナルティスコア
 */
#define CITY_PENALTY (100)



/***************************************************
 * 都市振動
 ***************************************************/

/**
 * ショック　振幅
 */
#define SHOCK_RANGE (1)


/**
 * ショック　間隔
 */
#define SHOCK_INTERVAL (4)


/**
 * ショック　長さ
 */
#define SHOCK_DURATION (SHOCK_INTERVAL * 4)



/***************************************************
 * フラッシュ
 ***************************************************/

/**
 * フラッシュ　間隔
 */
#define FLASH_INTERVAL (4)

/**
 * フラッシュ　色
 */
#define FLASH_COLOR (15)


/***************************************************
 * ミサイル
 ***************************************************/

/**
 * ミサイル　画面に表示される最大数
 */
#define MAX_ENEMY_MISSILES (12)


/**
 * ミサイル　発射位置Y座標
 */
#define MISSILE_START_Y (8)


/**
 * ミサイル　目標位置Y座標
 */
#define MISSILE_END_Y (SCREEN_HEIGHT - 16)


/**
 * ミサイル　発射のインターバル
 */
#define INIT_ENEMY_FIRE_INTERVAL (2*60);


/**
 * AMBミサイル　最大数
 */
#define MAX_AMB_MISSILES (3)


/**
 * AMBミサイル　初期弾数
 */
#define INIT_AMB_MISSILES (30)


/**
 * AMBミサイル　速度
 */
#define AMB_MISSILE_SPEED (1)


/**
 * AMBミサイル　弾数警告になる残り数
 */
#define AMB_MISSILE_CAUTION (8)




/***************************************************
 * UFO
 ***************************************************/

/**
 * UFO　出現インターバル
 */
#define UFO_APPEAR_INTERVAL (1*60)


/**
 * UFO　ミサイル速度
 */
#define UFO_MISSILE_SPEED (2)

/**
 * UFO ミサイル　X座標補正
 */
#define UFO_MISSILE_ADJUST_X (6)
/**
 * UFO ミサイル　Y座標補正
 */
#define UFO_MISSILE_ADJUST_Y (4)



/***************************************************
 * ミサイル弾頭
 ***************************************************/

/**
 * 弾頭　X座標補正
 */
#define WARHEAD_ADJUST_X (-7)
/**
 * 弾頭　Y座標補正
 */
#define WARHEAD_ADJUST_Y (-7)


/**
 * 弾頭アニメーション　インターバル
 */
#define WARHEAD_ANIME_INTERVAL (6)
/**
 * 弾頭アニメーション　フレーム数
 */
#define WARHEAD_ANIME_MAX_FRAME (1)


/**
 * 弾頭当たり判定　幅
 */
#define WARHEAD_HIT_WIDTH (OBJ_W-1)
/**
 * 弾頭当たり判定　高さ
 */
#define WARHEAD_HIT_HEIGHT (OBJ_H-1)


/**
 * 弾頭　多弾頭の確率
 */
#define MULTIPLE_PROB (4)
/**
 * 弾頭　分裂数
 */
#define MAX_MULTIPLE (4)


/**
 * 弾頭　分裂位置　最小
 */
#define MULTIPLE_START (20)
/**
 * 弾頭　分裂位置　最大
 */
#define MULTIPLE_START_MARGIN (25)



/***************************************************
 * マーカー
 ***************************************************/

/**
 * マーカー数
 */
#define MAX_MARKERS MAX_AMB_MISSILES



/***************************************************
 * 爆風
 ***************************************************/

/**
 * 爆風　当たり判定　幅
 */
#define BOMB_HIT_WIDTH (OBJ_W-1)
/**
 * 爆風　当たり判定　高さ
 */
#define BOMB_HIT_HEIGHT (OBJ_H-1)


/**
 * 爆風　最大数
 */
#define MAX_BOMBS (32)


/**
 * 爆風　座標補正　X座標
 */
#define BOMB_ADJUST_X (0)
/**
 * 爆風　座標補正　Y座標
 */
#define BOMB_ADJUST_Y (0)


/**
 * 爆風　アニメーション　インターバル
 */
#define BOMB_ANIME_INTERVAL (5)
/**
 * 爆風　アニメーション　フレーム数
 */
#define BOMB_ANIME_MAX_FRAME (3)



/***************************************************
 * 残り弾数 敵,自機
 ***************************************************/

/**
 * 残り弾数　表示桁
 */
#define REMINING_DIGIT (3)


/**
 * 敵残り弾数　X座標
 */
#define REMINING_ENEMY_X (184)
/**
 * 敵残り弾数　Y座標
 */
#define REMINING_ENEMY_Y SCORE_Y
/**
 * AMB残り弾数　X座標
 */
#define REMINING_AMB_X (216)
/**
 * AMB残り弾数　Y座標
 */
#define REMINING_AMB_Y SCORE_Y




/***************************************************
 * アイテム：補給
 ***************************************************/

/**
 * アイテム　これ以下なら補給開始
 */
#define SUPPLY_DROP_LIMIT (10)


/**
 * アイテム　補給弾数
 */
#define SUPPLY_CHARGE_MISSILE (8);


/**
 * アイテム　幅
 */
#define SUPPLY_WIDTH (12)
/**
 * アイテム　高さ
 */
#define SUPPLY_HEIGHT (10)


/**
 * アイテム　当たり判定　幅
 */
#define SUPPLY_HIT_WIDTH (SUPPLY_WIDTH-1)
/**
 * アイテム　当たり判定　高さ
 */
#define SUPPLY_HIT_HEIGHT (SUPPLY_HEIGHT-1)




/***************************************************
 * アイテム：広範囲爆弾
 ***************************************************/

/**
 * 反応弾　敵ミサイルがこれ以上なら投下開始
 */
#define SUPERBOMB_ENEMY_UPPERLIMIT (8)


/**
 * 反応弾　スピード
 */
#define SUPERBOMB_SPEED (3)


/**
 * 反応弾　出現間隔
 */
#define SUPERBOMB_DROP_INTERVAL (400)


/**
 * 反応弾　誘爆するミサイル数
 */
#define SUPERBOMB_POWER (5)


/**
 * 反応弾　幅
 */
#define SUPERBOMB_WIDTH (8)
/**
 * 反応弾　高さ
 */
#define SUPERBOMB_HEIGHT (8)


/**
 * 反応弾　当たり判定　幅
 */
#define SUPERBOMB_HIT_WIDTH (SUPERBOMB_WIDTH-1)
/**
 * 反応弾　当たり判定　高さ
 */
#define SUPERBOMB_HIT_HEIGHT (SUPERBOMB_HEIGHT-1)




/***************************************************
 * アイテム一般
 ***************************************************/

/**
 * アイテム　スピード
 */
#define ITEM_SPEED (5)


/**
 * アイテム　アニメーション　インターバル
 */
#define ITEM_ANIME_INTERVAL (4)
/**
 * アイテム　アニメーション　フレーム数
 */
#define ITEM_ANIME_MAX_FRAME (1)


/**
 * アイテム　移動距離
 */
#define ITEM_LIMIT_Y (130)


/**
 * アイテム　出現間隔
 */
#define ITEM_DROP_INTERVAL (300)




/***************************************************
 *　情報表示
 ***************************************************/

/**
 * メッセージ表示　X座標
 */
#define MES_X (50)
/**
 * メッセージ表示　Y座標
 */
#define MES_Y (68)
/**
 * メッセージ表示　幅
 */
#define MES_W (140)
/**
 * メッセージ表示　高さ
 */
#define MES_H (24)


/**
 * レベルアップ表示　X座標
 */
#define LV_MES_X (0)
/**
 * レベルアップ表示　Y座標
 */
#define LV_MES_Y (0)
/**
 * レベルアップ表示　幅
 */
#define LV_MES_W (34)
/**
 * レベルアップ表示　高さ
 */
#define LV_MES_H (8)




/***************************************************
 *　シーン
 ***************************************************/

/**
 * シーン終了後のインターバル
 */
#define INTERMISSION (3*60)



/***************************************************
 *　モード選択
 ***************************************************/

/**
 * モード選択の矢印　X座標
 */
#define MODE_ARROW_X (28)
/**
 * モード選択の矢印　Y座標
 */
#define MODE_ARROW_Y (84)

/**
 * モード選択の矢印　移動後のX座標
 */
#define MODE_ARROW_X2 (38)



/***************************************************
 *　スコア
 ***************************************************/

/**
 * スコア加算のための高度間隔
 */
#define SCORE_HEIGHT_RANGE (32)


/**
 * スコア表示桁
 */
#define SCORE_DIGIT (8)


/**
 * スコア　X座標
 */
#define SCORE_X (88)
/**
 * スコア　Y座標
 */
#define SCORE_Y LV_Y
/**
 * ハイスコア　X座標
 */
#define HISCORE_X (70)
/**
 * ハイスコア　Y座標
 */
#define HISCORE_Y (64)




/***************************************************
 *　トロフィー
 ***************************************************/

/**
 * トロフィーの数
 */
#define MAX_TROPHY (6)

/**
 * アンロックメッセージ　X座標
 */
#define UNLOCK_MES_X (90)
/**
 * アンロックメッセージ　Y座標
 */
#define UNLOCK_MES_Y (76)

/**
 * アンロックメッセージ　インターバル
 */
#define UNLOCK_MES_INTERVAL (6)

/**
 * アンロックメッセージ　幅
 */
#define UNLOCK_MES_W (48)
/**
 * アンロックメッセージ　高さ
 */
#define UNLOCK_MES_H (7)

/**
 * トロフィーマーク　X座標
 */
#define TROPHY_X (192)
/**
 * トロフィーマーク　Y座標
 */
#define TROPHY_Y (1)
/**
 * トロフィーマーク　幅
 */
#define TROPHY_W (8)
/**
 * トロフィーマーク　高さ
 */
#define TROPHY_H (8)



/***************************************************
 *　BGM
 ***************************************************/

/**
 * ステージBGM数
 */
#define MAX_STAGE_BGM (3)




///////////////////////////////////////////////////////////////////// クラス


/**
 * 座標
 */
typedef struct
{ 
  int x;
  int y;
}ALIGN(4) PointType;



/**
 * 矩形座標
 */
typedef struct
{ 
  int x;
  int y;
  int width;
  int height;
}ALIGN(4) RectangleType;


/**
 * 振動
 */
typedef struct
{
  int range;
  int direc;
  int interval;
  int duration;
} ALIGN(4) ShockType;



/**
 * フラッシュ
 */
typedef struct
{
  u8 color[32];
  int interval;
} ALIGN(4) FlashType;



/**
 * ゲーム管理
 */
typedef struct
{ 
  int scene;
  int next_scene;
  int intermission; // 次のシーンまでの幕間
  u16 key;
  u16 keyr;
  int demo_start;
}ALIGN(4) GameStateType;



/**
 * ステージ管理
 */
typedef struct
{ 
  int lv;
  int mode;
  int cities;
  bool demo;

  FlashType flash;
  RectangleType rect;
}ALIGN(4) StageType;



/**
 * キャラクタ
 */
typedef struct
{ 
  int chr; // スプライト番号
  int tile;// タイル番号
  bool show;// 表示/非表示
  PointType coord;
}ALIGN(4) SpriteChrType;



/**
 * フレームアニメ
 */
typedef struct
{ 
  bool is_start; // 開始
  int frame;// 現在のフレーム
  int max_frame;// フレーム最大値
  int interval;// インターバル
  int interval_rel;// インターバル既定値
}ALIGN(4) AnimeType;



/**
 * AIM
 */
typedef struct
{ 
  PointType acc; // 加速度
  int fire_interval;
  FlashType flash;
  SpriteChrType sprite;
}ALIGN(4) AimType;



/**
 * AMB
 */
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) AmbType;



/**
 * CITY
 */
typedef struct
{ 
  int damage; // ダメージ
  
  ShockType shock;
  SpriteChrType sprite;
}ALIGN(4) CityType;



/**
 * ミサイル
 */
typedef struct
{ 
  PointType start; // 開始座標
  PointType end;// 終了座標
  
  PointType direc;// XY方向
  PointType delta;// XY傾き
  
  int step;// 現在位置
  int max_step;// 停止位置
  
  int speed;// スピード
  int speed_rel;// スピードデフォルト
  
  bool is_multiple;// 多弾頭か
  int multiple_step;// 分裂位置
  
  AnimeType anime;// 弾頭アニメ
  SpriteChrType sprite;// 弾頭スプライト
}ALIGN(4) MissileType;



/**
 * 爆風
 */
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) BombType;



/**
 * マーカー
 */
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) MarkerType;



/**
 * 補給アイテム
 */
typedef struct
{ 
  int type; // アイテムの種類
  bool is_used;// 使用中
  int drop_interval;// 出現のインターバル
  int speed;// スピード
  int speed_rel;// スピードデフォルト
  
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) SupplyType;



/**
 * 敵管理
 */
typedef struct
{ 
  int interval; // 発生間隔
  int interval_rel;
  int max_missiles;// レベルごとのミサイル数
  int max_launch;// 同時発射数
  int speed;// 弾頭スピード
  
  int ufo_interval;// UFO出現間隔
  int ufo_interval_rel;
  int ufo_prob;// UFO出現確率
  
  int index;// 現在のバッファ位置
  MissileType buf[MAX_ENEMY_MISSILES];
}ALIGN(4) EnemyStateType;



/**
 * AMB管理
 */
typedef struct
{ 
  int max_missiles; // レベルごとの最大数
  int max_launch;// 同時発射数
  int remaining;// 残り弾数
  int index;// 現在のバッファ位置
  MissileType buf[MAX_AMB_MISSILES];

  // マーカー
  int marker_index;// 現在のバッファ位置
  MarkerType marker_buf[MAX_AMB_MISSILES];
}ALIGN(4) AMBStateType;



/**
 * 爆風管理
 */
typedef struct
{ 
  int index; // 現在のバッファ位置
  BombType buf[MAX_BOMBS];
}ALIGN(4) BombStateType;



/**
 * UFO
 */
typedef struct
{ 
  bool is_appeared; // 出現中か
  int direc_x;// X方向
  int max_step;// 移動量
  int stop_time;// 停止時間
  
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) UFOType;



/**
 * 点滅メッセージ
 */
typedef struct
{ 
  bool is_start;
  int x;
  int y;
  int wait;
  int wait_rel;
  int count;
  int chr;
}ALIGN(4) BlinkMessageType;



/**
 * ウェイト
 */
typedef struct
{ 
  int wait;
}ALIGN(4) WaitType;



///////////////////////////////////////////////////////////////////// ENUM

/**
 * CITYダメージ
 */
enum
{
  NOMAL, LITTLE, LARGE, DESTRUCTION
};



/**
 * シーン
 */
enum
{
  GAME_MAIN = 0,
  GAME_TITLE = 1,
  GAME_PAUSE = 2,
  GAME_OVER = 4,
  GAME_READY = 8,
  GAME_OVER_PRE = 16,
  GAME_CRASH = 32,
  GAME_CLEAR = 64,
  GAME_DEMO = 128,
};



/**
 * スプライトキャラクタ
 */
enum
{
  SPRITE_AIM = 0,

  SPRITE_BOMB = 1, // +MAX_BOMBS
  
  SPRITE_AMB = 33,
  
  SPRITE_CITY1 = 34,
  SPRITE_CITY2 = 35,
  SPRITE_CITY3 = 36,
  SPRITE_CITY4 = 37,
  SPRITE_CITY5 = 38,
  SPRITE_CITY6 = 39,
  
  SPRITE_ITEM1 = 40,// 補給
  SPRITE_ITEM2 = 41,// 爆弾
  SPRITE_ITEM3 = 42,
  
  SPRITE_MODE_ARROW = 43,// モード矢印

  SPRITE_UFO = 44,
  
  SPRITE_MARKER = 45, // +MAX_MARKERS
  
  SPRITE_WARHEAD_AMB = 48, // +MAX_AMB_MISSILES
  
  SPRITE_WARHEAD = 51, // +MAX_ENEMY_MISSILES
  
} SpriteEnum;



///////////////////////////////////////////////////////////////////// タイル番号
#define TILE_SIZE_8 (2)
#define TILE_SIZE_16 (8)


#define TILE_AIM1 (512)
#define TILE_AIM2 (520)
#define TILE_AIM3 (528)
#define TILE_AIM4 (536)

#define TILE_WARHEAD1 (544)
#define TILE_WARHEAD2 (552)

#define TILE_WARHEAD_UFO1 (560)
#define TILE_WARHEAD_UFO2 (568)

#define TILE_MARKER1 (576)
#define TILE_MARKER2 (584)

#define TILE_CITY1 (592)
#define TILE_CITY2 (600)
#define TILE_CITY3 (608)
#define TILE_CITY4 (616)

#define TILE_AMB1 (624)

// TILE_AMB2
// TILE_AMB3
// TILE_AMB4

#define TILE_BOMB1 (656)

// TILE_BOMB2
// TILE_BOMB3
// TILE_BOMB4

#define TILE_UFO (688)

#define TILE_ITEM1 (696)
#define TILE_ITME1_2 (704)

#define TILE_SUPERBOMB1 (712)
#define TILE_SUPERBOMB2 (720)

#define TILE_MODE_ARROW (728)



///////////////////////////////////////////////////////////////////// SRAM

/**
 * ハイスコアSRAM保存用
 */
#define SRAM_CHECK (0)
/**
 * ハイスコアSRAM保存用
 */
#define SRAM_HISCORE_NORMAL (4)
/**
 * ハイスコアSRAM保存用
 */
#define SRAM_HISCORE_HARD (8)

/**
 * トロフィーフラグ
 */
#define SRAM_TROPHY_NORMAL (12) // 4*6

#define SRAM_TROPHY_HARD (36) // 4*6


/**
 * 乱数の種
 */
#define SRAM_SEED (12)


/**
 * ゲームモード保存
 */
#define SRAM_MODE (16)


/**
 * SRAM書き込みフラグ
 */
#define SRAM_ON ((u32)0xf0f0f0f0)



///////////////////////////////////////////////////////////////////// COLOR

#define COLOR_TRANS RGB5(0,0,0)



///////////////////////////////////////////////////////////////////// PALETTE COLOR

#define PAL_TRANS (0)
#define PAL_ENEMY_MISSILE (15)



///////////////////////////////////////////////////////////////////// timer

#define DEF_TIMER_RELOAD (0)



///////////////////////////////////////////////////////////////////// マクロ

#define RND(MIN,MAX) ((MIN)+(int)(rand()/(float)RAND_MAX*((MAX)-(MIN)+1)))



///////////////////////////////////////////////////////////////////// グローバル変数

/**
 * ゲーム管理
 */
GLOBAL GameStateType game_state;
/**
 * ステージ管理
 */
GLOBAL StageType stage;

/**
 * AIM
 */
GLOBAL AimType aim;
/**
 * AMB
 */
GLOBAL AmbType amb;
/**
 * 都市
 */
GLOBAL CityType cities[MAX_CITIES];

/**
 * AMB管理
 */
GLOBAL AMBStateType am;

/**
 * 爆風管理
 */
GLOBAL BombStateType bomb;

/**
 * 敵管理
 */
GLOBAL EnemyStateType enemy;

/**
 * 補給アイテム
 */
GLOBAL SupplyType supply;

/**
 * 広範囲爆弾
 */
GLOBAL SupplyType superbomb;

/**
 * UFO
 */
GLOBAL UFOType ufo;

/**
 * スコア
 */
GLOBAL int score;
/**
 * ハイスコア
 */
GLOBAL int hiscore;

/**
 * メッセージ表示
 */
GLOBAL BlinkMessageType mes;
/**
 *　メッセージ表示
 */
GLOBAL BlinkMessageType lv_mes;
/**
 *　メッセージ表示
 */
GLOBAL BlinkMessageType trophy_mes;

/**
 * 乱数の種
 */
GLOBAL u32 seed;

/**
 * 現在のステージBGM
 */
GLOBAL int stage_bgm;

/**
 * トロフィー解除フラグ
 */
GLOBAL bool trophy_unlocked[MAX_TROPHY];



///////////////////////////////////////////////////////////////////// プロトタイプ

void
game ();
void
init_game ();
void
update_hiscore ();
void
disp_game_mode ();
void
load_title ();


#endif	/* _game_h_ */
