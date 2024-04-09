#include "PreCompile.h"
#include "Level.h"

#include "EngineCore.h"
#include "GameMode.h"
#include "Renderer.h"
#include "Camera.h"
#include "EngineRenderTarget.h"

bool ULevel::IsActorConstructor = true;

ULevel::ULevel()
{
	MainCamera = SpawnActor<UCamera>("MainCamera");
	UICamera = SpawnActor<UCamera>("NewActor");
	UICamera->InputOff();
}

ULevel::~ULevel()
{

}

void ULevel::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			Actor->Tick(_DeltaTime);
		}
	}
}

void ULevel::Render(float _DeltaTime)
{
	MainCamera->ViewPortSetting();
	GEngine->GetEngineDevice().BackBufferRenderTarget->Setting();
	MainCamera->CameraTransformUpdate();

	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& GroupRenderers = RenderGroup.second;

		for (std::shared_ptr<URenderer> Renderer : GroupRenderers)
		{
			if (Renderer->IsActive() == false)
			{
				continue;
			}

			Renderer->RenderingTransformUpdate(MainCamera);
			Renderer->Render(_DeltaTime);
		}
	}
}

void ULevel::LevelStart(ULevel* _PrevLevel)
{
	Super::LevelStart(_PrevLevel);

	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			Actor->LevelStart(_PrevLevel);
		}
	}
}

void ULevel::LevelEnd(ULevel* _NextLevel)
{
	Super::LevelEnd(_NextLevel);

	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			Actor->LevelEnd(_NextLevel);
		}
	}
}

void ULevel::ConstructorActor(std::shared_ptr<AActor> _Actor)
{
	_Actor->RootCheck();
}

void ULevel::PushActor(std::shared_ptr<AActor> _Actor)
{
	if (_Actor == nullptr)
	{
		MsgBoxAssert("만들어지지 않은 액터를 레벨에 추가하려 했습니다.");
		return;
	}

	_Actor->SetWorld(this);
	_Actor->BeginPlay();

	Actors[_Actor->GetOrder()].push_back(_Actor);
}

void ULevel::PushRenderer(std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_Renderer->GetOrder()].push_front(_Renderer);
}