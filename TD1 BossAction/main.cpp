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
	bool isAlive;
	int actionJudge;
	int shotCoolTime;
	int hp;
	int damege;
	int animationFlameCount;
	int flameNumber;
};

struct  Bullet
{
	Vector2 pos;
	Vector2 direction;
	float speed;
	float wide;
	float height;
	float radius;
	bool isShot;
	int damege;
};

//関数作成

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
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
	{
		player.velocity = -player.jumpPower;
		player.isJump = true;
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

Bullet playerShockWave[3]; //衝撃波
void ShockWave(Charactor& player) //衝撃波の描画
{	
	for (int i = 0; i < 3; i++)
	{
		if (!playerShockWave[i].isShot)
		{
			player.shotCoolTime = 60;
			playerShockWave[i].isShot = true;
			playerShockWave[i].pos.x = player.pos.x;
			playerShockWave[i].pos.y = player.pos.y;
			playerShockWave[i].direction.x = player.directionVecter;
			playerShockWave[i].radius = 8.0f;
			playerShockWave[i].damege = 1;
			break;
		}
	}
}

void MoveShockWave()
{
	for (int i = 0; i < 3; i++)
	{
		playerShockWave[i].pos.x += playerShockWave[i].direction.x * playerShockWave[i].speed;
	}
}

void ShockWaveRange() //衝撃波のfalse
{
	for (int i = 0; i < 3; i++)
	{
		if (playerShockWave[i].pos.x - playerShockWave[i].radius >= 1280.0f || playerShockWave[i].pos.x + playerShockWave[i].radius <= 0.0f)
		{
			playerShockWave[i].isShot = false;
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

	int playerMoveFlameCount = 0;
	int playerMoveFlameNumber = 0;

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


	//衝撃波の変数　初期化
	for (int i = 0; i < 3; i++)
	{
		playerShockWave[i].direction.x = 1.0f;
		playerShockWave[i].pos.x = player.pos.x;
		playerShockWave[i].pos.y = player.pos.y;
		playerShockWave[i].speed = 10.0f;
		playerShockWave[i].radius = 8.0f;
		playerShockWave[i].isShot = false;
	}

	Charactor enemy;
	enemy.pos.x = 700.0f;
	enemy.pos.y = 300.0f;
	enemy.radius.x = 64.0f;
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

	/*int enemyMoveCount = 0;
	int enemyMoveGraphCount = 0;*/

	PlayerDirection enemyDirection = FRONT;

	int enemyBackGraph = Novice::LoadTexture("./image/enemyBack.png");
	int enemyFrontGraph = Novice::LoadTexture("./image/enemyFront.png");
	
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
		CharactorMove(player,keys); 

		if (keys[DIK_D])
		{
			player.directionVecter = 1.0f;
			player.direction.x = 1.0f;
			playerDirection = FRONT;		
			playerMoveDirection = FRONT;
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
			GraphAnimation(playerMoveFlameCount, playerMoveFlameNumber, 4);
		}

		// プレイヤーのジャンプ
		Jump(player,keys,preKeys); 

		//衝撃波描画
		if (player.isCanShot)
		{
			if (keys[DIK_Q] && !preKeys[DIK_Q])
			{
				ShockWave(player);
			}
		}

		aa;
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

		//敵の攻撃
		if (!enemy.isAction)
		{
			enemy.actionJudge = static_cast<int>(rand() % 10);
			enemy.isAction = true;
		}

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

		Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y , (int)player.radius.x, (int)player.radius.y, 0.0f, BLUE, kFillModeWireFrame);

		Novice::DrawEllipse((int)player.pos.x , (int)player.pos.y,3, 3, 0.0f, RED, kFillModeWireFrame);

		if (enemy.isAlive)
		{
			switch (enemyDirection)
			{
			case FRONT:
				Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.x, (int)enemy.pos.y - (int)enemy.radius.y, enemyFrontGraph, 1, 1, 0.0f, WHITE);
				break;
			case BACK:
				Novice::DrawSprite((int)enemy.pos.x - (int)enemy.radius.x, (int)enemy.pos.y - (int)enemy.radius.y, enemyBackGraph, 1, 1, 0.0f, WHITE);
				break;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (playerShockWave[i].isShot)
			{
				Novice::DrawEllipse((int)playerShockWave[i].pos.x, (int)playerShockWave[i].pos.y, (int)playerShockWave[i].radius, (int)playerShockWave[i].radius, 0.0f, BLUE, kFillModeSolid);
			}
		}

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
