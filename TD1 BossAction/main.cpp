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
	float speed;
	float wide;
	float height;
	float radius;
	float velocity;
	float gravity;
	float jumpPower;
	bool isJump;
	bool isCanShot;
	bool action;
	bool isAlive;
	int shotCoolTime;
	int hp;
	int damege;
	int animationFlameCount;
	int flameNumber;
};

struct  Bullet
{
	Vector2 pos;
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
	if (keys[DIK_D])
	{
		player.pos.x += player.speed;
	}

	if (keys[DIK_A])
	{
		player.pos.x -= player.speed;
	}
}

void Jump(Charactor& player,char* keys,char* preKeys) //プレイヤージャンプ
{	
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
	{
		player.velocity = -player.jumpPower;
		player.isJump = true;
	}
	
	player.velocity += player.gravity;
	player.pos.y += player.velocity;

	if (player.pos.y + player.radius >= 500)
	{
		player.pos.y = 500 - player.radius;
		player.isJump = false;
		player.velocity = 0.0f;
	}
	
}

void MoveRange(Charactor& player) //プレイヤーの横移動範囲
{
	if (player.pos.x - player.radius <= 0.0f)
	{
		player.pos.x = 0 + player.radius;
	}

	if (player.pos.x + player.radius >= 1280.0f)
	{
		player.pos.x = 1280.0f - player.radius;
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
			playerShockWave[i].radius = 8.0f;
			playerShockWave[i].damege = 1;
			break;
		}
	}
}

void ShockWaveMove() //衝撃波の移動
{
	for (int i = 0; i < 3; i++)
	{
		if (playerShockWave[i].isShot)
		{
			playerShockWave[i].pos.x += playerShockWave[i].speed;
		}

		if (playerShockWave[i].pos.x - playerShockWave[i].radius >= 1280.0f)
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
	player.radius = 64.0f;
	player.speed = 6.0f;
	player.velocity = 0.0f;
	player.gravity = 0.7f;
	player.jumpPower = 18.0f;
	player.shotCoolTime = 60;
	player.hp = 5;
	player.isJump = false;
	player.isCanShot = false;

	//衝撃波の変数　初期化
	for (int i = 0; i < 3; i++)
	{
		playerShockWave[i].pos.x = player.pos.x;
		playerShockWave[i].pos.y = player.pos.y;
		playerShockWave[i].speed = 10.0f;
		playerShockWave[i].radius = 8.0f;
		playerShockWave[i].isShot = false;
	}

	Charactor enemy;
	enemy.pos.x = 200.0f;
	enemy.pos.y = 700.0f;
	enemy.radius = 64.0f;
	enemy.speed = 6.0f;
	enemy.gravity = 0.7f;
	enemy.shotCoolTime = 60;
	enemy.hp = 5;
	enemy.action = false;
	enemy.isCanShot = false;

	enum BossAction 
	{
		MOVE,
		DASH,
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

		// プレイヤーのジャンプ
		Jump(player,keys,preKeys); 

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

		//衝撃波描画
		if (player.isCanShot)
		{
			if (keys[DIK_Q] && !preKeys[DIK_Q])
			{
				ShockWave(player);
			}
		}

		//衝撃波の移動
		ShockWaveMove();

		//プレイヤーの横移動範囲
		MoveRange(player); 

		Novice::ScreenPrintf(10, 20, "%d", player.shotCoolTime);

	
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y, (int)player.radius, (int)player.radius, 0.0f, RED, kFillModeSolid);;

		for (int i = 0; i < 3; i++)
		{
			if (playerShockWave[i].isShot)
			{
				Novice::DrawEllipse((int)playerShockWave[i].pos.x, (int)playerShockWave[i].pos.y, (int)playerShockWave[i].radius, (int)playerShockWave[i].radius, 0.0f, BLUE, kFillModeSolid);
			}

			Novice::ScreenPrintf(10, 40 * i, "%f", playerShockWave[i].isShot);
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
