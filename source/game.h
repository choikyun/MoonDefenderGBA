/***************************************************
 Moon Defender GBA
 ver 1.00
 2017.02.01
 Choe Gyun (choi kyun)
 ***************************************************/

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

/*
 * 固定小数点
 */
#define FIX (16)



/*
 * ステージ
 */
#define STAGE_W_MIN (0)
#define STAGE_W_MAX (SCREEN_WIDTH-8)
#define STAGE_H_MIN (8)
#define STAGE_H_MAX (SCREEN_HEIGHT - 32 - 8)



/*
 * 数値表示
 */
#define NUM_W (8)
#define NUM_H (8)



/*
 * 数値の実際の幅
 */
#define REAL_NUM_W (NUM_W / 2)



/*
 * キー
 */
#define DEF_KEY_DELAY (1)
#define DEF_KEY_REPEAT (1)



/*
 * メッセージ表示ウェイト
 */
#define MES_WAIT (20)



/*
 * メッセージ点滅カウント
 */
#define MES_COUNT (10)



/***************************************************
 * ゲーム個別
 ***************************************************/

/***************************************************
 * モード
 ***************************************************/

/*
 * モード数
 */
#define MAX_MODE (2)




/***************************************************
 * 照準 スプライト
 ***************************************************/

/*
 * XY座標
 */
#define AIM_X (240 / 2 - 8 / 2)
#define AIM_Y ((160-8) / 2 - 8 / 2)


/*
 * 幅
 */
#define AIM_W (8)
#define AIM_H (8)


/*
 * 速度
 */
#define AIM_SPEED (1)


/*
 * 座標補正
 */
#define AIM_ADJUST_X (3)
#define AIM_ADJUST_Y (3)


/*
 * 加速度
 */
#define AIM_ACC (4096*2)
#define AIM_ACC_MAX (2 << FIX)


/*
 * 摩擦係数
 */
#define AIM_FRIC (0.08)


/*
 * アニメーション
 */
#define AIM_ANIME_INTERVAL (3)
#define AIM_ANIME_MAX_FRAME (3)


/*
 * AIM警告点滅間隔
 */
#define AIM_CAUTION_INTERVAL (4)


/*
 * 発射までのインターバル
 */
#define AIM_FIRE_INTERVAL (1)




/***************************************************
 * Anti-Missile-Battery スプライト
 ***************************************************/

/*
 * XY座標
 */
#define AMB_X (112)
#define AMB_Y (144)


/*
 * センター
 */
#define AMB_C (AMB_X + 8)


/*
 * アニメーション
 */
//#define AMB_ANIME_INTERVAL (8)
//#define AMB_MAX_FRAME (3)




/***************************************************
 * レベル
 ***************************************************/

/*
 * 表示桁
 */
#define LV_DIGIT (2)


/*
 * XY座標
 */
#define LV_X (36)
#define LV_Y (0)


/*
 * 最大レベル
 */
#define MAX_LEVEL (10)



/***************************************************
 * 都市
 ***************************************************/

/*
 * 都市数
 */
#define MAX_CITIES (6)


/*
 * 都市ダメージ
 */
#define CITY_MAX_DAMAGE (3)


/*
 * XY座標
 */
#define CITY_Y (SCREEN_HEIGHT - CITY_H - 4)
//#define CITY_X (0)


/*
 * 幅
 */
#define CITY_W (16)
#define CITY_H (10)




/***************************************************
 * 都市振動
 ***************************************************/

/*
 * 振幅
 */
#define SHOCK_RANGE (1)


/*
 * 間隔
 */
#define SHOCK_INTERVAL (4)


/*
 * 長さ
 */
#define SHOCK_DURATION (SHOCK_INTERVAL * 4)



/***************************************************
 * フラッシュ
 ***************************************************/

/*
 * 間隔
 */
#define FLASH_INTERVAL (4)

/*
 * 閃光の色
 */
#define FLASH_COLOR (15)


/***************************************************
 * ミサイル
 ***************************************************/

/*
 * 画面に表示される最大数
 */
#define MAX_ENEMY_MISSILES (12)


/*
 * 発射位置Y座標
 */
#define MISSILE_START_Y (8)


/*
 * 目標位置Y座標
 */
#define MISSILE_END_Y (SCREEN_HEIGHT - 8)


/*
 * 発射のインターバル
 */
#define INIT_ENEMY_FIRE_INTERVAL (2*60);


/*
 * AMBミサイル最大数
 */
#define MAX_AMB_MISSILES (3)


/*
 * AMBミサイル初期値
 */
#define INIT_AMB_MISSILES (30)


/*
 * AMBの速度
 */
#define AMB_MISSILE_SPEED (1)


/*
 * AMB弾数警告
 */
#define AMB_MISSILE_CAUTION (5)




/***************************************************
 * UFO
 ***************************************************/

/*
 * 出現インターバル
 */
#define UFO_APPEAR_INTERVAL (1*60)


/*
 * UFOミサイル速度
 */
#define UFO_MISSILE_SPEED (2)



/***************************************************
 * ミサイル弾頭
 ***************************************************/

/*
 * XY補正
 */
#define WARHEAD_ADJUST_X (-3)
#define WARHEAD_ADJUST_Y (-3)


/*
 * アニメーション
 */
#define WARHEAD_ANIME_INTERVAL (4)
#define WARHEAD_ANIME_MAX_FRAME (1)


/*
 * 当たり判定
 */
#define WARHEAD_HIT_WIDTH (8-1)
#define WARHEAD_HIT_HEIGHT (8-1)


/*
 * 多弾頭の確率
 */
#define MULTIPLE_PROB (4)
#define MAX_MULTIPLE (3)


/*
 * 分裂位置
 */
#define MULTIPLE_START (20)
#define MULTIPLE_START_MARGIN (20)



/***************************************************
 * マーカー
 ***************************************************/

#define MAX_MARKERS MAX_AMB_MISSILES



/***************************************************
 * 爆風
 ***************************************************/

/*
 * 当たり判定
 */
#define BOMB_HIT_WIDTH (16-1)
#define BOMB_HIT_HEIGHT (16-1)


/*
 * 爆風最大数
 */
#define MAX_BOMBS (32)


/*
 * 座標補正
 */
#define BOMB_ADJUST_X (-5)
#define BOMB_ADJUST_Y (-5)


/*
 * アニメーション
 */
#define BOMB_ANIME_INTERVAL (5)
#define BOMB_ANIME_MAX_FRAME (3)



/***************************************************
 * 残り弾数 敵,自機
 ***************************************************/

/*
 * 表示桁
 */
#define REMINING_DIGIT (3)


/*
 * XY座標
 */
#define REMINING_ENEMY_X (184)
#define REMINING_ENEMY_Y SCORE_Y
#define REMINING_AMB_X (216)
#define REMINING_AMB_Y SCORE_Y



/***************************************************
 * アイテム：補給
 ***************************************************/

/*
 * これ以下なら補給開始
 */
#define SUPPLY_DROP_LIMIT (10)


/*
 * 補給弾数
 */
#define SUPPLY_CHARGE_MISSILE (8);


/*
 * 幅
 */
#define SUPPLY_WIDTH (12)
#define SUPPLY_HEIGHT (10)


/*
 * 当たり判定
 */
#define SUPPLY_HIT_WIDTH (SUPPLY_WIDTH-1)
#define SUPPLY_HIT_HEIGHT (SUPPLY_HEIGHT-1)


/***************************************************
 * アイテム：広範囲爆弾
 ***************************************************/

/*
 * 敵ミサイルがこれ以上なら投下開始
 */
#define SUPERBOMB_ENEMY_UPPERLIMIT (10)


/*
 * スピード
 */
#define SUPERBOMB_SPEED (3)


/*
 * 出現間隔
 */
#define SUPERBOMB_DROP_INTERVAL (800)


/*
 * 爆破するミサイル数
 */
#define SUPERBOMB_POWER (5)


/*
 * 幅
 */
#define SUPERBOMB_WIDTH (8)
#define SUPERBOMB_HEIGHT (8)


/*
 * 当たり判定
 */
#define SUPERBOMB_HIT_WIDTH (SUPERBOMB_WIDTH-1)
#define SUPERBOMB_HIT_HEIGHT (SUPERBOMB_HEIGHT-1)

/***************************************************
 * アイテム一般
 ***************************************************/

/*
 * スピード
 */
#define ITEM_SPEED (5)


/*
 * アニメーション
 */
#define ITEM_ANIME_INTERVAL (4)
#define ITEM_ANIME_MAX_FRAME (1)


/*
 * 移動距離
 */
#define ITEM_LIMIT_Y (130)


/*
 * 出現間隔
 */
#define ITEM_DROP_INTERVAL (300)



/***************************************************
 *　情報表示
 ***************************************************/

/*
 * メッセージ表示
 */
#define MES_X (50)
#define MES_Y (68)
#define MES_W (140)
#define MES_H (24)


/*
 * レベルアップ点滅
 */
#define LV_MES_X (0)
#define LV_MES_Y (0)
#define LV_MES_W (34)
#define LV_MES_H (8)



/***************************************************
 *　シーン
 ***************************************************/

/*
 * シーン終了後のインターバル
 */
#define INTERMISSION (3*60)



/***************************************************
 *　モード選択
 ***************************************************/

/*
 * シーン終了後のインターバル
 */
#define MODE_ARROW_X (28)
#define MODE_ARROW_Y (84)

#define MODE_ARROW_X2 (38)



/***************************************************
 *　スコア
 ***************************************************/

/*
 * スコア加算のための高度間隔
 */
#define SCORE_HEIGHT_RANGE (32)


/*
 * スコア表示桁
 */
#define SCORE_DIGIT (8)


/*
 * スコア座標
 */
#define SCORE_X (88)
#define SCORE_Y LV_Y
#define HISCORE_X (70)
#define HISCORE_Y (65)




/***************************************************
 *　BGM
 ***************************************************/

/*
 * ステージBGM数
 */
#define MAX_STAGE_BGM (3)




///////////////////////////////////////////////////////////////////// クラス

// 座標
typedef struct
{ 
  int x;
  int y;
}ALIGN(4) PointType;



// 矩形座標
typedef struct
{ 
  int x;
  int y;
  int width;
  int height;
}ALIGN(4) RectangleType;


// 振動
typedef struct
{
  int range;
  int direc;
  int interval;
  int duration;
} ALIGN(4) ShockType;



// フラッシュ
typedef struct
{
  u8 color[32];
  int interval;
} ALIGN(4) FlashType;



// ゲーム管理
typedef struct
{ 
  int scene;
  int next_scene;
  int intermission; // 次のシーンまでの幕間
  u16 key;
  u16 keyr;
  int demo_start;
}ALIGN(4) GameStateType;



// ステージ管理
typedef struct
{ 
  int lv;
  int mode;
  int cities;
  bool demo;

  FlashType flash;
  RectangleType rect;
}ALIGN(4) StageType;



// キャラクタ
typedef struct
{ 
  int chr; // スプライト番号
  int tile;// タイル番号
  bool show;// 表示/非表示
  PointType coord;
}ALIGN(4) SpriteChrType;



// フレームアニメ
typedef struct
{ 
  bool is_start; // 開始
  int frame;// 現在のフレーム
  int max_frame;// フレーム最大値
  int interval;// インターバル
  int interval_rel;// インターバル既定値
}ALIGN(4) AnimeType;



// AIM
typedef struct
{ 
  PointType acc; // 加速度
  int fire_interval;
  FlashType flash;
  SpriteChrType sprite;
}ALIGN(4) AimType;



// AMB
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) AmbType;



// CITY
typedef struct
{ 
  int damage; // ダメージ
  
  ShockType shock;
  SpriteChrType sprite;
}ALIGN(4) CityType;



// ミサイル
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



// 爆風
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) BombType;



// マーカー
typedef struct
{ 
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) MarkerType;



// 補給アイテム
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



// 敵管理
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



// AMB管理
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



// 爆風管理
typedef struct
{ 
  int index; // 現在のバッファ位置
  BombType buf[MAX_BOMBS];
}ALIGN(4) BombStateType;



// UFO
typedef struct
{ 
  bool is_appeared; // 出現中か
  int direc_x;// X方向
  int max_step;// 移動量
  int stop_time;// 停止時間
  
  AnimeType anime;
  SpriteChrType sprite;
}ALIGN(4) UFOType;



// 点滅メッセージ
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



// ウェイト
typedef struct
{ 
  int wait;
}ALIGN(4) WaitType;



///////////////////////////////////////////////////////////////////// ENUM

// CITYダメージ
enum
{
  NOMAL, LITTLE, LARGE, DESTRUCTION
};



// シーン
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



// スプライトキャラクタ
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

#define TILE_AIM1 (512)
#define TILE_AIM2 (514)
#define TILE_AIM3 (516)
#define TILE_AIM4 (518)

#define TILE_WARHEAD1 (520)
#define TILE_WARHEAD2 (522)

#define TILE_WARHEAD_UFO1 (524)
#define TILE_WARHEAD_UFO2 (526)

#define TILE_MARKER1 (528)
#define TILE_MARKER2 (530)

#define TILE_CITY1 (532)
#define TILE_CITY2 (540)
#define TILE_CITY3 (548)
#define TILE_CITY4 (556)

#define TILE_AMB1 (564)

#define TILE_BOMB1 (596)

#define TILE_UFO (628)

#define TILE_ITEM1 (636)
#define TILE_ITME1_2 (644)

#define TILE_SUPERBOMB1 (652)
#define TILE_SUPERBOMB2 (660)

#define TILE_MODE_ARROW (668)



///////////////////////////////////////////////////////////////////// SRAM

/*
 * ハイスコアSRAM保存用
 */
#define SRAM_CHECK_HISC (0)
#define SRAM_HISCORE_NOMAL (4)
#define SRAM_HISCORE_HARD (8)


/*
 * 乱数の種
 */
#define SRAM_SEED (12)


/*
 * ゲームモード保存
 */
#define SRAM_MODE (16)


/*
 * SRAM書き込みフラグ
 */
#define SRAM_ON (1)



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

// ゲーム管理
GLOBAL GameStateType game_state;
GLOBAL StageType stage;

// キャラクタ
GLOBAL AimType aim;
GLOBAL AmbType amb;
GLOBAL CityType cities[MAX_CITIES];

// AMB管理
GLOBAL AMBStateType am;

// 爆風管理
GLOBAL BombStateType bomb;

// 敵管理
GLOBAL EnemyStateType enemy;

// 補給アイテム
GLOBAL SupplyType supply;

// 広範囲爆弾
GLOBAL SupplyType superbomb;

// UFO
GLOBAL UFOType ufo;

// スコア
GLOBAL u32 score;
GLOBAL u32 hiscore;

// メッセージ表示
GLOBAL BlinkMessageType mes;
// メッセージ表示
GLOBAL BlinkMessageType lv_mes;

// 乱数の種
GLOBAL u32 seed;

// 現在のステージBGM
GLOBAL int stage_bgm;



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
