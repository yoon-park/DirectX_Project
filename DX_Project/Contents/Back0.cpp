#include "PreCompile.h"
#include "Back0.h"

ABack0::ABack0()
{
	Renderer = CreateDefaultSubObject<USpriteRenderer>("Renderer");

	SetRoot(Renderer);
	InputOn();
}

ABack0::~ABack0()
{

}

void ABack0::BeginPlay()
{
	Super::BeginPlay();

	Renderer->SetSprite("hills_02.png");
	Renderer->SetAutoSize(1.5f, true);
	Renderer->SetOrder(ERenderOrder::Back0);
}

void ABack0::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}