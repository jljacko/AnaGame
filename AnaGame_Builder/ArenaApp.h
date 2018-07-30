#pragma once
#include "BuilderApp.h"

#define ARENA_APP_COUNT 14

class ArenaApp;
typedef void (ArenaApp::*ArenaBuilderEvents)(TControl* tc, EventArgs ea);

class ArenaApp : public BuilderApp
{
	// friend class CAnaGame_BuilderDoc;
	friend class CAnaGameBuilderDoc;
public:
	ArenaApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, HWND w);
	~ArenaApp();

	bool InitializeControls() override;
	TString GetArenaName();

	RECT Get3DArea()override;

	void SetColor(D2D1::ColorF c);
	
	TrecPointer<ArenaEngine> GetArenaEngine()override;

private:
	void MessageHandler() override;
	ArenaBuilderEvents handlers[ARENA_APP_COUNT];
	void SetArenaName(TString& name);

	void UpdateCameraText();
protected:
	TrecPointer<TArena> arena;
	TrecPointer<ArenaEngine> modelCollection;
	TrecPointer<ArenaModel> scale;
	TrecPointer<TTextField> d_x, d_y, d_z, l_x, l_y, l_z;
	bool rotateMode;
	TString arenaName;


	void TextDirectionX(TControl* tc, EventArgs ea);
	void TextLocationX(TControl* tc, EventArgs ea);
	void TextDirectionY(TControl* tc, EventArgs ea);
	void TextLocationY(TControl* tc, EventArgs ea);
	void TextDirectionZ(TControl* tc, EventArgs ea);
	void TextLocationZ(TControl* tc, EventArgs ea);
	void OnUp(TControl* tc, EventArgs ea);
	void OnDown(TControl* tc, EventArgs ea);
	void OnLeft(TControl* tc, EventArgs ea);
	void OnRight(TControl* tc, EventArgs ea);
	void OnNear(TControl* tc, EventArgs ea);
	void OnFar(TControl* tc, EventArgs ea);
	void OnSetCameraRotate(TControl* tc, EventArgs ea);
	void OnSetCameraTranslate(TControl* tc, EventArgs ea);
};

