//==========================================
//
//  頂点シェーダー(SimpleVS.hlsl)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  VSInput構造体定義
//==========================================
struct VSInput
{
    float3 m_pos : POSITION; // 位置座標
    float4 m_col : COLOR; // 頂点カラー
};

//==========================================
//  VSOutput構造体定義
//==========================================
struct VSOutput
{
    float4 m_pos : SV_Position; // 位置座標
    float4 m_col : COLOR; // 頂点カラー
};

//==========================================
//  Transform定数バッファを追加
//==========================================
cbuffer Transform : register(b0)
{
    float4x4 World : packoffset(c0); // ワールド行列
    float4x4 View : packoffset(c4); // ビュー行列
    float4x4 Proj : packoffset(c8); // 射影行列
}

//==========================================
//  頂点シェーダのメインエントリーポイント
//==========================================
VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    float4 localPos = float4(input.m_pos, 1.0f);
    float4 worldPos = (World, localPos);
    float4 viewPos = mul(View, worldPos);
    float4 projPos = mul(Proj, viewPos);

    output.m_pos = projPos;
    output.m_col = input.m_col;
    
    return output;
}
