struct PS_INPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{


	/*float3 edgeColor = float3(1.0f, 0.0f, 0.0f);
	float edgeThreshold = 1.0f;

	// Определяем текущую координату пикселя
	float3 pixelCoord = input.position.xyz / input.position.w;

	// Определяем, находится ли пиксель на ребре
	bool isOnEdge = (pixelCoord.x <= edgeThreshold || pixelCoord.y <= edgeThreshold || pixelCoord.z <= edgeThreshold);

	// Если пиксель находится на ребре, возвращаем цвет ребра, иначе возвращаем исходный цвет
	return isOnEdge ? float4(edgeColor, 1.0f) : float4(input.color, 1.0f);*/


	return float4(0.0f, 1.0f, 0.6f, 1.0f);
}