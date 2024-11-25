#include <Novice.h>
#include <time.h>
#include <stdlib.h>

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
	float wide;
	float height;
	float velocity;
	float gravity;
	float jumpPower;
	float directionVecter;
	bool isJump;
	bool isCanShot;
	bool isAction; 
	bool isAction2;
	bool isAlive;
	bool isStore; //溜める
	int rivivalTime;
	int jumpCount;
	int actionJudge;
	int shotCoolTime;
	int hp;
	int damege;
	int flameNumber;
	int actionCount;
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
	int damege;
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

void Jump(Charactor& player,char* keys,char* preKeys) //プレイヤージャンプ
{	
	if (player.jumpCount <= 2)
	{
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			player.jumpCount += 1;
			player.velocity = -player.jumpPower;
			player.isJump = true;
		}
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
			player.shotCoolTime = 10;
			playerKick[i].isShot = true;
			playerKick[i].pos.x = player.pos.x;
			playerKick[i].pos.y = player.pos.y;
			playerKick[i].direction.x = player.directionVecter;
			playerKick[i].radius = bullet.radiusAdd;
			playerKick[i].damege = 1;
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

void GraphAnimation(int& animationFlameCount, int& flameNunber, int graphSheet) //　リソーススプレットシートの画像切り替わり
{
	animationFlameCount++;
	flameNunber = (animationFlameCount / 10) % graphSheet;

	if (animationFlameCount >= graphSheet * 10)
	{
		animationFlameCount = 0;
	}
}
#pragma endregion 関数

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//プレイヤーの変数　初期化
	Charactor player;
	player.pos.x = 200.0f;
	player.pos.y = 300.0f;
	player.wide = 128.0f;
	player.height = 128.0f;
	player.radius.x = 32.0f;
	player.radius.y = 64.0f;
	player.speed = 6.0f;
	player.velocity = 0.0f;
	player.gravity = 0.7f;
	player.jumpPower = 18.0f;
	player.direction.x = 0.0f;
	player.directionVecter = 1.0f;
	player.shotCoolTime = 60;
	player.hp = 5;
	player.isJump = false;
	player.isCanShot = false;
	player.isStore = false;
	player.isAction = false;
	player.isAction2 = false;
	player.rivivalTime = 10;
	player.actionCount = 0;

	//プレイヤーの四隅
	player.rightTop.x = 0.0f;
	player.rightTop.y = 0.0f;
	player.leftBottom.x = 0.0f;
	player.leftBottom.y = 0.0f;

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
	
	//キック
	int kickFrontGraph = Novice::LoadTexture("./image/kickFont.png");
	int kickBackGraph = Novice::LoadTexture("./image/kickBaack.png");

	//ジャンプ
	int playerJumpFrontGraph = Novice::LoadTexture("./image/playerJumpFront.png");
	int playerJumpBackGraph = Novice::LoadTexture("./image/playerJumpBack.png");

	//パンチ
	int playerBlowFrontGraph = Novice::LoadTexture("./image/playerActionFront.png");
	int playerBlowBackGraph = Novice::LoadTexture("./image/playerActionBack.png");
	
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
	enemy.pos.x = 700.0f;
	enemy.pos.y = 300.0f;
	enemy.radius.x = 38.0f;
	enemy.radius.y = 64.0f;
	enemy.direction.x = 0.0f;
	enemy.speed = 6.0f;
	enemy.gravity = 0.7f;
	enemy.velocity = 0.0f;
	enemy.shotCoolTime = 60;
	enemy.hp = 5;
	enemy.actionJudge = 0;
	enemy.isAction = false;
	enemy.isCanShot = false;
	enemy.isAlive = true;
	enemy.rivivalTime = 10;

	//敵の四隅
	enemy.rightTop.x = 0.0f;
	enemy.rightTop.y = 0.0f;
	enemy.leftBottom.x = 0.0f;
	enemy.leftBottom.y = 0.0f;

	//敵の画像切り替えの変数
	int enemyrMoveFlameCount = 0;
	int enemyMoveFlameNumber = 0;

	//シーン切り替え
	PlayerDirection enemyDirection = FRONT;

	//敵の画像
	int bossBackGraph = Novice::LoadTexture("./image/bossBack.png");
	int bossFrontGraph = Novice::LoadTexture("./image/bossFront.png");
	
	//敵の挙動　種類
	enum BossAction 
	{
		MOVE, //歩き
		DASH, //ダッシュ
		SHOCKWAVE, //衝撃波
		BLOW,  //パンチ・キック
	};


	BossAction bossAction = MOVE;

	unsigned int currentTime = static_cast<int>(time(nullptr));
	srand(currentTime);


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
		}

		//衝撃波描画
		if (player.isCanShot)
		{
			for (int i = 0; i < playerKickCount; i++)
			{
				if (!playerKick[i].isShot)
				{
					if (keys[DIK_E])
					{
						if (playerKickRadius.radiusAdd <= 20)
						{
							playerKickRadius.radiusAdd += 0.05f;
							player.isStore = true;
						}
					}
				}
			}

			if (!keys[DIK_E] && preKeys[DIK_E])
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
		if (keys[DIK_Q] && !preKeys[DIK_Q])
		{
			playerPunchKeyCount += 1;
			player.isAction = true;
		}
		
		if (player.isAction)
		{
			GraphAnimation(playerBlowFlameCount, playerBlowFlameNumber, 3);

			playerNextPunchCount++;

			if (playerNextPunchCount >= 5 && playerNextPunchCount <= 30 && playerPunchKeyCount == 1)
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

		Novice::ScreenPrintf(10, 40, "punchCount %d", playerNextPunchCount);
		Novice::ScreenPrintf(10, 70, "1 %d", player.isAction);
		Novice::ScreenPrintf(10, 90, "2 %d", player.isAction2);
		Novice::ScreenPrintf(10, 120,"punch2Count %d", playerPunch2Count);

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
	
		//敵の向き
		if (enemy.pos.x >= player.pos.x)
		{
			enemy.direction.x = -1.0f;
			enemyDirection = BACK;
		}
		else
		{
			enemy.direction.x = 1.0f;
			enemyDirection = FRONT;
		}

		//敵の攻撃 決める
		if (!enemy.isAction)
		{
			enemy.actionJudge = static_cast<int>(rand() % 10);
			enemy.isAction = true;
		}

		//敵の挙動
		if (enemy.isAlive)
		{
			GraphAnimation(enemyrMoveFlameCount, enemyMoveFlameNumber, 2);	//スプレットシートを動かす関数　使用
		}

		//敵の向き
		if (enemy.pos.x >= player.pos.x)
		{
			enemyDirection = BACK;
		}
		else
		{
			enemyDirection = FRONT;
		}

		//敵の攻撃を決める
		if (enemy.actionJudge >= 0)
		{
			bossAction = MOVE;
		}
		else if (enemy.actionJudge >= 3)
		{
			bossAction = DASH;
		}
		else if (enemy.actionJudge >= 5)
		{
			bossAction = SHOCKWAVE;
		}
		else if (enemy.actionJudge >= 7)
		{
			bossAction = BLOW;
		}

		//敵の攻撃の挙動制限
		switch (bossAction)
		{
		case MOVE:
			break;
		case DASH:
			break;
		case SHOCKWAVE:
			break;
		case BLOW:
			break;
		}

		//当たり判定
		//弾の左下、右上の座標
		for (int i = 0; i < playerKickCount; i++) 
		{
			playerKick[i].rightTop.x = playerKick[i].pos.x + playerKick[i].radius;
			playerKick[i].rightTop.y = playerKick[i].pos.y - playerKick[i].radius;
			playerKick[i].leftBottom.x = playerKick[i].pos.x - playerKick[i].radius;
			playerKick[i].leftBottom.y = playerKick[i].pos.y + playerKick[i].radius;
		}

		//敵の左下、右上の座標
		enemy.rightTop.x = enemy.pos.x + enemy.radius.x;
		enemy.rightTop.y = enemy.pos.y - enemy.radius.y;
		enemy.leftBottom.x = enemy.pos.x - enemy.radius.x;
		enemy.leftBottom.y = enemy.pos.y + enemy.radius.y;
		
		//プレイヤーの左下、右上の座標
		player.rightTop.x = player.pos.x + player.radius.x;
		player.rightTop.y = player.pos.y - player.radius.y;
		player.leftBottom.x = player.pos.x - player.radius.x;
		player.leftBottom.y = player.pos.y + player.radius.y;

		for (int i = 0; i < playerKickCount; i++) //弾と敵の当たり判定（矩形）
		{
			if (playerKick[i].rightTop.x > enemy.leftBottom.x && playerKick[i].leftBottom.x < enemy.rightTop.x && playerKick[i].rightTop.y < enemy.leftBottom.y && playerKick[i].leftBottom.y > enemy.rightTop.y)
			{
				enemy.isAlive = false;
			}
		}		

		//プレイヤーの横移動範囲
		MoveRange(player); 
		MoveRange(enemy);
			
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


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

			if (player.isAction2)
			{
				switch (playerDirection)
				{
				case FRONT:					
					Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, ((4 + playerBlowFlameNumber) * 128), 0, 128, 128, playerBlowFrontGraph, 1 / 6.0f, 1, 0.0f, WHITE);
					break;
				case BACK:
					Novice::DrawSpriteRect((int)player.pos.x - (int)player.radius.y, (int)player.pos.y - (int)player.radius.y, 0, 0, 128, 128, playerBlowBackGraph, 1 / 6.0f, 1, 0.0f, WHITE);
					break;
				default:
					break;
				}
			}
		}

		for (int i = 0; i < playerKickCount; i++) //キック
		{
			if (playerKick[i].isShot)
			{
				Novice::DrawEllipse((int)playerKick[i].pos.x, (int)playerKick[i].pos.y, (int)playerKick[i].radius, (int)playerKick[i].radius, 0.0f, BLUE, kFillModeSolid);
			}
		}

		Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y , (int)player.radius.x, (int)player.radius.y, 0.0f, BLUE, kFillModeWireFrame);

		Novice::DrawEllipse((int)player.pos.x , (int)player.pos.y,3, 3, 0.0f, RED, kFillModeWireFrame);

		if (enemy.isAlive)
		{
			switch (enemyDirection)
			{
			case FRONT:
				Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y,128 * enemyMoveFlameNumber,0,128,128,bossFrontGraph,1.0f/2.0f, 1.0f,0.0f,WHITE);
				break;
			case BACK:
				Novice::DrawSpriteRect((int)enemy.pos.x - (int)enemy.radius.y, (int)enemy.pos.y - (int)enemy.radius.y, 128 * enemyMoveFlameNumber, 0, 128, 128, bossBackGraph, 1.0f / 2.0f, 1.0f, 0.0f, WHITE);
				break;
			}
		}

		Novice::DrawEllipse((int)enemy.pos.x, (int)enemy.pos.y, (int)enemy.radius.x, (int)enemy.radius.y, 0.0f, BLUE, kFillModeWireFrame);

		Novice::ScreenPrintf(10, 10, "%d", player.jumpCount);

		Novice::ScreenPrintf(500, 10, "radiusAdd %f", playerKickRadius.radiusAdd);

		Novice::DrawLine(0, 600, 1280, 600, RED);

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
