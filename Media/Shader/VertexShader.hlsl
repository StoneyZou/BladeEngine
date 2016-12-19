float4 vs_main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

float4 ps_main(float4 pos : SV_POSITION) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0);
}