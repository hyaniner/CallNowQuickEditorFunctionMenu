﻿/*
Copyright (c) 2023 hyaniner@outlook.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "CallNowBPEditor.h"

#include "BlueprintEditorModule.h"
#include "GraphEditor.h"

#define LOCTEXT_NAMESPACE "FCallNowBPEditorModule"


class FPinFactoryTestCommands : public TCommands<FPinFactoryTestCommands>
{
public:

	FPinFactoryTestCommands()
		: TCommands<FPinFactoryTestCommands>
		(
			TEXT("CallNowFunctionBPEditor")
			, LOCTEXT("CallNowFunctionBPEditorCommands", "CallNowFunctionBPEditorCommands")
			, NAME_None
			, FAppStyle::Get().GetStyleSetName()
		)
	{}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > CallNowFunctionBPEditor;
	
};

void FPinFactoryTestCommands::RegisterCommands()
{
	UI_COMMAND(CallNowFunctionBPEditor, "CallNowFunctionBPEditor", "CallNowFunctionBPEditor", EUserInterfaceActionType::Button, FInputChord());
}

void FCallNowBPEditorModule::CallNowBPTestFunctionBody()
{
	UE_LOG(LogTemp, Warning, TEXT("CallNowBPTestFunctionBody called successfully"));
}

void FCallNowBPEditorModule::StartupModule()
{
	if(!IsRunningCommandlet())
	{
		FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FCallNowBPEditorModule::OnFEngineLoopInitComplete);	
	}
		
}

void FCallNowBPEditorModule::ShutdownModule()
{
	if(!IsRunningCommandlet())
	{
		FPinFactoryTestCommands::Unregister();
		TestCommands.Reset();	
	}
}


void FCallNowBPEditorModule::OnFEngineLoopInitComplete()
{
	FPinFactoryTestCommands::Register();
	
	TestCommands = MakeShareable(new FUICommandList);
	TestCommands->MapAction(
		FPinFactoryTestCommands::Get().CallNowFunctionBPEditor
		, FExecuteAction::CreateRaw(this, &FCallNowBPEditorModule::CallNowBPTestFunctionBody)
		, FCanExecuteAction()
		);
	
	FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BlueprintEditorModule.GetsSharedBlueprintEditorCommands()->Append(TestCommands.ToSharedRef());
	TSharedPtr<FExtensibilityManager> ExtensibilityManager = BlueprintEditorModule.GetMenuExtensibilityManager();
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuBarExtension("Tools", EExtensionHook::After, TestCommands, FMenuBarExtensionDelegate::CreateRaw(this, &FCallNowBPEditorModule::MakePullDownMenuBlueprintEditor));
	ExtensibilityManager->AddExtender(MenuExtender);
	
}


void FCallNowBPEditorModule::MakePullDownMenuBlueprintEditor(FMenuBarBuilder& menuBuilder)
{
	menuBuilder.AddPullDownMenu(
	LOCTEXT("CallNowBPAtMenubarLabel", "CallNowBP"),
	LOCTEXT("CallNowBPAtMainMenubarDesc", "CallNowBP: Menu section to place functions you want to call right now, in the Blueprint Editor"),
	FNewMenuDelegate::CreateRaw(this, &FCallNowBPEditorModule::FillPullDownMenuBlueprintEditor),
	"FCallNowBPEditorModule"	
	);
}

void FCallNowBPEditorModule::FillPullDownMenuBlueprintEditor(FMenuBuilder& menuBuilder)
{
	menuBuilder.BeginSection("BPPatternFindReplaceMenuSection");
	
	menuBuilder.AddMenuEntry(
		FPinFactoryTestCommands::Get().CallNowFunctionBPEditor		
		, TEXT("CallNowBPEditorHookMenu")
		, LOCTEXT("MenuItemCallBPEditorFunctionLabel", "CallNowBP Test Function")
		, LOCTEXT("MenuItemCallBPEditorFunctionDesc", "CallNowBP Test Function")
		, FSlateIcon()
		);

	menuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCallNowBPEditorModule, CallNowBPEditor)