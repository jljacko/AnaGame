
cbuffer singleColorBuffer : register(b0)
{
	matrix mvp;
};

Texture2D cMap : register(t0);
SamplerState colorSample : register(s0);

struct VSData
{
	float4 pos: POSITION;
	float2 col: TEXCOORD0;
};

struct PSData
{
	float4 pos: SV_POSITION;
	float2 col: TEXCOORD0;
};

PSData VS_Main(VSData vDat) 
{
	PSData returnable = (PSData)0;
	returnable.pos = mul(vDat.pos,mvp);
	returnable.col = vDat.col;
	return returnable;
}

float4 PS_Main(PSData pDat) :SV_TARGET
{
	return cMap.Sample(colorSample, pDat.col);
}