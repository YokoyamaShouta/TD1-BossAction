#include <Novice.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


const char kWindowTitle[] = "TD BossAction";

struct Vector2
{
	float x;
	float y;
};

struct Charactor
{
	Vector2 pos;
	Vector2 radius;
	Vector2 direction;
	Vector2 rightTop;
	Vector2 leftBottom;
	float speed;
	float dashSpeed;
	float wide;
	float height;
	float velocity;
	float gravity;
	float jumpPower;
	float directionVecter;
	float browRange;
	bool isPunch;
	bool isJump;
	bool isCanShot; //キックしているか
	bool isAction; //パンチ
	bool isAction2; //パンチ２
	bool isAlive;
	bool isStore; //溜める
	bool isNone;
	bool isMove;
	bool isDash;
	bool isKick;
	int noneCount;
	int dashCount;
	int moveCount;
	int punchCount;
	int kickCount;
	int rivivalTime = 0;
	int jumpCount;
	int actionJudge;
	int shotCoolTime;
	int hp;
	int kickDamage;
	int punchDamage;
	int flameNumber;
	int actionCount;
	int actionCoolTime;
	int shakes;
};

struct  Bullet
{
	Vector2 pos;
	Vector2 direction;
	float speed;
	float wide;
	float height;
	float radius;
	float radiusAdd;
	Vector2 leftBottom;
	Vector2 rightTop;
	bool isShot;
	int damage;
};

struct Effect
{
	Vector2 pos;
	float speed;
	float radius;
};

//関数作成
#pragma region

void CharactorMove(Charactor& player, char* keys) //プレイヤー移動
{
	player.direction.x = 0.0f;

	if (keys[DIK_D])
	{
		player.direction.x = 1.0f;
	}

	if (keys[DIK_A])
	{
		player.direction.x = -1.0f;
	}

	player.pos.x += player.direction.x * player.speed;
}

void Jump(Charactor& player, char* keys, char* preKeys) //プレイヤージャンプ
{
	if (player.jumpCount < 2)
	{
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			player.jumpCount += 1;
			player.velocity = -player.jumpPower;
			player.isJump = true;
		}
	}
}

void enemyJump(Charactor& enemy) //敵ジャンプ
{
	enemy.velocity = -enemy.jumpPower;
	enemy.isJump = true;

	if (enemy.pos.y + enemy.radius.y >= 600.0f)
	{
		enemy.pos.y = 600.0f - enemy.radius.y;
		enemy.isJump = false;
		enemy.velocity = 0.0f;
	}
}

void MoveRange(Charactor& player) //プレイヤーの横移動範囲
{
	player.velocity += player.gravity;
	player.pos.y += player.velocity;

	if (player.pos.y + player.radius.y >= 600.0f)
	{
		player.pos.y = 600.0f - player.radius.y;
		player.isJump = false;
		player.jumpCount = 0;
		player.velocity = 0.0f;
	}

	if (player.pos.x - player.radius.x <= 0.0f)
	{
		player.pos.x = 0 + player.radius.x;
	}

	if (player.pos.x + player.radius.x >= 1280.0f)
	{
		player.pos.x = 1280.0f - player.radius.x;
	}
}

Bullet playerKick[4]; //衝撃波
void ShockWave(Charactor& player, Bullet& bullet) //衝撃波の描画
{
	for (int i = 0; i < 4; i++)
	{
		if (!playerKick[i].isShot)
		{
			player.shotCoolTime = 60;
			playerKick[i].isShot = true;
			playerKick[i].pos.x = player.pos.x;
			playerKick[i].pos.y = player.pos.y;
			playerKick[i].direction.x = player.directionVecter;
			playerKick[i].radius = bullet.radiusAdd;
			if (playerKick[i].radius <= 10.0f)
			{
				playerKick[i].damage = 1;
			}
			else if (playerKick[i].radius > 10.0f && playerKick[i].radius <= 21.0f)
			{
				playerKick[i].damage = 3;
			}
			break;
		}
	}
}

void MoveShockWave() // 衝撃波の動き
{
	for (int i = 0; i < 4; i++)
	{
		playerKick[i].pos.x += playerKick[i].direction.x * playerKick[i].speed;
	}
}

void ShockWaveRange() //衝撃波のfalse
{
	for (int i = 0; i < 4; i++)
	{
		if (playerKick[i].pos.x - playerKick[i].radius >= 1280.0f || playerKick[i].pos.x + playerKick[i].radius <= 0.0f)
		{
			playerKick[i].isShot = false;
			playerKick[i].radius = 0.0f;
		}
	}
}

Bullet enemyKick[5];
void enemyBrow(Charactor& enemy, Charactor& player) {//敵の物理攻撃

	if (enemy.pos.x >= player.pos.x) {
		enemy.pos.x += -1.5f;
	}
	else {
		enemy.pos.x += 1.5f;
	}
	for (int i = 0; i < 5; i++) {

		if (!enemyKick[i].isShot)
		{
			enemy.shotCoolTime = 10;
			enemyKick[i].isShot = true;
			enemyKick[i].pos.x = enemy.pos.x;
			enemyKick[i].pos.y = enemy.pos.y;
			enemyKick[i].direction.x = enemy.directionVecter;
			enemyKick[i].radius = 8.0f;
			enemyKick[i].damage = 1;
		}
	}
}

void GraphAnimation(int& animationFlameCount, int& flameNunber, int graphSheet) //　リソーススプレットシートの画像切り替わり
{
	animationFlameCount++;
	flameNunber = (animationFlameCount / 10) % graphSheet;

	if (animationFlameCount >= graphSheet * 10)
	{
		animationFlameCount = 0;
	}
}

void BackGroundAnimation(int& animationFlameCount, int& flameNunber, int graphSheet)
{
	animationFlameCount++;
	flameNunber = (animationFlameCount / 10) % graphSheet;

	if (animationFlameCount >= 80)
	{
		animationFlameCount = 80;
	}
}
#pragma endregion 関数

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//プレイヤーの変数　初期化
	Charactor player;
	player.pos.x = 200.0f;
	player.pos.y = 300.0f;
	player.wide = 128.0f;
	player.height = 128.0f;
	player.radius.x = 25.0f;
	player.radius.y = 64.0f;
	player.speed = 6.0f;
	player.velocity = 0.0f;
	player.gravity = 0.7f;
	player.jumpPower = 18.0f;
	player.direction.x = 0.0f;
	player.directionVecter = 1.0f;
	player.shotCoolTime = 0;
	player.hp = 30; //HP
	player.isJump = false;
	player.isCanShot = false;
	player.isStore = false;
	player.isAlive = true;
	player.isAction = false;
	player.isAction2 = false;
	player.rivivalTime = 10;
	player.actionCount = 0;
	player.kickDamage = 5;
	player.punchDamage = 4;
	player.shakes = 0;

	//プレイヤーの四隅
	player.rightTop.x = 0.0f;
	player.rightTop.y = 0.0f;
	player.leftBottom.x = 0.0f;
	player.leftBottom.y = 0.0f;

	//パンチしているときプレイヤー変数
	Charactor isPunchPlayer;
	isPunchPlayer.pos.x = 60.0f;
	isPunchPlayer.pos.y = 64.0f;
	isPunchPlayer.radius.x = 60.0f;
	isPunchPlayer.radius.y = 64.0f;

	//パンチしているときの四隅
	isPunchPlayer.rightTop.x = 0.0f;
	isPunchPlayer.rightTop.y = 0.0f;
	isPunchPlayer.leftBottom.x = 0.0f;
	isPunchPlayer.leftBottom.y = 0.0f;

	//キックしているときのプレイヤー変数
	Charactor isKickPlayer;
	isKickPlayer.pos.x = 0.0f;
	isKickPlayer.pos.y = 0.0f;
	isKickPlayer.radius.x = 64.0f;
	isKickPlayer.radius.y = 64.0f;

	//キックしているときの四隅
	isKickPlayer.rightTop.x = 0.0f;
	isKickPlayer.rightTop.y = 0.0f;
	isKickPlayer.leftBottom.x = 0.0f;
	isKickPlayer.leftBottom.y = 0.0f;

	//画像切り替えに必要な変数 その他
	//キック
	int playerKickGraphCount = 0;
	bool playerIsKick = false; //キックしているかのフラグ

	//歩き
	int playerMoveFlameCount = 0;
	int playerMoveFlameNumber = 0;

	//パンチ　キック
	int playerNextPunchCount = 0;
	int playerPunch2Count = 0;
	int playerPunchKeyCount = 0;

	int playerBlowFlameCount = 0;
	int playerBlowFlameNumber = 0;

	enum PlayerDirection
	{
		FRONT,
		BACK,
	};

	PlayerDirection playerDirection = FRONT;
	PlayerDirection playerMoveDirection = FRONT;

	//画像

	//向き
	int playerFrontGraph = Novice::LoadTexture("./image/playerFront.png");
	int playerBackGraph = Novice::LoadTexture("./image/playerBack.png");

	//歩いている
	int playerMoveFrontGraph = Novice::LoadTexture("./image/playerMoveFront.png");
	int playerMoveBackGraph = Novice::LoadTexture("./image/playerMoveBack.png");

	//ダメージを受けた時
	int playerHitFrontGraph = Novice::LoadTexture("./image/playerIsHitFront.png");
	int playerHitBackGraph = Novice::LoadTexture("./image/playerIsHitBack.png");

	//キック
	int kickFrontGraph = Novice::LoadTexture("./image/kickFont.png");
	int kickBackGraph = Novice::LoadTexture("./image/kickBaack.png");

	//ジャンプ
	int playerJumpFrontGraph = Novice::LoadTexture("./image/playerJumpFront.png");
	int playerJumpBackGraph = Novice::LoadTexture("./image/playerJumpBack.png");

	//パンチ
	int playerBlowFrontGraph = Novice::LoadTexture("./image/playerActionFront.png");
	int playerBlowBackGraph = Novice::LoadTexture("./image/playerActionBack.png");

	//背景
	int backgroundGraph = Novice::LoadTexture("./image/background.png");

	//タイトル画面
	/*int titleBackgroundGraph = Novice::LoadTexture("./image/titleBackground.png");*/

	//ゲームオーバー画面
	int gameOverGraph = Novice::LoadTexture("./image/gameOver.png");

	//クリア画面
	int gameClearGraph = Novice::LoadTexture("./image/Clear.png");

	//タイトル文字
	int titleLetterGraph = Novice::LoadTexture("./image/titleLetter.png");


	//BGM,SE

	//プレイヤーが攻撃を当てたとき
	int playerBlowBgmHandle = Novice::LoadAudio("./image/se_damage12.mp3");

	////プレイヤーの攻撃が空ぶったとき
	//int playerBlowSkyBgmHandle = Novice::LoadAudio("./image/se_swing2.mp3");

	//敵のキックが当たった時
	int enemyKickBgmHandle = Novice::LoadAudio("./image/taihou1.mp3");

	//敵が攻撃を当てたとき
	int enemyBlowBgmHandle = Novice::LoadAudio("./image/se_damage13.mp3");

	//ジャンプしたとき
	int jumpBgmHandle = Novice::LoadAudio("./image/se_jump1.mp3");

	int playerBlowPlayHandle = 0;
	int enemyBlowPlayHandle = 0;
	int enemyKickPlayHandle = 0;
	int jumpPlayHandle = 0;

	/*int playerBlowSkyPlayHandle = 0;*/
	//int titleMoveFlameNumber = 0;
	//int titleMoveFlameCount = 0;
	int gameOverMoveFlameNumber = 0;
	int gameOverMoveFlameCount = 0;
	//int gameClearMoveFlameNumber = 0;

	//衝撃波の変数　初期化
	const int playerKickCount = 4;

	for (int i = 0; i < playerKickCount; i++)
	{
		playerKick[i].direction.x = 1.0f;
		playerKick[i].pos.x = player.pos.x;
		playerKick[i].pos.y = player.pos.y;
		playerKick[i].speed = 10.0f;
		playerKick[i].radius = 0.0f;
		playerKick[i].isShot = false;
		playerKick[i].rightTop.x = 0.0f;
		playerKick[i].rightTop.y = 0.0f;
		playerKick[i].leftBottom.x = 0.0f;
		playerKick[i].leftBottom.y = 0.0f;
	}

	//弾を大きくする
	Bullet playerKickRadius;
	playerKickRadius.radiusAdd = 5.0f;

	//敵の変数
	Charactor enemy;
	enemy.pos.x = 900.0f;
	enemy.pos.y = 300.0f;
	enemy.radius.x = 38.0f;
	enemy.radius.y = 64.0f;
	enemy.direction.x = 0.0f;
	enemy.speed = 2.0f;
	enemy.dashSpeed = 5.0f;
	enemy.gravity = 0.7f;
	enemy.velocity = 0.0f;
	enemy.shotCoolTime = 60;
	enemy.hp = 60; //HP
	enemy.actionJudge = 0;
	enemy.isPunch = false;
	enemy.isAction = false;
	enemy.isCanShot = false;
	enemy.isMove = false;
	enemy.isDash = false;
	enemy.isKick = false;
	enemy.isAlive = true;
	enemy.rivivalTime = 0;
	enemy.dashCount = 30;
	enemy.moveCount = 30;
	enemy.noneCount = 30;
	enemy.punchCount = 30;
	enemy.kickCount = 40;
	enemy.actionCoolTime = 120;
	enemy.directionVecter = 1.0f;
	enemy.browRange = 90;
	enemy.kickDamage = 5;
	enemy.punchDamage = 4;
	enemy.shakes = 0;

	int shakesPlus = 21;
	int shakesMinus = 10;

	//敵の四隅
	enemy.rightTop.x = 0.0f;
	enemy.rightTop.y = 0.0f;
	enemy.leftBottom.x = 0.0f;
	enemy.leftBottom.y = 0.0f;

	//パンチしているときenemyの変数
	Charactor isPunchEnemy;
	isPunchEnemy.pos.x = 60.0f;
	isPunchEnemy.pos.y = 64.0f;
	isPunchEnemy.radius.x = 60.0f;
	isPunchEnemy.radius.y = 64.0f;

	//パンきnemyEの四隅
	isPunchEnemy.rightTop.x = 0.0f;
	isPunchEnemy.rightTop.y = 0.0f;
	isPunchEnemy.leftBottom.x = 0.0f;
	isPunchEnemy.leftBottom.y = 0.0f;

	//敵のキック
	Charactor isKickEnemy;
	isKickEnemy.pos.x = 0.0f;
	isKickEnemy.pos.y = 0.0f;
	isKickEnemy.radius.x = 64.0f;
	isKickEnemy.radius.y = 30.0f;

	//敵がキックしてる時の四隅
	isKickEnemy.rightTop.x = 0.0f;
	isKickEnemy.rightTop.y = 0.0f;
	isKickEnemy.leftBottom.x = 0.0f;
	isKickEnemy.leftBottom.y = 0.0f;

	//敵の画像切り替えの変数

	//シーン切り替え
	PlayerDirection enemyDirection = FRONT;

	//敵の画像
	int  bossFrontGraph = Novice::LoadTexture("./image/boss1.png");
	int  bossBackGraph = Novice::LoadTexture("./image/boss1Back.png");

	int bossMoveFrontGraph = Novice::LoadTexture("./image/bossFront.png");
	int bossMoveBackGraph = Novice::LoadTexture("./image/bossBack.png");
	int enemyrMoveFlameCount = 0;
	int enemyMoveFlameNumber = 0;

	//敵のパンチ
	int enemyBlowFrontGraph = Novice::LoadTexture("./image/bossKickFront.png");
	int enemyBlowBackGraph = Novice::LoadTexture("./image/bossKickBack.png");
	int enemyBlowFlameCount = 0;
	int enemyBlowFlameNumber = 0;

	//敵のジャンプキック
	int enemyJumpKickFront = Novice::LoadTexture("./image/bossJumpKickFront.png");
	int enemyJumpKickBack = Novice::LoadTexture("./image/bossJumpKickBack.png");
	int enemyJumpKickflameCount = 0;
	int enemyJumpKickflameNumber = 0;

	//敵がダメージを食らったとき
	int enemyHitFrontGraph = Novice::LoadTexture("./image/bossHitFront.png");
	int enemyHitBackGraph = Novice::LoadTexture("./image/bossHitBack.png");


	//敵の挙動　種類
	enum BossAction
	{
		NONE,//何もしない
		MOVE, //歩き
		DASH, //ダッシュ
		JUMPKICK, //衝撃波
		PUNCH,  //パンチ・キック
		ENEMYJANP, //ジャンプ
	};

	BossAction bossAction = NONE;

	unsigned int currentTime = static_cast<int>(time(nullptr));
	srand(currentTime);

	/*Effect hit[4];
	for (int i = 0; i < 4; i++)
	{

	}*/

	enum SCENE
	{
		TITLE,
		SETUMEI,
		GAMEPLAY,
		GAMEEND,
		GAMECLEAR
	};
	SCENE sceneNow = TITLE;

	int setumeiGraph = Novice::LoadTexture("./image/setumei.png");
	int titleGraph = Novice::LoadTexture("./image/titleTmp.png");
	int playAgainGraph = Novice::LoadTexture("./image/playAgain.png");


	int backImg[30];
	for (int i = 0; i < 30; i++) {
		char filePath[128];
		sprintf_s(filePath, "./image/Frame%02d.png", i);
		backImg[i] = Novice::LoadTexture(filePath);
	}

	int frameCounter = 0;
	int lastFrameHoldTime = 600; //
	int currentFrame = 0;
	int frameIndex = 0;



	int graphPosY = 500;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (sceneNow)
		{
		case TITLE:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				sceneNow = GAMEPLAY;
			}

			if (keys[DIK_TAB] && !preKeys[DIK_TAB])
			{
				sceneNow = SETUMEI;
			}

			currentFrame = frameCounter / 5; // 每帧显示5次
			if (currentFrame >= 29) { // 最后一帧逻辑
				if (frameCounter < 29 * 5 + lastFrameHoldTime) {
					frameCounter++;
				}
				else {
					frameCounter = 0; // 循环回到第一帧
				}
			}
			else {
				frameCounter++;
			}

			player.pos.x = 200.0f;
			player.pos.y = 300.0f;
			player.hp = 30; //HP
			enemy.pos.x = 900.0f;
			enemy.pos.y = 300.0f;
			enemy.hp = 60; //HP

			break;
		case SETUMEI:
			if (keys[DIK_TAB] && !preKeys[DIK_TAB])
			{
				sceneNow = TITLE;
			}

			break;
		case GAMEPLAY:
#pragma region

			// 左右移動
			if (!player.isStore && !playerIsKick && !player.isAction && !player.isAction2)
			{
				CharactorMove(player, keys);
			}

			if (keys[DIK_D])
			{
				player.directionVecter = 1.0f; //if文外でも使えるプレイヤーの向き
				player.direction.x = 1.0f;	   // プレイヤーの移動するための向き
				playerDirection = FRONT;	   //プレイヤーの向き	
				playerMoveDirection = FRONT;   //プレイヤーが動いているときの向き
			}

			if (keys[DIK_A])
			{
				player.directionVecter = -1.0f;
				player.direction.x = -1.0f;
				playerDirection = BACK;
				playerMoveDirection = BACK;
			}

			if (keys[DIK_A] || keys[DIK_D])
			{
				GraphAnimation(playerMoveFlameCount, playerMoveFlameNumber, 4); //スプレットシートを動かす関数　使用
			}

			// プレイヤーのジャンプ
			if (!player.isAction && !player.isAction2 && !player.isStore)
			{
				Jump(player, keys, preKeys);

				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] /*&& player.jumpCount > 2*/) {

					if (!Novice::IsPlayingAudio(jumpPlayHandle)) {
						jumpPlayHandle = Novice::PlayAudio(jumpBgmHandle, false, 1.0f);
					}

				}
				
			}

			//キック
			if (player.isCanShot)
			{
				for (int i = 0; i < playerKickCount; i++)
				{
					if (!playerKick[i].isShot && !player.isJump && !player.isAction && !player.isAction2)
					{
						if (keys[DIK_E]) //弾を溜めている
						{
							if (playerKickRadius.radiusAdd <= 20.0f)
							{
								playerKickRadius.radiusAdd += 0.05f;
								player.isStore = true;
							}
						}
					}
				}

				if (!keys[DIK_E] && preKeys[DIK_E] && player.isStore) //キーを離されたとき　弾を描画する
				{
					player.isStore = false;
					playerIsKick = true;
					ShockWave(player, playerKickRadius);
					playerKickRadius.radiusAdd = 5.0f;
				}
			}
			//キックした時の描画
			if (playerIsKick)
			{
				playerKickGraphCount++;
			}
			//キックして弾が出た時の描画
			if (playerKickGraphCount > 10)
			{
				playerIsKick = false;
				playerKickGraphCount = 0;
			}

			//パンチ キック
			if (keys[DIK_Q] && !preKeys[DIK_Q] && !player.isJump && !player.isStore && !player.isAction2)
			{
				playerPunchKeyCount += 1;
				player.isAction = true;
			}

			if (player.isAction)
			{
				GraphAnimation(playerBlowFlameCount, playerBlowFlameNumber, 3);

				playerNextPunchCount++;

				if (playerNextPunchCount >= 2 && playerNextPunchCount <= 30 && playerPunchKeyCount == 1)
				{
					if (keys[DIK_Q] && !preKeys[DIK_Q])
					{
						playerPunchKeyCount += 1;
					}
				}
				else if (playerNextPunchCount >= 30 && playerPunchKeyCount != 2)
				{
					player.isAction = false;
				}
			}
			else
			{
				playerPunchKeyCount = 0;
				playerNextPunchCount = 0;
			}

			if (playerPunchKeyCount == 2 && playerNextPunchCount >= 30)
			{
				player.isAction = false;
				player.isAction2 = true;
			}

			if (player.isAction2) //二発目が出る
			{
				GraphAnimation(playerBlowFlameCount, playerBlowFlameNumber, 3);
				playerPunch2Count++;
			}

			if (playerPunch2Count >= 30)
			{
				player.isAction2 = false;
				playerPunch2Count = 0;
				playerPunchKeyCount = 0;
			}

			//衝撃波の移動
			MoveShockWave();
			ShockWaveRange();

			//衝撃波のクールタイム
			if (player.shotCoolTime >= 0)
			{
				player.shotCoolTime--;
			}

			if (player.shotCoolTime <= 0)
			{
				player.isCanShot = true;
			}
			else
			{
				player.isCanShot = false;
			}
#pragma endregion プレイヤー

			//敵の向き
			if (enemy.pos.x >= player.pos.x)
			{
				enemyDirection = BACK;
			}
			else
			{
				enemyDirection = FRONT;
			}

			//敵の攻撃 決める
			if (!enemy.isAction)
			{
				enemy.actionJudge = static_cast<int>(rand() % 9 + 1) ;
				enemy.isAction = true;
			}
			
			//敵の攻撃を紐づける		
			if (enemy.actionJudge >= 1 && enemy.actionJudge <= 3)
			{
				bossAction = NONE;
				enemy.isNone = true;
			}
			else if (enemy.actionJudge > 3 && enemy.actionJudge <= 5)
			{
				bossAction = MOVE;
				enemy.isMove = true;
			}
			else if (enemy.actionJudge > 5 && enemy.actionJudge <= 6)
			{
				bossAction = DASH;
				enemy.isDash = true;
			}
			else if (enemy.actionJudge >= 7 && enemy.actionJudge <= 9)
			{
				bossAction = JUMPKICK;
				if (!enemy.isKick)
				{
					enemy.isKick = true;

					if (enemy.pos.x >= player.pos.x)
					{
						enemy.direction.x = -4.0f;
					}
					else
					{
						enemy.direction.x = 4.0f;
					}					
					break;
				}				
			}

			switch (enemyDirection)
			{
			case FRONT:
				if (enemy.pos.x + enemy.browRange >= player.pos.x && !enemy.isKick)
				{
					enemy.isPunch = true;
					enemy.isDash = false;
					enemy.isNone = false;
					enemy.isMove = false;
					bossAction = PUNCH;
				}
				break;
			case BACK:
				if (enemy.pos.x - enemy.browRange <= player.pos.x && !enemy.isKick)
				{
					enemy.isPunch = true;
					enemy.isDash = false;
					enemy.isNone = false;
					enemy.isMove = false;
					bossAction = PUNCH;
				}
				break;
			}

			//敵の挙動
			switch (bossAction)
			{
			case NONE:
				if (enemy.noneCount >= 0)
				{
					enemy.noneCount--;
					if (enemy.noneCount <= 0)
					{
						enemy.noneCount = 40;
						enemy.isNone = false;
						enemy.isAction = false;
					}
				}

				if (!enemy.isNone)
				{
					enemy.noneCount = 40;
				}

				break;
			case MOVE:
				if (enemy.moveCount >= 0)
				{
					enemy.moveCount--;
					if (enemy.moveCount <= 0)
					{
						enemy.isMove = false;
						enemy.isAction = false;
					}
				}
				if (enemy.isMove)
				{
					switch (enemyDirection)
					{
					case FRONT:
						enemy.direction.x = 1.0f;
						break;
					case BACK:
						enemy.direction.x = -1.0f;
						break;
					}
				}

				enemy.pos.x += enemy.direction.x * enemy.speed;

				if (!enemy.isMove)
				{
					enemy.moveCount = 40;
				}

				GraphAnimation(enemyrMoveFlameCount, enemyMoveFlameNumber, 4);
				break;
			case DASH:
				if (enemy.dashCount >= 0)
				{
					enemy.dashCount--;
					if (enemy.dashCount <= 0)
					{
						enemy.isDash = false;
						enemy.isAction = false;
					}
				}

				if (enemy.isDash)
				{
					switch (enemyDirection)
					{
					case FRONT:
						enemy.direction.x = 2.0f;
						break;
					case BACK:
						enemy.direction.x = -2.0f;
						break;
					}
				}

				if (!enemy.isDash)
				{
					enemy.dashCount = 20;
				}
				
				enemy.pos.x += enemy.direction.x * enemy.speed;

				GraphAnimation(enemyrMoveFlameCount, enemyMoveFlameNumber, 4);

				break;
			case JUMPKICK:
				if (enemy.kickCount >= 0)
				{
					enemy.kickCount--;
					if (enemy.kickCount <= 0)
					{
						enemy.kickCount = 40;
						enemy.isKick = false;
						enemy.isAction = false;
					}
				}

				
				
				
				enemy.pos.x += enemy.direction.x * enemy.speed;

				GraphAnimation(enemyJumpKickflameCount, enemyJumpKickflameNumber, 2);
				break;
			case PUNCH:
				if (enemy.punchCount >= 0)
				{
					enemy.punchCount--;
					if (enemy.punchCount <= 0)
					{
						enemy.punchCount = 20;
						enemy.isPunch = false;
						enemy.isAction = false;
					}
				}
				GraphAnimation(enemyBlowFlameCount, enemyBlowFlameNumber, 2);
				break;
			}

			//敵の攻撃の挙動制限

#pragma region

			//プレイヤー 敵 の横移動範囲
			MoveRange(player);
			MoveRange(enemy);

			//当たり判定

			//弾の左下、右上の座標
			for (int i = 0; i < playerKickCount; i++)
			{
				playerKick[i].rightTop.x = playerKick[i].pos.x + playerKick[i].radius;
				playerKick[i].rightTop.y = playerKick[i].pos.y - playerKick[i].radius;
				playerKick[i].leftBottom.x = playerKick[i].pos.x - playerKick[i].radius;
				playerKick[i].leftBottom.y = playerKick[i].pos.y + playerKick[i].radius;
			}

			//プレイヤーの左下、右上の座標
			player.rightTop.x = player.pos.x + player.radius.x;
			player.rightTop.y = player.pos.y - player.radius.y;
			player.leftBottom.x = player.pos.x - player.radius.x;
			player.leftBottom.y = player.pos.y + player.radius.y;

			//パンチしてるときのプレイヤーの四隅
			isPunchPlayer.rightTop.x = player.pos.x + isPunchPlayer.radius.x;
			isPunchPlayer.rightTop.y = player.pos.y - isPunchPlayer.radius.y;
			isPunchPlayer.leftBottom.x = player.pos.x - isPunchPlayer.radius.x;
			isPunchPlayer.leftBottom.y = player.pos.y + isPunchPlayer.radius.y;

			//キックしているときのプレイヤーの四隅
			isKickPlayer.rightTop.x = player.pos.x + isKickPlayer.radius.x;
			isKickPlayer.rightTop.y = player.pos.y - isKickPlayer.radius.y;
			isKickPlayer.leftBottom.x = player.pos.x - isKickPlayer.radius.x;
			isKickPlayer.leftBottom.y = player.pos.y + isKickPlayer.radius.y;

			//敵の四隅

			//敵の左下、右上の座標
			enemy.rightTop.x = enemy.pos.x + enemy.radius.x;
			enemy.rightTop.y = enemy.pos.y - enemy.radius.y;
			enemy.leftBottom.x = enemy.pos.x - enemy.radius.x;
			enemy.leftBottom.y = enemy.pos.y + enemy.radius.y;

			//パンチしてるときのenemyの四隅
			isPunchEnemy.rightTop.x = enemy.pos.x + isPunchEnemy.radius.x;
			isPunchEnemy.rightTop.y = enemy.pos.y - isPunchEnemy.radius.y;
			isPunchEnemy.leftBottom.x = enemy.pos.x - isPunchEnemy.radius.x;
			isPunchEnemy.leftBottom.y = enemy.pos.y + isPunchEnemy.radius.y;

			//キックしている敵の四隅
			isKickEnemy.rightTop.x = enemy.pos.x + isKickEnemy.radius.x;
			isKickEnemy.rightTop.y = enemy.pos.y - isKickEnemy.radius.y;
			isKickEnemy.leftBottom.x = enemy.pos.x - isKickEnemy.radius.x;
			isKickEnemy.leftBottom.y = enemy.pos.y + isKickEnemy.radius.y;

			//弾と敵の当たり判定（矩形）
			for (int i = 0; i < playerKickCount; i++)
			{
				if (playerKick[i].rightTop.x > enemy.leftBottom.x &&
					playerKick[i].leftBottom.x < enemy.rightTop.x &&
					playerKick[i].rightTop.y < enemy.leftBottom.y &&
					playerKick[i].leftBottom.y > enemy.rightTop.y &&
					playerKick[i].isShot && enemy.isAlive
					)
				{
					enemy.isAlive = false;
					enemy.hp -= playerKick[i].damage;
				}
			}

			//敵と自分の当たり判定
			if (player.rightTop.x > enemy.leftBottom.x &&
				player.leftBottom.x < enemy.rightTop.x &&
				player.rightTop.y < enemy.leftBottom.y &&
				player.leftBottom.y > enemy.rightTop.y &&
				player.isAlive && enemy.isAlive
				)
			{
				player.isAlive = false;
				player.hp -= 1;
			}

			//パンチしているときの敵との当たり判定
			if (player.isAction || player.isAction2)
			{
				if (isPunchPlayer.rightTop.x > enemy.leftBottom.x &&
					isPunchPlayer.leftBottom.x < enemy.rightTop.x &&
					isPunchPlayer.rightTop.y < enemy.leftBottom.y &&
					isPunchPlayer.leftBottom.y > enemy.rightTop.y &&
					player.isAlive && enemy.isAlive
					)
				{
					enemy.isAlive = false;
					enemy.hp -= player.punchDamage;
					if (!Novice::IsPlayingAudio(playerBlowPlayHandle)) {
						playerBlowPlayHandle = Novice::PlayAudio(playerBlowBgmHandle, false, 1.0f);
					}
				}
				
			}

			//キックしているときの敵との当たり判定
			if (isKickPlayer.rightTop.x > enemy.leftBottom.x &&
				isKickPlayer.leftBottom.x < enemy.rightTop.x &&
				isKickPlayer.rightTop.y < enemy.leftBottom.y &&
				isKickPlayer.leftBottom.y > enemy.rightTop.y &&
				player.isAlive && enemy.isAlive &&
				playerIsKick)
			{
				enemy.isAlive = false;
				enemy.hp -= player.kickDamage;
			}

			//ここから敵からプレイヤー
			//パンチしているときのplayerとの当たり判定(敵目線)
			if (enemy.isPunch)
			{
				if (isPunchEnemy.rightTop.x > player.leftBottom.x &&
					isPunchEnemy.leftBottom.x < player.rightTop.x &&
					isPunchEnemy.rightTop.y < player.leftBottom.y &&
					isPunchEnemy.leftBottom.y > player.rightTop.y &&
					enemy.isAlive && player.isAlive
					)
				{
					player.isAlive = false;
					player.hp -= enemy.punchDamage;
					if (!Novice::IsPlayingAudio(enemyBlowPlayHandle)) {
						enemyBlowPlayHandle = Novice::PlayAudio(enemyBlowBgmHandle, false, 1.0f);
					}
				}
			}

			if (enemy.isKick)
			{
				if (isKickEnemy.rightTop.x > player.leftBottom.x &&
					isKickEnemy.leftBottom.x < player.rightTop.x &&
					isKickEnemy.rightTop.y < player.leftBottom.y &&
					isKickEnemy.leftBottom.y > player.rightTop.y &&
					enemy.isAlive && player.isAlive
					)
				{
					player.isAlive = false;
					player.hp -= enemy.punchDamage;
					if (!Novice::IsPlayingAudio(enemyKickPlayHandle)) {
						enemyKickPlayHandle = Novice::PlayAudio(enemyKickBgmHandle, false, 1.0f);
					}
				}
			}
			//復活　
			//敵
			if (!enemy.isAlive)
			{
				enemy.rivivalTime++;

				if (enemy.rivivalTime >= 30)
				{
					enemy.isAlive = true;
					enemy.rivivalTime = 0;
				}
			}

			//プレイヤー
			if (!player.isAlive)
			{
				player.rivivalTime++;


				//シェイク
				player.shakes = (rand() % shakesPlus - shakesMinus);

				shakesPlus -= 2;
				shakesMinus -= 1;

				if (shakesPlus == 0 || shakesMinus == 0) {

					shakesPlus = 41;
					shakesMinus = 20;
				}

				if (player.rivivalTime >= 20)
				{
					player.isAlive = true;
					player.rivivalTime = 0;
				}
			}

			if (player.hp <= 0)
			{
				sceneNow = GAMEEND;
			}

			if (enemy.hp <= 0)
			{
				sceneNow = GAMECLEAR;
			}
#pragma endregion

			break;
		case GAMEEND:

			BackGroundAnimation(gameOverMoveFlameCount, gameOverMoveFlameNumber, 9);

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				sceneNow = TITLE;
			}

			break;
		case GAMECLEAR:

			/*GraphAnimation(titleMoveFlameCount, titleMoveFlameNumber, 30);*/

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				sceneNow = TITLE;
			}

			break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (sceneNow)
		{
		case TITLE:

			Novice::DrawSprite(0, 0, titleGraph, 1, 1, 0.0f, WHITE);
			frameIndex = (currentFrame >= 30) ? 29 : currentFrame; // 确保索引不越界
			Novice::DrawSprite(0, 0, backImg[frameIndex], 1, 1, 0, WHITE);
			/*Novice::DrawSpriteRect(0, 0, 1280 * titleMoveFlameNumber, 0, 1280, 720, titleBackgroundGraph, 1.0f / 30.0f, 1.0f, 0.0f, WHITE);*/
			Novice::DrawSprite(-40, 100, titleLetterGraph, 1, 1, 0.0f, WHITE);

			break;
		case SETUMEI:
			Novice::DrawSprite(0, 0, setumeiGraph, 1.0f, 1.0f, 0.0f, WHITE);
			break;
		case GAMEPLAY:
#pragma region			
			Novice::DrawSprite(0, 0, backgroundGraph, 1, 1, 0.0f, WHITE);

			if (player.isAlive)
			{
				if (!player.isStore && !playerIsKick && !player.isJump && !player.isAction && !player.isAction2)
				{
					switch (playerDirection)
					{
					case FRONT:
						if (!preKeys[DIK_D])
						{
							Novice::DrawSprite((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerFrontGraph, 1, 1, 0.0f, WHITE);
						}
						break;
					case BACK:
						if (!preKeys[DIK_A])
						{
							Novice::DrawSprite((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerBackGraph, 1, 1, 0.0f, WHITE);
						}
						break;
					}

					switch (playerMoveDirection)
					{
					case FRONT:
						if (keys[DIK_D])
						{
							Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 128 * playerMoveFlameNumber, 0, 128, 128, playerMoveFrontGraph, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
						}
						break;
					case BACK:
						if (keys[DIK_A])
						{
							Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 128 * playerMoveFlameNumber, 0, 128, 128, playerMoveBackGraph, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
						}
						break;
					}
				}

				if (player.isJump) // ジャンプ
				{
					switch (playerDirection)
					{
					case FRONT:
						Novice::DrawSprite((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerJumpFrontGraph, 1, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSprite((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerJumpBackGraph, 1, 1, 0.0f, WHITE);
						break;
					}
				}

				if (player.isStore) // 溜めてる時のスプライト
				{
					switch (playerDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 0, 0, 128, 128, kickFrontGraph, 1 / 2.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 0, 0, 128, 128, kickBackGraph, 1 / 2.0f, 1, 0.0f, WHITE);
						break;
					}
				}

				if (playerIsKick) // キックした時(キーが話されたとき)
				{
					switch (playerDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 128, 0, 128, 128, kickFrontGraph, 1 / 2.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 128, 0, 128, 128, kickBackGraph, 1 / 2.0f, 1, 0.0f, WHITE);
						break;
					}
				}

				if (player.isAction) // パンチ
				{
					switch (playerDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerBlowFlameNumber * 128, 0, 128, 128, playerBlowFrontGraph, 1 / 6.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerBlowFlameNumber * 128, 0, 128, 128, playerBlowBackGraph, 1 / 6.0f, 1, 0.0f, WHITE);
						break;
					}
				}

				if (player.isAction2) // 二回目のパンチ
				{
					switch (playerDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, (4 + playerBlowFlameNumber) * 128, 0, 128, 128, playerBlowFrontGraph, 1 / 6.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, (4 + playerBlowFlameNumber) * 128, 0, 128, 128, playerBlowBackGraph, 1 / 6.0f, 1, 0.0f, WHITE);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				switch (playerDirection)
				{
				case FRONT:
					Novice::DrawSprite((int)player.pos.x - (int)player.radius.y + player.shakes, (int)player.pos.y - (int)player.radius.y + player.shakes, playerHitFrontGraph, 1, 1, 0.0f, WHITE);
					Novice::DrawEllipse((int)player.pos.x + player.shakes, (int)player.pos.y+ player.shakes, (int)player.radius.x, (int)player.radius.y, 0.0f, RED, kFillModeWireFrame);
					break;
				case BACK:
					Novice::DrawSprite((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, playerHitBackGraph, 1, 1, 0.0f, WHITE);
					Novice::DrawEllipse((int)player.pos.x + player.shakes, (int)player.pos.y + player.shakes, (int)player.radius.x, (int)player.radius.y, 0.0f, RED, kFillModeWireFrame);
					break;
				}
			}

			for (int i = 0; i < playerKickCount; i++) //キック
			{
				if (playerKick[i].isShot)
				{
					Novice::DrawEllipse((int)playerKick[i].pos.x, (int)playerKick[i].pos.y, (int)playerKick[i].radius, (int)playerKick[i].radius, 0.0f, BLUE, kFillModeSolid);
				}
			}
#pragma endregion プレイヤー

			//敵
			if (enemy.isAlive)
			{
				switch (bossAction)
				{
				case NONE:
					switch (enemyDirection)
					{
					case FRONT:
						Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, bossFrontGraph, 1.0f, 1.0f, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, bossBackGraph, 1.0f, 1.0f, 0.0f, WHITE);
						break;
					}
					break;
				case MOVE:
					switch (enemyDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyMoveFlameNumber * 128, 0, 128, 128, bossMoveFrontGraph, 1.0f / 4.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyMoveFlameNumber * 128, 0, 128, 128, bossMoveBackGraph, 1.0f / 4.0f, 1, 0.0f, WHITE);
						break;
					}
					break;
				case DASH:
					switch (enemyDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyMoveFlameNumber * 128, 0, 128, 128, bossMoveFrontGraph, 1.0f / 4.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyMoveFlameNumber * 128, 0, 128, 128, bossMoveBackGraph, 1.0f / 4.0f, 1, 0.0f, WHITE);
						break;
					}
					break;
				case JUMPKICK:
					Novice::DrawEllipse((int)enemy.pos.x, (int)enemy.pos.y + 15, (int)isKickEnemy.radius.x, (int)isKickEnemy.radius.y, 0.0f, BLUE, kFillModeWireFrame);

					switch (enemyDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyJumpKickflameNumber * 128, 0, 128, 128, enemyJumpKickFront, 1.0f / 2.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyJumpKickflameNumber * 128, 0, 128, 128, enemyJumpKickBack, 1.0f / 2.0f, 1, 0.0f, WHITE);
						break;
					}
					break;

				case PUNCH:
					switch (enemyDirection)
					{
					case FRONT:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyBlowFlameNumber * 128, 0, 128, 128, enemyBlowFrontGraph, 1.0f / 2.0f, 1, 0.0f, WHITE);
						break;
					case BACK:
						Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyBlowFlameNumber * 128, 0, 128, 128, enemyBlowBackGraph, 1.0f / 2.0f, 1, 0.0f, WHITE);
						break;
					}
				}
			}
			else {

				switch (enemyDirection)
				{
				case FRONT:
					Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyHitFrontGraph, 1, 1, 0.0f, WHITE);
					break;
				case BACK:
					Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, enemyHitBackGraph, 1, 1, 0.0f, WHITE);
					break;
				}

			}
			Novice::DrawBox(80, 30, player.hp * 16, 20, 0.0f, 0xff000085, kFillModeSolid);
			Novice::DrawBox(700, 30, enemy.hp * 8, 20, 0.0f, 0xff000085, kFillModeSolid);
		
			Novice::ScreenPrintf(10, 80, "%d", enemy.kickCount);
			break;
		case GAMEEND:

			Novice::DrawSpriteRect(0, 0, 1280 * gameOverMoveFlameNumber, 0, 1280, 720, gameOverGraph, 1.0f / 9.0f, 1.0f, 0.0f, WHITE);

			/*if (gameOverMoveFlameNumber >= 90) {
				gameOverMoveFlameNumber = 90;
			}*/

				if (graphPosY >= 200)
				{
					graphPosY--;
					Novice::DrawSprite(0, graphPosY, playAgainGraph, 1.0f, 1.0f, 0.0f, WHITE);
				}
			
				if (graphPosY <= 200)
				{
					graphPosY = 200;
				}
			Novice::ScreenPrintf(10, 10, "%d", graphPosY);

			break;
		case GAMECLEAR:
			Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0xFFFFFFFF, kFillModeSolid);
			Novice::DrawSprite(0, 0, gameClearGraph, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0, 300, playAgainGraph, 1.0f, 1.0f, 0.0f, WHITE);
			break;

			break;
		default:
			break;
		}


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}

	}
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
