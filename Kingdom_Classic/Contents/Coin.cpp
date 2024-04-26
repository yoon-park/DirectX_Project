#include "PreCompile.h"
#include "Coin.h"

#include "PlayGameMode.h"
#include "Player.h"

ACoin::ACoin()
{
	UDefaultSceneComponent* Root = CreateDefaultSubObject<UDefaultSceneComponent>("Renderer");

	Renderer = CreateDefaultSubObject<USpriteRenderer>("Renderer");
	Renderer->SetupAttachment(Root);

	Collision = CreateDefaultSubObject<UCollision>("Collision");
	Collision->SetupAttachment(Root);
	Collision->SetScale(FVector(10.0f, 10.0f, 100.0f));
	Collision->SetCollisionGroup(ECollisionOrder::Coin);
	Collision->SetCollisionType(ECollisionType::Rect);

	SetRoot(Root);
}

ACoin::~ACoin()
{

}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	Renderer->SetAutoSize(1.0f, true);
	Renderer->SetOrder(ERenderOrder::UI);

	Renderer->CreateAnimation("Pay", "Coin_Pay", 0.10f);
	Renderer->CreateAnimation("Spin", "Coin_Spin", 0.10f);

	StateInit();
}

void ACoin::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	State.Update(_DeltaTime);
}

void ACoin::StateInit()
{
	{
		State.CreateState("Idle");
		State.SetStartFunction("Idle", std::bind(&ACoin::IdleStart, this));
		State.SetUpdateFunction("Idle", std::bind(&ACoin::Idle, this, std::placeholders::_1));

		State.CreateState("Fall");
		State.SetStartFunction("Fall", std::bind(&ACoin::FallStart, this));
		State.SetUpdateFunction("Fall", std::bind(&ACoin::Fall, this, std::placeholders::_1));

		State.CreateState("Pay");
		State.SetStartFunction("Pay", std::bind(&ACoin::PayStart, this));
		State.SetUpdateFunction("Pay", std::bind(&ACoin::Pay, this, std::placeholders::_1));
	}

	if (APlayGameMode::MainPlayer->GetCurSpot() != nullptr)
	{
		State.ChangeState("Pay");
	}
	else
	{
		State.ChangeState("Fall");
	}
}

void ACoin::IdleStart()
{
	Renderer->ChangeAnimation("Spin");
}

void ACoin::FallStart()
{
	Renderer->ChangeAnimation("Spin");

	Speed = 150.0f;
}

void ACoin::PayStart()
{
	Renderer->ChangeAnimation("Pay");

	Speed = 50.0f;
}

void ACoin::Idle(float _DeltaTime)
{
	/* Player, Npc, Monster�� �浹 �� ���� */
}

void ACoin::Fall (float _DeltaTime)
{
	/* �ٴڿ� ������ ground�� �ȼ��浹 �˻� */
}

void ACoin::Pay(float _DeltaTime)
{
	if (APlayGameMode::MainPlayer->GetIsPaying() == false)
	{
		State.ChangeState("Fall");
	}

	FVector Indicator_Location = APlayGameMode::MainPlayer->GetCurSpot()->GetCoin00Location();
	FVector Coin_Location = GetActorLocation();

	FVector Diff = Indicator_Location - Coin_Location;
	FVector Dir = Diff.Normalize3DReturn();

	AddActorLocation(Dir * _DeltaTime * 100);
}