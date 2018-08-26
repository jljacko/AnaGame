#pragma once
#include <TDialog.h>
#include <ArenaModel.h>
#define T3D_DIALOG_METHOD_COUNT 15

class NewObjectTDialog;

typedef void (NewObjectTDialog::*methodArray)(TControl* tc, EventArgs ea);

typedef enum ModelChangeMode
{
	mc_not_set,
	mc_size,
	mc_rotate,
	mc_translate
}ModelChangeMode;

class NewObjectTDialog : public TDialog
{
public:
	NewObjectTDialog();
	~NewObjectTDialog();


	// Event Handlers
	void OnSelectCube(TControl* tc, EventArgs ea);
	void OnSelectSphere(TControl* tc, EventArgs ea);
	void OnSelectTrianglePyramid(TControl* tc, EventArgs ea);
	void OnSelectSquarePyramid(TControl* tc, EventArgs ea);
	void OnSelectCone(TControl* tc, EventArgs ea);
	void OnSelectCylinder(TControl* tc, EventArgs ea);

	void OnSetToSize(TControl* tc, EventArgs ea);
	void OnSetToRotate(TControl* tc, EventArgs ea);
	void OnSetToTranslate(TControl* tc, EventArgs ea);

	void OnFar(TControl* tc, EventArgs ea);
	void OnUp(TControl* tc, EventArgs ea);
	void OnNear(TControl* tc, EventArgs ea);
	void OnLeft(TControl* tc, EventArgs ea);
	void OnDown(TControl* tc, EventArgs ea);
	void OnRight(TControl* tc, EventArgs ea);

	void OnUpdateNear(TControl* tc, EventArgs ea);
	void OnUpdateFar(TControl* tc, EventArgs ea);

	bool getValidConstruction();
	void initializeControls();

	bool InitializeEngine();


	void onDraw() override;


	TrecPointer<ArenaModel> GetModel();
private:
	TrecPointer<TLayout> stack, topGallary, bottomGallary;
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecPointer<TArena> arenaControl;
	TrecPointer<TArray<styleTable>> table;
	void MessageHandler()override;
	TString fileHolder;
	bool constructionWorked;

	methodArray _3DDialogMethods[T3D_DIALOG_METHOD_COUNT];
	TrecPointer<ArenaModel> targetModel, scale;

	ModelChangeMode mcm;
};

