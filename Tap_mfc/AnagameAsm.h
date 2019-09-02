#pragma once
#define UCHAR unsigned char

bool Initialize(UINT codeSize, UINT StackSize, UINT DataSize);

void add(UCHAR det, int dest, int source);

void add_64(UCHAR det, int dest, int source);

void addf(UCHAR det, int dest, int source);

void addf_64(UCHAR det, int dest, int source);

void addif(UCHAR det, int dest, int source);

void addfi(UCHAR det, int dest, int source);

void addif_64(UCHAR det, int dest, int source);

void addfi_64(UCHAR det, int dest, int source);