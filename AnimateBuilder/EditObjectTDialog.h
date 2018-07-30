#pragma once
#include <TDialog.h>
#include <ArenaModel.h>


#define ED_3D_DIALOG_METHOD_COUNT 26

class EditObjectTDialog;

typedef void (EditObjectTDialog::*EdmethodArray)(TControl* tc, EventArgs ea);

class EditObjectTDialog :public TDialog
{
public:
	EditObjectTDialog();
	~EditObjectTDialog();


	bool getValidConstruction();
	//void initializeControls();

	bool InitializeEngine();
	void initializeControls();

	void onDraw() override;

	// The Anaface Level Event Handlers
	void OnModelLocationNear(TControl* tc, EventArgs ea);
	void OnModelLocationFar(TControl* tc, EventArgs ea);
	void OnModelLocationUp(TControl* tc, EventArgs ea);
	void OnModelLocationDown(TControl* tc, EventArgs ea);
	void OnModelLocationLeft(TControl* tc, EventArgs ea);
	void OnModelLocationRight(TControl* tc, EventArgs ea);

	void OnModelRotationNear(TControl* tc, EventArgs ea);
	void OnModelRotationFar(TControl* tc, EventArgs ea);
	void OnModelRotationUp(TControl* tc, EventArgs ea);
	void OnModelRotationDown(TControl* tc, EventArgs ea);
	void OnModelRotationLeft(TControl* tc, EventArgs ea);
	void OnModelRotationRight(TControl* tc, EventArgs ea);

	void OnModelRotationGrow(TControl* tc, EventArgs ea);
	void OnModelRotationShrink(TControl* tc, EventArgs ea);

	void OnCameraLocationNear(TControl* tc, EventArgs ea);
	void OnCameraLocationFar(TControl* tc, EventArgs ea);
	void OnCameraLocationUp(TControl* tc, EventArgs ea);
	void OnCameraLocationDown(TControl* tc, EventArgs ea);
	void OnCameraLocationLeft(TControl* tc, EventArgs ea);
	void OnCameraLocationRight(TControl* tc, EventArgs ea);

	void OnCameraRotationNear(TControl* tc, EventArgs ea);
	void OnCameraRotationFar(TControl* tc, EventArgs ea);
	void OnCameraRotationUp(TControl* tc, EventArgs ea);
	void OnCameraRotationDown(TControl* tc, EventArgs ea);
	void OnCameraRotationLeft(TControl* tc, EventArgs ea);
	void OnCameraRotationRight(TControl* tc, EventArgs ea);

protected:
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecPointer<TArena> arenaControl;
	TString fileHolder;


	void MessageHandler()override;
	bool constructionWorked;
	EdmethodArray _3DDialogMethods[ED_3D_DIALOG_METHOD_COUNT];
	TrecPointer<ArenaModel> targetModel;
};

