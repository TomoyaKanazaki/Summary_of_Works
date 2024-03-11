//==========================================
//
//  ピクセルシェーダー(SimplePS.hlsl)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  VSOutput構造体定義
//==========================================
struct VSOutput
{
    float4 m_pos : SV_Position; // 位置座標
    float4 m_col : COLOR; // 頂点カラー
};

//==========================================
//  PSOutput構造体定義
//==========================================
struct PSOutput
{
    float4 m_col : SV_TARGET0; // ピクセルカラー
};

//==========================================
//  ピクセルシェーダのメインエントリ―ポイント
//==========================================
PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    
    output.m_col = input.m_col;
    
    return output;
}
