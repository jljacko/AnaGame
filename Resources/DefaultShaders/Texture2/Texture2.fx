
cbuffer singleColorBuffer : register(b0)
{
	matrix mvp;
};

Texture2D cMap : register(t0);
SamplerState colorSample : register(s0);

Texture2D cMap2 : register(t1);
SamplerState colorSample2 : register(s1);

struct VSData
{
	float4 pos: POSITION;
	float2 col: TEXCOORD0;
	float2 col2: TEXCOORD1;
};

struct PSData
{
	float4 pos: SV_POSITION;
	float2 col: TEXCOORD0;
	float2 col2: TEXCOORD1;
};

PSData VS_Main(VSData vDat) 
{
	PSData returnable = (PSData)0;
	returnable.pos = mul(vDat.pos,mvp);
	returnable.col = vDat.col;
	returnable.col2 = vDat.col2;
	return returnable;
}

float4 PS_Main(PSData pDat) :SV_TARGET
{
	return float4((cMap.Sample(colorSample, pDat.col) + cMap2.Sample(colorSample2, pDat.col2)) / 2);
}