cbuffer modelBuffer : register(b0)
{
	matrix model;
};

cbuffer veiwBuffer: register(b1)
{
	matrix view;
};

cbuffer projBuffer: register(b2)
{
	matrix proj;
};

cbuffer singleColorBuffer : register(b3)
{
	float4 singleC;
};



float4 VS_Main(float3 pos3:POSITION) : SV_POSITION
{
	matrix mvp = mul(model, view);
	mvp = mul(mvp, proj);
	float4 pos = float4(pos3,1.0);
	
	return mul(pos, mvp);

	//return mul(pos, mul(proj, mul(view, model)));

//	return mul(proj, mul(view, mul(model, pos)));
}

float4 PS_Main(float4 pos:SV_POSITION):SV_TARGET
{
	return float4(singleC);
}