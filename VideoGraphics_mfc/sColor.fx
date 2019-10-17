
cbuffer cbmvp : register(b0)
{
	matrix mvp;
};

cbuffer singleColor : register(b1)
{
	float4 singleC;
};


float4 VS_Main(float4 pos:POSITION) : SV_POSITION
{
	return mul(pos, mvp);
}

float4 PS_Main(float4 pos:SV_POSITION):SV_TARGET
{
	return singleC;
}