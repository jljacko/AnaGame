#pragma once
#include "BuilderApp.h"

class ArenaApp;
typedef void (ArenaApp::* ArenaBuilderEvents)(TControl* tc, EventArgs ea);


class ArenaApp :
	public BuilderApp
{
public:
	ArenaApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance>, TString& arenaName);
	~ArenaApp();

	bool InitializeControls() override;
	TString GetArenaName();

	RECT Get3DArea()override;

	void SetColor(D2D1_COLOR_F c);

	void MessageHandler() override;
	TDataArray<ArenaBuilderEvents> arenaHandlers;
	void SetArenaName(TString& name);

	void UpdateCameraText();
protected:
	TArena* arena;
	TrecPointer<ArenaEngine> modelCollection;
	TrecPointer<TArray<TObject>> models;
	TrecPointer<ArenaModel> scale;
	TTextField *d_x, *d_y, *d_z, *l_x, *l_y, *l_z;
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

