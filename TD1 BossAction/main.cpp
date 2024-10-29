#include <Novice.h>

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
	bool action;
	bool isAlive;
	bool isCanShot;
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

void CharactorMove(Charactor& player, char* keys)
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

void GraphAnimation(int& animationFlameCount, int& flameNunber, int graphSheet) //　リソーススプレットシートの画像切り替わり
{
	animationFlameCount++;
	flameNunber = (animationFlameCount / 10) % graphSheet;

	if (animationFlameCount >= graphSheet * 10)
	{
		animationFlameCount = 0;
	}
}

void PlayerMovingRangeJump(Charactor& player,char* keys,char* preKeys) //プレイヤーの移動範囲制限
{	
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
	{
		player.velocity = -player.jumpPower;
		player.isJump = true;
	}
	
	player.velocity += player.gravity;
	player.pos.y += player.velocity;

	if (player.pos.x - player.radius <= 0.0f)
	{
		player.pos.x = 0 + player.radius;
	}

	if (player.pos.x + player.radius >= 1280.0f)
	{
		player.pos.x = 1280.0f - player.radius;
	}

	if (player.pos.y + player.radius >= 500)
	{
		player.pos.y = 500 - player.radius;
		player.isJump = false;
		player.velocity = 0.0f;
	}
	
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Charactor player;
	player.pos.x = 200.0f;
	player.pos.y = 300.0f;
	player.wide = 64.0f;
	player.height = 64.0f;
	player.radius = 32.0f;
	player.speed = 5.0f;
	player.velocity = 0.0f;
	player.gravity = 0.7f;
	player.jumpPower = 18.0f;
	player.isJump = false;

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
		CharactorMove(player,keys); // 左右移動

		PlayerMovingRangeJump(player,keys,preKeys); //移動範囲　プレイヤーのジャンプ

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y, (int)player.radius, (int)player.radius, 0.0f, RED, kFillModeSolid);;

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
