#pragma once

namespace UEngineDebug
{
	#define LeakCheck _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	#define MsgBoxAssert(Value) \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), "ġ������ ����", MB_OK); assert(false);

	void OutPutDebugText(std::string_view _DebugText);
}