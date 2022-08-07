#pragma once
#include "../global/vars.h"

void inject_delstuff(bool replace);

void DrawLara(ITEM_INFO* item, long mirror);
void GetLaraJointPos(PHD_VECTOR* pos, long node);
void SetLaraUnderwaterNodes();
void Rich_CalcLaraMatrices_Normal(short* frame, long* bone, long flag);
void Rich_CalcLaraMatrices_Interpolated(short* frame1, short* frame2, long frac, long rate, long* bone, long flag);
void CalcLaraMatrices(long flag);

extern short* GLaraShadowframe;
extern long lara_matrices[180];
extern long lara_joint_matrices[180];
extern long LaraNodeAmbient[2];
extern long bLaraUnderWater;
extern uchar LaraNodeUnderwater[15];
extern char SkinVertNums[40][12];
extern char ScratchVertNums[40][12];
extern char HairRotScratchVertNums[5][12];
extern char bLaraInWater;
