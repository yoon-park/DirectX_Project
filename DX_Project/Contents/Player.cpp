#include "PreCompile.h"
#include "Player.h"

APlayer::APlayer()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>("Renderer");
	
	InputOn();
}

APlayer::~APlayer()
{

}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(300.0f, 300.0f, 100.0f));
	
	Renderer->CreateAnimation("Die", "Die");
	Renderer->CreateAnimation("Idle", "Idle");
	Renderer->CreateAnimation("Jump", "Jump");
	Renderer->CreateAnimation("Run", "Run", 1.0f);
	StateInit();

	Renderer->SetAutoSize(10.0f, true);
	Renderer->SetOrder(ERenderOrder::Player);
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	State.Update(_DeltaTime);
}