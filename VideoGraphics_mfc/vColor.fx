
cbuffer singleColorBuffer : register(b0)
{
	matrix mvp;
};

struct VSData
{
	float4 pos: POSITION;
	float4 col: COLOR;
};

struct PSData
{
	float4 pos: SV_POSITION;
	float4 col: COLOR;
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
	return pDat.col;
}